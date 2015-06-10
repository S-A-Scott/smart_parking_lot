#include <termios.h>
#include <fcntl.h>
#include "spcsot.h"

#define UART /dev/ttyUSB0

static void
set_attr(int fd)
{
	struct termios tp;

	if (tcgetattr(fd, &tp) == -1)
		errExit("tcgetattr");

	if (cfsetispeed(&tp, B9600) == -1)
		errExit("cfsetispeed");

	tp.c_cflag = CS8 | CREAD;
	tp.c_cflag &= ~PARENB;
	tp.c_cflag &= ~CSTOPB;
	tp.c_cc[VMIN] = 1;
	tp.c_cc[VTIME] = 0;
	tcflush(fd, TCIOFLUSH);


	if (tcsetattr(fd, TCSANOW, &tp) == -1)
		errExit("tcsetattr");
}

int
main(int argc, char *argv[])
{
	int fd;
	char buf[1024];
	int numRead;
	if ((fd = open("/dev/ttyUSB0", O_RDONLY)) == -1)
		errExit("open");

	set_attr(fd);

	while (1) {
		if ((numRead = read(fd, buf, 1024)) > 0)
			write(STDOUT_FILENO, buf, numRead);

		sleep(1);
	}

}
