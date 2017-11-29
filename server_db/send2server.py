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
import pandas as pd

class trip_details(object):
	"""
		An server object with details of streams and end points
	"""
	def __init__(self):
		self.header = {'Content-Type': 'application/json'}
		self.base = "http://127.0.0.1:5000"
		self.object_id = "userTripData"
		self.stream_id_dict = {"e": "elevation", "g": "gps", 
								"s": "speed", "p": "proximity", "n": "nightlight",
								"r": "rating", "c": "comments",
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
	#put path to SD csv HERE 
	path = '/Users/apple/Desktop/CE186/project/git_project/ce186/server_db/GPSLOG00.csv'
	reader = pd.read_csv(path)
	reader = reader.replace('a', '', regex = True).dropna()
	reader.columns = ['time', 'lat', 'lon', 'vel', 'elev', 'warning', 'nightlight' ]
	reader = reader[reader.iloc[:,0].str.contains('(2017)', regex = True)]

	# f = open("/Users/apple/Desktop/CE186/project/git_project/ce186/server_db/wallflower/static/test_gps.txt", "wb")
	# f.write("[")
	for i, row in reader.iterrows():
		time = datetime.datetime.strptime(row['time'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	#	#gps==============================================
	# 	lat = (row['lat'])
	# 	lon = (row['lon'])
	# 	val = "{0}, {1}".format(lat, lon)
		# val4list = [float(lat), float(lon)]
	# 	#write data to txt
	# 	f.write("{},".format(val4list))
		# send_2server(val, "g", at=time, trip_obj=trip_obj) 
	# 	#velocity=========================================
	# 	vel = float(row['vel']) 
	# 	send_2server(vel, "s", at=time, trip_obj=trip_obj)    
	# 	#proximity warning================================
	# 	warning = float(row['warning'])
	# 	send_2server(warning, "w", at=time, trip_obj=trip_obj)
	# 	#elevation============================================
	# 	elev = float(row['elev'])
	# 	send_2server(elev, "e", at=time, trip_obj=trip_obj)
	# f.write("]")

	#random proximity data
	start = datetime.datetime.now()
	end = datetime.datetime(2017, 11, 29, start.hour, 40, 59)
	step = datetime.timedelta(seconds=30)
	time_list = []
	while start < end:
		time_list.append(start.isoformat()+"Z")
		start += step
	date_length = len(time_list)
	proximity = np.random.rand(date_length)+5
	for idx, val in enumerate(proximity):
		send_2server(float(val), "p", at=time_list[idx], trip_obj=trip_obj)	
main()

