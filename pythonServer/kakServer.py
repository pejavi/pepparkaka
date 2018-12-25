import time
import socket
import datetime
import logging
import threading
import serial.tools.list_ports

DEBUG = True;
SER_BAUDRATE = 9600
# logging.basicConfig(filename='example.log', filemode='w', level=logging.DEBUG)
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(message)s')
controllers = list()
ser = serial


class Controller:
    def __init__(self, device):
        try:
            self.name = device
            self.ser = serial.Serial(device)
            time.sleep(1)
            self.response = ""
            self.thread = threading.Thread(target=self.read_from_port)
            self.thread.start()
        except (OSError, serial.SerialException):
            logging.debug("Failed to connect to :" + device)

    def __del__(self):
        self.ser.close()

    def write(self, msg):
        try:
            logging.debug("Write to " + self.name + ": " + msg.strip())
            self.ser.write(msg.encode('ascii'))
            self.ser.flush()
            # self.read()
        except (OSError, serial.SerialException):
            logging.warning("Failed to communicate with " + self.name)

    def read(self):
        # TODO: Handle exceptions, like disconnecting cable
        msg = self.ser.readline()
        self.response = msg.strip()
        logging.debug("Read from " + self.name + ": " + msg.strip())
        return msg

    def read_from_port(self):
        while True:
            self.read()
        logging.error("Stopped reading " + self.name)


def debug_print(msg):
    if DEBUG is True:
        print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f: ") + msg)


def scan_serial_ports():
    serial_ports = serial.tools.list_ports.comports()
    for p in serial_ports:
        try:
            logging.info("Try to attach to " + str(p))
            ctrl = Controller(p.device)
            time.sleep(1)
            ctrl.write('ID?\n')
            time.sleep(0.5)
            if "RGBW" in ctrl.response:
                logging.info("Successfully connected to " + p.device)
                controllers.append(ctrl)
        except (OSError, serial.SerialException):
            logging.warning("Failed to connect to :" + p.device)

    if not controllers:
        logging.error("No ports found!")

def main():
    server_socket = socket.socket(
      socket.AF_INET, socket.SOCK_STREAM)

    host = ''
    port = 5555

    server_socket.bind((host, port))

    server_socket.listen(5)

    scan_serial_ports()

    while True:
        try:
            logging.info('Waiting for connection')
            client_socket, address = server_socket.accept()
            logging.info('Got a connection from {}'.format(str(address)))

            msg = "CONNECTED\n"
            client_socket.send(msg.encode('ascii'))

            while True:
                msg = client_socket.recv(1024).decode('ascii')
                if msg.strip() == "" or msg.strip() == "DISCONNECT":
                    break
                logging.debug("Socket received: " + msg.strip())
                controllers[0].write(msg)

            msg = "Closing connection\n"
            client_socket.send(msg.encode('ascii'))

        except socket.error, e:
            logging.error("Socket error" + e)

        finally:
            logging.info('Disconnecting client ' + str(client_socket.getpeername()))
            client_socket.close()


if __name__ == '__main__':
    main()