import socket
import time
import random

client = socket.socket
sleepTime = 0.1
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
high1 = range(29, 33)
high2 = range(36, 40)
high = high1 + high2
all_leds = station + stins + lastbil + bodar + hus + high

red = "100000000000"
yellow = "100100000000"
green = "000100000000"
cyan = "000100100000"
blue = "000000100000"
magenta = "100000100000"
white = "000000000100"
black = "000000000000"
colors = [red,
          yellow,
          green,
          cyan,
          blue,
          magenta,
          white,
          black]


def send_msg_to_server(msg):
    global client
    client.send(msg.encode('ascii'))


def main():
    global client
    client = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = '192.168.1.17'
    port = 5555
    print("Connect to " + host + " : " + str(port))
    client.connect((host, port))

    # Turn all off
    for i in all_leds:
        msg = "RGB%02d" % i
        send_msg_to_server(msg + black + "\n")

    time.sleep(4)

    # Light train
    for c in range(2):
        for i in all_leds:
            msg = "RGB%02d" % i
            send_msg_to_server(msg + colors[c] + "\n")
            time.sleep(0.2)
            send_msg_to_server(msg + black + "\n")

    # All white
    for i in all_leds:
        msg = "RGB%02d" % i
        send_msg_to_server(msg + white + "\n")

    time.sleep(4)

    # Random lights in high
    for i in range(200):
        r = int(round(random.random() * (len(high)-1)))
        msg = "RGB%02d" % high[r]
        c = int(round(random.random() * (len(colors)-1)))
        send_msg_to_server(msg + colors[c] + "\n")
        if i % 50 == 0:
            print("High: " + str(i/2))
        time.sleep(sleepTime)

    for i in high:
        msg = "RGB%02d" % i
        send_msg_to_server(msg + white + "\n")

    # Random lights in station
    for i in range(50):
        r = int(round(random.random() * (len(station)-1)))
        msg = "RGB%02d" % station[r]
        c = int(round(random.random() * (len(colors)-1)))
        send_msg_to_server(msg + colors[c] + "\n")
        if i % 10 == 0:
            print("Station: " + str(i*2))
        time.sleep(sleepTime)

    for i in station:
        msg = "RGB%02d" % i
        send_msg_to_server(msg + white + "\n")

    time.sleep(2)

    # Random all lights
    for i in range(400):
        r = int(round(random.random() * (len(all_leds)-1)))
        msg = "RGB%02d" % all_leds[r]
        c = int(round(random.random() * (len(colors)-1)))
        send_msg_to_server(msg + colors[c] + "\n")
        if i % 50 == 0:
            print("Now: " + str(i/4))
        time.sleep(sleepTime)

    send_msg_to_server("\n")
    send_msg_to_server("DISCONNECT\n")
    time.sleep(sleepTime)

    print('Done')


if __name__ == '__main__':
    main()
