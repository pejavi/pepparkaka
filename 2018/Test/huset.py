from __future__ import print_function
import socket
import time
from threading import Thread

run = True
color = [1, 2, 3]
mode = 0
speed = 0.5


def ledController():
    global run
    global mode
    global color
    global speed
    strength = 0.0
    increase = True
    redLeds = [0]*10
    greenLeds = [0]*10
    blueLeds = [0]*10

    while run:
        # Shift in new color
        if mode == 0:
            redLeds = redLeds[1:] + [color[0]]
            greenLeds = greenLeds[1:] + [color[1]]
            blueLeds = blueLeds[1:] + [color[2]]

        # Pulse train
        elif mode == 1:
            strength += 0.1 if increase else -0.1
            increase = True if (strength < 0.9 and increase) or strength <= 0.1 else False
            redLeds = redLeds[1:] + [int(color[0] * strength)]
            greenLeds = greenLeds[1:] + [color[1]] + [int(color[1] * strength)]
            blueLeds = blueLeds[1:] + [color[2]] + [int(color[2] * strength)]

        # Pulse
        elif mode == 2:
            strength += 0.1 if increase else -0.1
            increase = True if (strength < 0.9 and increase) or strength <= 0.1 else False
            #            #print("Strength: " + str(strength) + "; Increase: " + str(increase))
            redLeds = [int(color[0] * strength)] * len(redLeds)
            greenLeds = [int(color[1] * strength)] * len(greenLeds)
            blueLeds = [int(color[2] * strength)] * len(blueLeds)

        # Default turned off
        else:
            redLeds = [5]*len(redLeds)
            greenLeds = [5]*len(greenLeds)
            blueLeds = [5]*len(blueLeds)

        print("LEDs: ", end=' ')
        for l in redLeds:
            print(str(l), end=' ')
        print("")
        time.sleep(speed)
    print("--- Finished running ---")


def main():
    global mode
    server_socket = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()
    port = 5556

    server_socket.bind((host, port))

    server_socket.listen(5)

    Thread(target=ledController).start()

    while True:
        print('Waiting for connection')
        clientsocket, addr = server_socket.accept()

        print('Got a connection from {}'.format(str(addr)))

        msg = "CONNECTEDn"
        clientsocket.send(msg.encode('ascii'))

        while "DISCONNECT" not in msg:
            msg = clientsocket.recv(1024).decode('ascii')
            print(msg)
            commands = msg.split()
            print(commands)
            if len(commands) < 1:
                continue
            if commands[0] == "MODE":
                mode = int(commands[1])
                print("New mode: " + str(mode))
            if commands[0] == "COLOR":
                color[0] = int(commands[1])
                color[1] = int(commands[2])
                color[2] = int(commands[3])
                print("New color: R:" + str(color[0]) + " G:" + str(color[1]) + " B:" + str(color[2]))

        msg = "Bye\n"
        clientsocket.send(msg.encode('ascii'))

        print('Disconnecting client')
        clientsocket.close()

    
if __name__ == '__main__':
    main()
