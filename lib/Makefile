#
# Makfile for misc library
#
ROOT=..
include $(ROOT)/PLATFORM
include $(ROOT)/Make.defines.$(PLATFORM)

LIBMISC = libspcsot.a
OBJS = error_handle.o get_number.o log_message.o inet_sockets.o \
	   become_daemon.o serial_communication.o

all: $(LIBMISC)

$(LIBMISC): $(OBJS)
	$(AR) rv $(LIBMISC) $?

clean:
	rm -f *.o
	rm libspcsot.a
