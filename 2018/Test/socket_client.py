import socket
import time

client = socket.socket
sleepTime=2

def sendMsgToServer(msg):
    global client
    client.send(msg.encode('ascii'))


def main():
    global client
    client = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()

    port = 5556
    print("host" + host + " : " + str(port))
    client.connect((host, port))
    sendMsgToServer("LIST")
    time.sleep(sleepTime)

    sendMsgToServer("MODE 0")
    time.sleep(sleepTime)
    sendMsgToServer("COLOR 255 0 0")
    time.sleep(sleepTime)
    sendMsgToServer("COLOR 0 255 0")
    time.sleep(sleepTime)
    sendMsgToServer("COLOR 0 0 255")
    time.sleep(sleepTime)

    sendMsgToServer("MODE 1")
    time.sleep(sleepTime)
    sendMsgToServer("COLOR 255 0 0")
    time.sleep(sleepTime)
    sendMsgToServer("COLOR 0 255 0")
    time.sleep(sleepTime)
    sendMsgToServer("COLOR 0 0 255")
    time.sleep(sleepTime)

    sendMsgToServer("DISCONNECT")
    time.sleep(sleepTime)

    print('Done')

if __name__ == '__main__':
    main()
