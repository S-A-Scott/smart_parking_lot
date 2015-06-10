ROOT=..
include $(ROOT)/PLATFORM
include $(ROOT)/Make.defines.$(PLATFORM)

PROGS = parking_lot_sv parking_lot_cl

all: $(PROGS)

$(PROGS): $(LIBSPCSOT) 
	$(CC) $(CFLAGS)  $@.c -o $@ $(LDLIBS)

clean:
	rm -f *.o
	rm -f *.o $(PROGS)
