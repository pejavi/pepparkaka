import socket
import time

def main():
    serversocket = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()
    port = 5556

    serversocket.bind((host, port))

    serversocket.listen(5)

    while True:
        print('Waiting for connection')
        clientsocket, addr = serversocket.accept()

        print('Got a connection from {}'.format(str(addr)))

        msg = "CONNECTEDn"
        clientsocket.send(msg.encode('ascii'))

        while not "DISCONNECT" in msg:
            msg = clientsocket.recv(1024).decode('ascii')
            print(msg)

        msg = "Bye\n"
        clientsocket.send(msg.encode('ascii'))

        print('Disconnecting client')
        clientsocket.close()

    
if __name__ == '__main__':
    main()