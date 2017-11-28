import time
import serial
import requests
import json
import datetime
import sys

# Change the port name to match the port
# to which your Arduino is connected.
serial_port_name = 'COM4' # for Windows
ser = serial.Serial(serial_port_name, 9600, timeout=1)

delay = 1 * 5  # Delay in seconds

base = 'http://127.0.0.1:5000'
network_id = 'local'
header = {}

# Run once at the start
def setup():
    try:
        print("Setup")
    except:
        print("Setup Error")


# Run continuously forever
def loop():
    #Check if something is in serial buffer
    if ser.inWaiting() > 0:
        print("\nLoop:")
        try:
            print("Read file")

        except:
            print("--Error:", sys.exc_info()[0])
    # 100 ms delay
    time.sleep(0.1)
    return


# Run continuously forever
# with a delay between calls
def delayed_loop():
    global prev_bright
    print "\nDelayed Loop:"
    try:
        print("try")
    except:
        print("--Error:", sys.exc_info()[0])

# Run once at the end
def close():
    try:
        print("Closing file")
    except:
        print("Close Error")


# Program Structure
def main():
    # Call setup function
    setup()
    # Set start time
    nextLoop = time.time()
    while (True):
        # Try loop() and delayed_loop()
        try:
            loop()
            if time.time() > nextLoop:
                # If next loop time has passed...
                nextLoop = time.time() + delay
                delayed_loop()
        except KeyboardInterrupt:
            # If user enters "Ctrl + C", break while loop
            break
        except:
            # Catch all errors
            print("Unexpected error.")
    # Call close function
    close()


# Run the program
main()
