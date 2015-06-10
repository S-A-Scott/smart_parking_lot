#include <termios.h>
#include "spcsot.h"
#include "serial_communication.h"

void
startSerial(int fd, int flags)
{
	struct termios tp;

	if (tcgetattr(fd, &tp) == -1)
		errExit("tcgetattr");

	if (cfsetispeed(&tp, (flags & BD_9600) ? B9600 :
				(flags & BD_115200) ? B115200 : B9600)
			== -1)
		errExit("cfsetispeed");

	tp.c_cflag = CS8 | CREAD;
	tp.c_cflag &= ~PARENB;
	if ((flags & STOPB_1))
		tp.c_cflag &= ~CSTOPB;
	else
		tp.c_cflag |= CSTOPB;
	tp.c_cc[VMIN] = 1;
	tp.c_cc[VTIME] = 0;
	tcflush(fd, TCIOFLUSH);

	if (tcsetattr(fd, TCSANOW, &tp) == -1)
		errExit("tcsetattr");
}
