#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#define BD_9600    01
#define BD_115200  02

#define STOPB_1    010 /* one stop bit */
#define STOPB_2    020 /* two stop bits */


void startSerial(int fd, int flags);

#endif
