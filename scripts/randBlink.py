import socket
import time
import random

client = socket.socket
sleepTime = 0.1
leds = [1, 2, 4, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 23, 25, 29, 30, 31, 32, 33, 36, 37, 38, 39, 40]
station = [1, 2]
stins = [4]
lastbil = [6]
bod1 = [8, 9]
bod2 = [10, 11]
bod3 = [12, 13, 14]
bod4 = [15, 16]
bodar = bod1 + bod2 + bod3 + bod4
hus1 = [18]
hus2 = [20]
hus3 = [22]
hus4 = [23]
hus5 = [25]
hus = hus1 + hus2 + hus3 + hus4 + hus5
high1 = range(29,33)
high2 = range(36,40)
high = high1 + high2


colors = ["100000000000",
          "100100000000",
          "000100000000",
          "000100100000",
          "000000100000",
          "100000100000",
          "100000100000",
          "000000000100"]
def sendMsgToServer(msg):
    global client
    client.send(msg.encode('ascii'))


def main():
    global client
    client = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()
    host = '192.168.1.17'
    port = 5555
    print("host " + host + " : " + str(port))
    client.connect((host, port))
    # sendMsgToServer("STA\n")
    # time.sleep(sleepTime)

    for i in leds:
        msg = "RGB%02d" % i
        sendMsgToServer(msg + "000000000100\n")

    time.sleep(3)
    for i in range(1000):
        r = int(round(random.random() * (len(leds)-1)))
        msg = "RGB%02d" % leds[r]
        c = int(round(random.random() * (len(colors)-1)))
        sendMsgToServer(msg + colors[c] + "\n")
        if (i%50 == 0):
            print("Now: " + str(i))
        time.sleep(sleepTime)

    for i in range(0):
        msg = "RGB%02d" % i
        sendMsgToServer(msg + "000000000000\n")

    sendMsgToServer("\n")
    sendMsgToServer("DISCONNECT\n")
    time.sleep(sleepTime)

    print('Done')


if __name__ == '__main__':
    main()
