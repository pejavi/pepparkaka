import socket
import time
from threading import Thread

def client_socket(clientsocket, addr, pause=5):
    print('Got a connection from {}'.format(str(addr)))

    msg = "Thank you for connecting\n"
    clientsocket.send(msg.encode('ascii'))

    msg = "Taking a break for " + str(pause) + " seconds\n"
    clientsocket.send(msg.encode('ascii'))

    time.sleep(pause)

    msg = "Bye\n"
    clientsocket.send(msg.encode('ascii'))

    print('Disconnecting client')
    clientsocket.close()


def main():
    serversocket = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()
    port = 5555

    serversocket.bind((host, port))

    serversocket.listen(5)

    while True:
        print('Waiting for connection')
        clientsocket, addr = serversocket.accept()
        Thread(target=client_socket, args=(clientsocket, addr, 10)).start()


    
if __name__ == '__main__':
    main()