import sqlite3
import data
import data.logic as logic
import Controller
import threading

# contains port number
PORT_FILE = 'port.info'
HOST = '127.0.0.1'
if __name__ == '__main__':
    # getting the port
    port = Controller.get_port_from_file(PORT_FILE)
    # getting the server object once
    server = Controller.Server(HOST, port)
    server.start()
