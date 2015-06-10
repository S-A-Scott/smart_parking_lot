#!/bin/sh

if [ "$1" = "client" ]
then
	./socket/parking_lot_cl $2 &
	./serial_com/com_read /dev/ttyUSB0
elif [ "$1" = "server" ]
then
	./socket/parking_lot_sv
fi
