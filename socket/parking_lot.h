#include "spcsot.h"
#include "inet_sockets.h"

#define SERVER_FIFO "/tmp/sv_fifo"
#define PORT_NUM "12345"

struct parking_msg {
	int  length;				/* number of parking space */
	char parkingSpace[1024];	/* indicate whether the parking
								   space is being used.
								   1 ---> used
								   0 ---> unused */
};
