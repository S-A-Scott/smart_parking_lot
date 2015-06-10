ps aux | grep 'parking_lot_sv'| grep -v 'grep' | awk '{print $2}' | sudo xargs kill -9
