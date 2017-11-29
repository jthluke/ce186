"""
	Post trip: send sensor data and user feedback data to central server
"""
import csv
import datetime
from datetime import date
import serial
import requests
import time
import json
import sys
import numpy as np

class trip_details(object):
	"""
		An server object with details of streams and end points
	"""
	def __init__(self):
		self.header = {'Content-Type': 'application/json'}
		self.base = "http://127.0.0.1:5000"
		self.object_id = "userTripData"
		self.stream_id_dict = {"e": "elevation", "g": "gps", 
								"s": "speed", "p": "proximity", "l": "light",
								"r": "rating", "c": "comments", "n": "nightlight",
								"w": "warninglight"}
	
	def get_endpoint(self, stream_id):
		return "/networks/local/objects/"+self.object_id+"/streams/"+stream_id+"/points"

def send_2server(point_value, stream_shorthand, at=None, trip_obj=None):
	"""
		Send data to server
		Attribute:
			type = "elevation" / "gps" / "speed" / "proximity" / "rating" / "comments"
	"""
	if trip_obj is None:
		trip_obj = trip_details()
		
	existType = stream_shorthand in trip_obj.stream_id_dict.keys()
	if not existType:
		print("Please check stream short hand and re-specify ")

	if at is None:
		at = datetime.datetime.utcnow().isoformat() + 'Z'

	stream_id = trip_obj.stream_id_dict[stream_shorthand]
	end_point = trip_obj.get_endpoint(stream_id)
	url = trip_obj.base + end_point
	payload = {"points-value": point_value, "points-at": at}
	r = requests.request('post', url, headers=trip_obj.header, params=payload)
	# print(r)

def main():
	trip_obj = trip_details()
	# --test--
	# test_array = np.linspace(1,2)
	# for data in test_array:
	# 	send_2server(data, "g", trip_obj=trip_obj)
	#GPS
	# =============================================================================
	# start = datetime.datetime.now()
	# end = datetime.datetime(2017, 11, 27, start.hour, 59, 59)
	# step = datetime.timedelta(seconds=30)
	# time_list = []
	# while start < end:
	# 	time_list.append(start.isoformat()+"Z")
	# 	print(start.isoformat()+"Z")
	# 	start += step
	# date_length = len(time_list)
	# proximity = np.random.rand(date_length)+5
	# for idx, val in enumerate(proximity):
	# 	send_2server(val, "g", at=time_list[idx], trip_obj=trip_obj)
#      
	# =============================================================================
	#Velocity
	# =============================================================================
	# 	speed_list = np.random.rand(date_length)*100
	# 	for idx, val in enumerate(speed_list):
	# 		send_2server(val, "s", at=time_list[idx], trip_obj=trip_obj)
	#         
	# =============================================================================
	#Elevation
	# =============================================================================
	# 	ele_list = np.random.rand(date_length)*20
	# 	for idx, val in enumerate(ele_list):
	# 		send_2server(val, "e", at=time_list[idx], trip_obj=trip_obj)        
	#         
	# =============================================================================

	#put path to SD csv HERE 
	SDcsv = '/Users/apple/Desktop/CE186/project/git_project/ce186/server_db/testSD.csv'
   #--send gps--
	# with open(SDcsv) as csvfile:
	# 	reader = csv.DictReader(csvfile)
	# 	# f = open("/Users/apple/Desktop/CE186/project/git_project/ce186/server_db/wallflower/static/test_gps.txt", "wb")
	# 	# f.write("[")
	# 	for idx, row in enumerate(reader):
	# 		time = datetime.datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	# 		lat = (row['lat'])
	# 		lon = (row['lon'])
	# 		val = "{0}, {1}".format(lat, lon)
	# 		# # write data to txt
	# 		# val4list = [float(lat), float(lon)]
	# 		# f.write("{},".format(val4list))
	# 	# f.write("]")
	# 		send_2server(val, "g", at=time, trip_obj=trip_obj)                                                                                             
        
	# #--send proximity--
	# with open(SDcsv) as csvfile:
	# 	reader = csv.DictReader(csvfile)
	# 	for row in reader:
	# 		time = datetime.datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	# 		val = float(row['prox']) #cm   
	# 		send_2server(val, "p", at=time, trip_obj=trip_obj)

	# #--send velocity--
	# with open(SDcsv) as csvfile:
	# 	reader = csv.DictReader(csvfile)    
	# 	for row in reader:
	# 		time = datetime.datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	# 		val = float(row['vel'])*1.15078 #convert knots to mph
	# 		send_2server(val, "s", at=time, trip_obj=trip_obj)

	# #--send elevation--
	# with open(SDcsv) as csvfile:
	# 	reader = csv.DictReader(csvfile)
	# 	for row in reader:
	# 		time = datetime.datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	# 		val = float(row['elev'])
	# 		send_2server(val, "e", at=time, trip_obj=trip_obj) 

	# #--send night light--#
	# """
	# Night light condictions check for the on and off status of the trip
	# """
	# with open(SDcsv) as csvfile:
	# 	reader = csv.DictReader(csvfile)
	# 	for row in reader:
	# 		time = datetime.datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	# 		val =int(row['nightlight'])
	# 		send_2server(val, "n", at=time, trip_obj=trip_obj)

	#--send warning light--#
	"""
	Warning light condictions check for the on and off status of the trip
	"""
	with open(SDcsv) as csvfile:
	    reader = csv.DictReader(csvfile)
	    f = open("/Users/apple/Desktop/CE186/project/git_project/ce186/server_db/wallflower/static/test_warning.txt", "wb")
	    for row in reader:
	        time = datetime.datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	        val =int(row['warninglight'])
	        send_2server(val, "w", at=time, trip_obj=trip_obj)
	        # write data to txt
	        f.write("{},".format(val))

main()

