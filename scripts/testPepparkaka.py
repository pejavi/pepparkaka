#!/usr/bin/python           # This is client.py file

import socket               # Import socket module
import time
import sys
from random import randint

s = socket.socket()         # Create a socket object
host = '192.168.1.17'         # Get local machine name
port = 5555                 # Reserve a port for your service.
id = '12345'
def toggle(pin):
  for x in range(1,7):
    switch(pin)
    time.sleep(1.2)
	
def switch(pin):
    msg = "RELAY;" + id + ";LED%02d\n" % pin
    print " Switch LED " + str(pin) + '\n' + "  " + msg
    s.send(msg)


def movie():
 for y in range(1,100):
  randX = randint(20,30)
  randZ = randint(2,9)
  for x in range (1,randX):
    switch(8)
    #switch(12)
    time.sleep(1)
  for z in range (1,randZ):
    switch(8)
    switch(11)
    time.sleep(1)
	
s.connect((host, port))
s.send("LOGIN;" + id + ";PASSWORD\n")
print s.recv(1024)

if (len(sys.argv) == 2):
  if (sys.argv[1] == "m"):
    movie()
  else:
    toggle(int(sys.argv[1]))
elif (len(sys.argv) == 3):
  for pin in range(int(sys.argv[1]),int(sys.argv[2])+1):
    print "Testing PIN " + str(pin)
    time.sleep(3)
    toggle(pin)
else:
  for pin in range(1,34):
    print "Testing PIN " + str(pin)
    time.sleep(3)
    toggle(pin)
    #print s.recv(1024)

  
print "Logging out"
s.send("LOGOUT;" + id + "\n")
print s.recv(1024)
print "Close connection"
s.close                     # Close the socket when done