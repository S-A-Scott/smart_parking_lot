#include "parking_lot.h"
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



int
main(int argc, char *argv[])
{
	int sfd;
	int serverFd, dummyFd;
	struct parking_msg parkingMsg;

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usageErr("%s: host msg...\n", argv[0]);



	/* Create well-known FIFO, and open it for reading */

	umask(0);	/* So we get the permissions we want */
	if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1
			&& errno != EEXIST)
		errExit("mkfifo %s", SERVER_FIFO);
	serverFd = open(SERVER_FIFO, O_RDONLY);
	if (serverFd == -1)
		errExit("open %s", SERVER_FIFO);

	/* Open an extra write descriptor, so that we nenver see EOF */
	if ((dummyFd = open(SERVER_FIFO, O_WRONLY)) == -1)
		errExit("open %s", SERVER_FIFO);

	for (;;) {
		if (read(serverFd, &parkingMsg, sizeof(struct parking_msg))
				!= sizeof(struct parking_msg)) {
			fprintf(stderr, "Error reading\n");
			continue;
		}
		
		/* Construct server address from first command-line argument */
		sfd = inetConnect(argv[1], PORT_NUM, SOCK_STREAM);
		if (sfd == -1)
			fatal("Could not connect to server socket");
		if (write(sfd, &parkingMsg, sizeof(struct parking_msg)) < 0)
			fatal("partial/failed write");
		close(sfd);
	}

	exit(EXIT_SUCCESS);
}
