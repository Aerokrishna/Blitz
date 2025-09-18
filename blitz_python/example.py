from interfaces import blitz_interfaces
from blitz_serial import SerialBlitz
import time

send = True

blitz = SerialBlitz()

while True:

    blitz.blitz_read()

    if send:
        blitz_interfaces["counter"].data = [2, 3, 3.4, 5.1]
        blitz.blitz_write(id=blitz_interfaces["counter"].id)

        time.sleep(0.1)

    # else:

    print("DATA :: ", blitz_interfaces["counter_response"].data)
