ROOT=..
include $(ROOT)/PLATFORM
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS = com_read

all: $(PROGS)

$(PROGS): $(LIBSPCSOT) 
	$(CC) $(CFLAGS)  $@.c -o $@ $(LDLIBS)

clean:
	rm -f *.o
	rm -f *.o $(PROGS)
