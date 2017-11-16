"""
    Simple program structure
    
"""
import datetime
import serial
import requests
import time
import json
import sys
import ListenAndProcess as LAP

serial_port_name = '/dev/cu.usbserial-DN01J2G2' # for Mac
ser = serial.Serial(serial_port_name, 9600, timeout=1)

delay = 3 # Delay in seconds

# Run once at the start
def setup():
    try:
        print "Setup"
    except:
        print "Setup Error"

# Run continuously forever
def loop():
    time.sleep(delay)
    if ser.inWaiting() > 0:
        try:
            # Read entire line
            # (until '\n')
            temperature = float(ser.readline())
            print "Received temperature:", temperature
            # print "Type:", type(temperature)
            voltage = float(ser.readline())
            print "Received photoresistor voltage:", voltage
            # print "Type:", type(voltage)
            send_2server(temperature, voltage)
            listen()
        except:
            print "Unexpected error:", sys.exc_info()[0]
    return

# Run continuously forever
# with a delay between calls
def delayed_loop():
    print "Delayed Loop"

# Run once at the end
def close():
    try:
        print "Close"
        ser.close()
    except:
        print "Close Error"
    
def send_2server(temperature, voltage):
    print("Sending temperature and voltage..")
    header = {'Content-Type':'application/json'} #Specify header

    object_id = "from-py"; #Specify object id
    temperature_stream_id = "temp"; #Specify stream id
    photoresistor_stream_id = "volt"; #Specify stream id

    base = "http://127.0.0.1:5000"
    temp_endpoint = "/networks/local/objects/"+object_id+"/streams/"+temperature_stream_id+"/points"
    volt_endpoint = "/networks/local/objects/"+object_id+"/streams/"+photoresistor_stream_id+"/points"

    temp_payload = {"points-value": temperature}
    volt_payload = {"points-value": voltage}

    temp_r = requests.request('post', base + temp_endpoint, headers = header, params=temp_payload)
    volt_r = requests.request('post', base + volt_endpoint, headers = header, params=volt_payload)
    
def send_2ar(data):
    #if streams have not been updated, then set the arduino LED to off
    #if streams have been updated, power on the arduino LED
    try:
        ser.write(str(data))
    except:
        print "Send2ar --- Unexpected error:", sys.exc_info()[0]

def listen():
    print("Listening to API..")
    header = {'Content-Type':'application/json'} #Specify header
    object_id = "from-py"; #Specify object id
    checkStatus_stream_id = "checkStatus"; #Specify stream id
    base = "http://127.0.0.1:5000"
    temp_endpoint = "/networks/local/objects/"+object_id+"/streams/"+checkStatus_stream_id
    try:
        listenrequest = requests.request('get', base + temp_endpoint)
        content = json.loads(listenrequest.text)
        result = content["points"][0]["value"]
    except:
        print "linsten --- Unexpected error:", sys.exc_info()[0]
    print("Value returned from server: "+result)
    send_2ar(result)


# Program Structure    
def main():
    # Call setup function
    setup()
    # Set start time
    nextLoop = time.time()
    prevAve = 0
    while(True):
        # Try loop() and delayed_loop()
        try:
            prevAve = LAP.main(prevAve)
            # if time.time() > nextLoop:
            loop()
                # If next loop time has passed...
                # nextLoop = time.time() + delay
            # else:
            #     delayed_loop()
        except KeyboardInterrupt:
            break
        except:
            print "The Program Has Failed You."
    # Call close function
    close()

# Run the program
main()
