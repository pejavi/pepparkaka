import serial
import time
com=serial.Serial('/dev/ttyUSB0')
print(com.name)
com.write('RGB01000000000150\n')
time.sleep(1)
com.write('RGB02000000000150\n')
time.sleep(1)
com.write('RGB04000000000100\n')
time.sleep(1)
com.write('RGB06000000000100\n')
time.sleep(1)
com.write('RGB08000000000100\n')
time.sleep(1)
com.write('RGB09100000000000\n')
time.sleep(1)
com.write('BLI09\n')
time.sleep(1)
com.write('RGB10050100050000\n')
#bod3
com.write('RGB12000000000100\n')
com.write('RGB13000000000100\n')
com.write('RGB14000000000100\n') 
#Bod4
com.write('RGB15000000000100\n') 
com.write('RGB16000000000100\n')
#Frisoren
com.write('RGB18000000000100\n')
#Stuga 
com.write('RGB20000000000100\n')
#Tomtestuga
com.write('RGB22000000000100\n')
#Skorsten
com.write('RGB23000000000100\n')
#Tomtestuga2 
com.write('RGB25000000000100\n')
#Forort1 29-33 
com.write('RGB29000000000100\n')
com.write('RGB31000000000100\n')
com.write('RGB33000000000100\n')
#Forort2 36-40
com.write('RGB37000000000100\n')
com.write('RGB39000000000100\n')
