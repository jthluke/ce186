"""
	Post trip: send sensor data and user feedback data to central server
"""
import datetime
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
								"s": "speed", "p": "proximity", 
								"r": "rating", "c": "comments"}
	
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
	#--test--
	# test_array = np.linspace(1,10)
	# for data in test_array:
	# 	send_2server(data, "p", trip_obj=trip_obj)

	#--send proximity--
	start = datetime.datetime.now()
	end = datetime.datetime(2017, 11, 20, start.hour+1, 59, 59)
	step = datetime.timedelta(seconds=30)
	time_list = []
	while start < end:
		time_list.append(start.isoformat()+"Z")
		start += step
	date_length = len(time_list)
	proximity = np.random.rand(date_length)+5
	for idx, val in enumerate(proximity):
		send_2server(val, "p", at=time_list[idx], trip_obj=trip_obj)
	
	#--send velocity--
	speed_list = np.random.rand(date_length)*100
	for idx, val in enumerate(speed_list):
		send_2server(val, "s", at=time_list[idx], trip_obj=trip_obj)

	#--send elevation--
	ele_list = np.random.rand(date_length)*20
	for idx, val in enumerate(ele_list):
		send_2server(val, "e", at=time_list[idx], trip_obj=trip_obj)

main()

