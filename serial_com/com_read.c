#include "../lib/spcsot.h"
#include "../socket/parking_lot.h"
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>

#define UART /dev/ttyUSB0

static void
set_attr(int fd, int databits, int stopbits, int parity)
{
	struct termios tp;

	if (tcgetattr(fd, &tp) == -1)
		errExit("tcgetattr");
	tp.c_cflag &= ~CSIZE;

	if (cfsetispeed(&tp, B9600) == -1)
		errExit("cfsetispeed");

	tp.c_cflag |= (CLOCAL | CREAD);
    tp.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tp.c_oflag &= ~OPOST;
    tp.c_oflag &= ~(ONLCR | OCRNL);   
    tp.c_iflag &= ~(ICRNL | INLCR);
    tp.c_iflag &= ~(IXON | IXOFF | IXANY);
	
	switch (databits) {
	case 7:
		tp.c_cflag |= CS7;
		break;
	case 8:
		tp.c_cflag |= CS8;
		break;
	default:
		errExit("switch databits");
	}

	switch (parity) {
	case 'n':
	case 'N':
		tp.c_cflag &= ~PARENB;		/* clear parity enable */
		tp.c_iflag &= ~INPCK;		/* disable  parity checking */
		break;

	case 'o':
	case 'O':
		tp.c_cflag |= (PARODD | PARENB);
		tp.c_iflag |= INPCK;
		break;

	case 'e':
	case 'E':
		tp.c_cflag |= PARENB;
		tp.c_cflag &= ~PARODD;
		tp.c_iflag |= INPCK;
		break;

	case 's':
	case 'S':
		tp.c_cflag &= ~PARENB;
		tp.c_cflag &= ~CSTOPB;
		break;
	
	default:
		errExit("switch parity");
	}
	
	switch (stopbits) {
	case 1:
		tp.c_cflag &= ~CSTOPB;
		break;
	
	case 2:
		tp.c_cflag |= CSTOPB;
		break;

	default:
		errExit("switch stopbits");
	}


	tcflush(fd, TCIOFLUSH);
	tp.c_cc[VTIME] = 15;
	tp.c_cc[VMIN] = 0;

	if (tcsetattr(fd, TCSANOW, &tp) != 0)
		errExit("tcsetattr");
}

int
main(int argc, char *argv[])
{
	char buf[2];
	int numRead;
	int serverFd;
	struct parking_msg parkingMsg;
	int fd;
	if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
		errExit("setvbuf");

	if ((fd = open(argv[1], O_RDWR)) < 0)
		errExit("open");
	set_attr(fd, 8, 1, 'N');

//	while (1) {
//		if ((numRead = read(fd, buf, 1024)) > 0)
//			write(STDOUT_FILENO, buf, numRead);
//	}


	bzero(&parkingMsg, sizeof(struct parking_msg));
	parkingMsg.length = 2;
	while (1) {
		if ((numRead = read(fd, buf, 2)) > 0) {
			char data = buf[0];
			int position = ((data - 48) % 2) ? (data - 48) / 2 : (data - 48) /2 - 1;
			char has_car = ((data - 48) % 2) ? '1' : '0';
			parkingMsg.parkingSpace[position] = has_car;
			printf("data: %c, position: %d, has_car: %c\n", data, position, has_car);

			if ((serverFd = open(SERVER_FIFO, O_WRONLY)) == -1)
				errExit("open %s", SERVER_FIFO);
			if (write(serverFd, &parkingMsg, sizeof(struct parking_msg)) !=
					sizeof(struct parking_msg))
				fatal("Can't write to server");
			close(serverFd);
		}
	}
}


