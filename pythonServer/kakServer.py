import time
import socket
import datetime
import serial.tools.list_ports

DEBUG = True;
SER_BAUDRATE = 9600
controllers = list()
ser = serial

class Controller:
    def __init__(self, device):
        try:
            self.ser = serial.Serial(device)
        except (OSError, serial.SerialException):
            debug_print("Failed to connect to :" + device)

    def __del__(self):
        self.ser.close()

    def write(self,msg):
        self.ser.write(msg)
        self.ser.flush()


def debug_print(msg):
    if DEBUG is True:
        print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f: ") + msg)


def scan_serial_ports():
    serial_ports = serial.tools.list_ports.comports()
    for p in serial_ports:
        try:
            # configure the serial connections (the parameters differs on the device you are connecting to)
            ser = serial.Serial('COM6') #p.device, timeout=1)
            time.sleep(1)
            print(p.device)
            for i in range(2):
                ser.write('ENA03000\n')
                ser.flush()
                time.sleep(0.5)
                ser.write('ENA03001\n')
                ser.flush()
                time.sleep(0.5)
            #msg = ser.readline()
            #debug_print(msg)
            ser.close()
            controllers.append(p.device)
        except (OSError, serial.SerialException):
            debug_print("Failed to connect to :" + p.device)


def send_to_ctrl(msg):
    debug_print("Send to Ctrl: " + msg.strip())
    ser.write(bytes(msg))
    ser.flush()
    debug_print('Sent: ' + msg.encode('ascii').strip())


def main():
    global ser

    server_socket = socket.socket(
      socket.AF_INET, socket.SOCK_STREAM)

    host = socket.gethostname()
    port = 5555

    server_socket.bind((host, port))

    server_socket.listen(5)

    # scan_serial_ports()

    while True:
        debug_print('Waiting for connection')
        clientsocket, addr = server_socket.accept()

        debug_print('Got a connection from {}'.format(str(addr)))

        msg = "CONNECTED\n"
        clientsocket.send(msg.encode('ascii'))

        try:
            ser = serial.Serial('COM6', timeout=1)
            while not "DISCONNECT" in msg:
                msg = clientsocket.recv(1024).decode('ascii')
                debug_print("Socket received: " + msg.strip())
                send_to_ctrl(msg)
        except (OSError, serial.SerialException):
            debug_print("Failed to comunicate")


        msg = "Closing connection\n"
        clientsocket.send(msg.encode('ascii'))

        print('Disconnecting client')
        clientsocket.close()


if __name__ == '__main__':
    main()