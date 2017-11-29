# -*- coding: utf-8 -*-
"""
Created on Sun Nov 26 22:02:39 2017

@author: Vania Fong
"""

import csv
import datetime
from datetime import date
from datetime import datetime
import pandas as pd

#import os
#cwd = os.getcwd()
#os.chdir('C:\\Users\\Vania Fong\\Documents\\College Academics\\4th Year\\CE 186\\Final Project 186')


path = 'GPSLOG00.csv'
path = 'sampleSD.csv'
reader = pd.read_csv(path)
reader = reader.replace('a', '', regex = True).dropna()
reader.columns = ['time', 'lat', 'lon', 'vel', 'elev', 'warning', 'nightlight' ]
reader = reader[reader.iloc[:,0].str.contains('(2017)', regex = True)]


for i, row in reader.iterrows():
    time = datetime.strptime(reader.loc[i][0], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
    print time
    
# =============================================================================
# with open(path) as csvfile:
#     reader = csv.DictReader(csvfile, delimiter = ',')
#     for row in reader:
#         time = datetime.strptime(row['timestamp'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
#         lat = row['lat']
#         lon = row['lon']
#         vel = float(row['vel'])*1.15078 #convert knots to mph
#         ele = float(row['elev'])
#         warning = float(row['warning']) #cm 
#         nightlight =float(row['nightlight'])
#         
#         
# =============================================================================
        
f = open("test_gps.txt", "wb")
f.write("[")
for i, row in reader.iterrows():
	time = datetime.strptime(row['time'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
	lat = (row['lat'])
	lon = (row['lon'])
	val = "{0}, {1}".format(lat, lon)
	val4list = [float(lat), float(lon)]
	#write data to txt
	#f.write("{},".format(val4list))
f.write("]")
    
for i, row in reader.iterrows():
	time = datetime.strptime(row['time'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
    vel = float(row['vel']) 
	send_2server(vel, "s", at=time, trip_obj=trip_obj)    
    
	warning = boolean(row['warning'])
	send_2server(warning, "w", at=time, trip_obj=trip_obj)
    
	nightlight = boolean(row['nightlight'])
	send_2server(nightlight, "l", at=time, trip_obj=trip_obj)
    
	elev = float(row['elev'])
	send_2server(elev, "e", at=time, trip_obj=trip_obj)
    
    

for i, row in reader.iterrows():
    time = datetime.strptime(row['time'], '%H:%M:%S.%f %d/%m/%Y').isoformat() + 'Z'
    vel = float(row['vel']) 
    print vel 

#timestr = '6:3:41.0 27/11/2017'
#d = date.fromordinal(730920)
#datetime.strptime(timestr, '%H:%M:%S.%f %d/%m/%Y')
