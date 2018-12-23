import socket
import datetime
import serial.tools.list_ports

DEBUG = True;
SER_BAUDRATE = 9600
controllers = list()


def debug_print(msg):
    if DEBUG is True:
        print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f: ") + msg)


def scan_serial_ports():
    serial_ports = serial.tools.list_ports.comports()
    for p in serial_ports:
        try:
            # configure the serial connections (the parameters differs on the device you are connecting to)
            ser = serial.Serial(p.device, SER_BAUDRATE, timeout=1)
            print(p.device)
            ser.write("STA\n")
            ser.flush()
            msg = ser.readline()
            debug_print(msg)
            controllers.append(p.device)
        except (OSError, serial.SerialException):
            debug_print("Failed to connect to :" + p.device)


def send_to_ctrl(msg):
    debug_print("Send to Ctrl: " + msg)


def main():
    server_socket = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()
    port = 5555

    server_socket.bind((host, port))

    server_socket.listen(5)

    scan_serial_ports()

    while True:
        debug_print('Waiting for connection')
        clientsocket, addr = server_socket.accept()

        debug_print('Got a connection from {}'.format(str(addr)))

        msg = "CONNECTEDn"
        clientsocket.send(msg.encode('ascii'))

        while not "DISCONNECT" in msg:
            msg = clientsocket.recv(1024).decode('ascii')
            debug_print("Received: " + msg)
            send_to_ctrl(msg)

        msg = "Closing connection\n"
        clientsocket.send(msg.encode('ascii'))

        print('Disconnecting client')
        clientsocket.close()


if __name__ == '__main__':
    main()