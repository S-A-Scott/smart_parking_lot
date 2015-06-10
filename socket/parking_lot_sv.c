#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "become_daemon.h"
#include "parking_lot.h"
#include "log_message.h"


static const char *logFilename = "/tmp/parking_lot.log";
static struct parking_msg latestParkingMsg;
int filedes[2];

static void
signalHandler(int sig)
{
	int savedErrno;

	savedErrno = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		continue;
	errno = savedErrno;
}

static void
handleRequest(int cfd)
{
	struct parking_msg parkingMsg;
	ssize_t numRead;
	int j;
	char situation[4096];

	if ((numRead = read(cfd, &parkingMsg, sizeof(parkingMsg))) > 0) {
		if (parkingMsg.length == 2) {
			logMessage("Threre is %d parking space", parkingMsg.length);
			//parkingMsg.parkingSpace[parkingMsg.length] = '\0';
			for (j = 0; j < parkingMsg.length; j++) {
				sprintf(situation, "NO.%-4d -- %s",
					j, (parkingMsg.parkingSpace[j] == '1' || parkingMsg.parkingSpace[j] == 1) ? "used" : "unused");
				logMessage("parking space used situation: %s", situation);
			}
			if (write(filedes[1], &parkingMsg, sizeof(parkingMsg)) != sizeof(parkingMsg))
				logMessage("write (%s)", strerror(errno));
		}

		if (parkingMsg.length == 8) {
			char result_to_client[1024];
			sprintf(result_to_client, "%c%c%c%c",
					(latestParkingMsg.parkingSpace[0] == '1' ? '1' : '0'),
					(latestParkingMsg.parkingSpace[1] == '1' ? '1' : '0'),
					(latestParkingMsg.parkingSpace[2] == '1' ? '1' : '0'),
					(latestParkingMsg.parkingSpace[3] == '1' ? '1' : '0'));
			//sprintf(result_to_client, "%c%c%c%c", '0', '1', '1', '1');
			result_to_client[strlen(result_to_client)] = '\0';
			logMessage("result_to_client %s", result_to_client);
			if (send(cfd, result_to_client, 4, 0) == -1)
				logMessage("send result_to_client %s", strerror(errno));
			if (write(filedes[1], &latestParkingMsg, sizeof(parkingMsg)) != sizeof(parkingMsg))
				logMessage("write (%s)", strerror(errno));
			//logMessage("moblie phone has connected");
		}
	}
}


int
main(int argc, char *argv[])
{
	int lfd, cfd;		/* Listening and connected sockets */
	int numRead;

	struct sigaction sa;

	if (becomeDaemon(0) == -1)
		errExit("becomeDaemon");

	logOpen(logFilename);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = signalHandler;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		logMessage("Error from sigaction(): %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (pipe(filedes) == -1) {
		logMessage("Error from pipe(): %s", strerror(errno));
		exit(EXIT_FAILURE);
	}


	lfd = inetListen(PORT_NUM, 512, NULL);
	if (lfd == -1) {
		logMessage("Could not create server socket (%s)", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (;;) {
		cfd = accept(lfd, NULL, NULL);	/* Wait for connection */
		if (cfd == -1) {
			logMessage("Failure in accept(): %s", strerror(errno));
			exit(EXIT_FAILURE);
		}


		/* Handle each client request in a new child process */

		switch (fork()) {
			case -1:
				logMessage("Can't create child (%s)", strerror(errno));
				close(cfd);
				logClose();
				break;

			case 0: /* Child */
				close(lfd);		/* Unneed copy of listening client */
				close(filedes[0]);
				handleRequest(cfd);
				close(filedes[1]);
				close(cfd);
				_exit(EXIT_SUCCESS);

			default: /* Parent */
				close(cfd);
				break;
		}

		numRead = read(filedes[0], &latestParkingMsg, sizeof(struct parking_msg));
		if (numRead == -1)
			logMessage("read numRead == -1 (%s)", strerror(errno));
		if (numRead == 0)
			logMessage("read numRead ==  0 (%s)", strerror(errno));
		logMessage("%c&%c", (latestParkingMsg.parkingSpace[0] == '1' ? '1': '0'), (latestParkingMsg.parkingSpace[1] == '1' ? '1' :'0'));
	}
}
