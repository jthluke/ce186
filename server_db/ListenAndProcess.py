"""
    Simple program structure
    
"""
import datetime
import serial
import requests
import time
import json
import sys

# serial_port_name = '/dev/cu.usbserial-DN01J2G2' # for Mac
# ser = serial.Serial(serial_port_name, 9600, timeout=1)

delay = 3 # Delay in seconds

# Run once at the start
def setup():
    try:
        print "Setup"
    except:
        print "Setup Error"

# Run continuously forever
def loop(prevAve):
    # 100 ms delay
    try:
        time.sleep(delay)
        print(prevAve)
        prevAve = listen(prevAve)
    except:
        print "Unexpected error:", sys.exc_info()[0]
    return prevAve

# Run continuously forever
# with a delay between calls
def delayed_loop():
    print "...................."

# Run once at the end
def close():
    try:
        print "Close"
        ser.close()
    except:
        print "Close Error"
    
def send_2server(status):
    """
    status - string, either "a" or "b" indicating if data has any change
    """
    print("Sending status check..")
    header = {'Content-Type':'application/json'} #Specify header

    object_id = "from-py"; #Specify object id
    checkStatus_stream_id = "checkStatus"; #Specify stream id

    base = "http://127.0.0.1:5000"
    checkStatus_endpoint = "/networks/local/objects/"+object_id+"/streams/"+checkStatus_stream_id+"/points"

    checkStatus_stream_id_payload = {"points-value": status}

    checkStatus_r = requests.request('post', base + checkStatus_endpoint, headers = header, 
                                                    params=checkStatus_stream_id_payload)

def listen(prevAve):
    print("Listening to API..")
    header = {'Content-Type':'application/json'} #Specify header
    object_id = "from-py"; #Specify object id
    temperature_stream_id = "temp"; #Specify stream id
    base = "http://127.0.0.1:5000"
    temp_endpoint = "/networks/local/objects/"+object_id+"/streams/"+temperature_stream_id
    listenrequest = requests.request('get', base + temp_endpoint)
    content = json.loads(listenrequest.text)
    curLen = len(content["points"])
    curAve = 0
    summ = 0
    for i, item in enumerate(content["points"]):
        summ += item["value"]
    curAve = float(summ/curLen)
    print("Current temperature average is: "+str(curAve))
    if curAve != prevAve:
        print("Average Temperature has changed")
        send_2server("b")
    else:
        print("Average Temperature is still the same")
        send_2server("a")
    return curAve



# Program Structure    
def main(prevAve):
    # Call setup function
    setup()
    # Set start time
    # nextLoop = time.time()
    # while(True):
        # Try loop() and delayed_loop()
    try:
        prevAve = loop(prevAve)
            # if time.time() > nextLoop:
                # If next loop time has passed...
                # nextLoop = time.time() + delay
            # else:
            #     delayed_loop()
    # except KeyboardInterrupt:
    #     break
    except:
        print "Unexpected error."
    # Call close function
    # ser.close()
    return prevAve

# Run the program
# main()
