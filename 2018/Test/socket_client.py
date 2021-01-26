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
    #host = '192.168.1.17'
    port = 5555
    print("host " + host + " : " + str(port))
    client.connect((host, port))
    # sendMsgToServer("STA\n")
    # time.sleep(sleepTime)


#    for i in range(10):
#        sendMsgToServer('RGB01100000000000\n')
#        time.sleep(sleepTime)
#        sendMsgToServer('RGB01000000000200\n')
#        time.sleep(sleepTime)
#        sendMsgToServer('RGB01000000000000\n')

    for i in range(5):
        msg = "RGB%02d" % i
        sendMsgToServer(msg + "100000000000\n")
        time.sleep(sleepTime)
        sendMsgToServer(msg + "000000000000\n")

    for i in range(0):
        msg = "RGB%02d" % i
        sendMsgToServer(msg + "000000000000\n")

    sendMsgToServer("\n")
    sendMsgToServer("DISCONNECT\n")
    time.sleep(sleepTime)

    print('Done')


if __name__ == '__main__':
    main()
