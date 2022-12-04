import importlib
import socket
import struct
import threading
import Controller.utilities_controller
from data import Clients, Files

class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_config = Controller.utilities_controller.get_server_configuration()
        self.database = self.init_db()

    def start(self):
        """
        start listening to multiple clients in concurrency and supported with threads
        :return:
        """
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen(self.server_config['MAX_CLIENTS_CONNECTED'])
            print("try to connect...")
            while True:
                conn, addr = s.accept()
                # handle client request
                client_thread = threading.Thread(target=self.handle_request, args=(conn, addr))
                client_thread.start()
                # get the running threads
                print(f"[ACTIVE CONNECTIONS] {threading.activeCount()-1}")

    def handle_request(self, conn, addr):
        """
        handle the requests, will be used in the server with threads from `start` method.
        could be used without threads or start method.
        :param conn: client socket connection
        :param addr: address of the client
        """
        connection = True
        message_header_length = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['TOTAL_SIZE']


        # header data received
        data_header = conn.recv(message_header_length)
        message_handler = self.__init_message_handler(data_header)
        # from the header get the size of the payload + header size
        size = message_handler.message_parser.payload_size
        size = int.from_bytes(size, byteorder='little') + message_header_length  # header+payload size


        # all message receive
        data = conn.recv(size)
        self.__init_all_message(message_handler, data)


        # response handler
        response_data_header = message_handler.message_parser.response_data_header
        response_data = message_handler.message_parser.response_data
        message_header_length = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE_RESPONSE']['TOTAL_SIZE']
        conn.sendall(response_data_header)
        conn.sendall(response_data)

        # a = 1
        # bt2 = int.to_bytes(a, byteorder='big', length=1001)
        # bt3 = struct.pack('23s1001s', message_handler.message_parser.response_data, bt2)
        # conn.sendall(message_handler.message_parser.response_data)
        # conn.sendall(bt3)






        conn.close()

    def __init_message_handler(self, data):
        """
        dynamic message requests handler and responses
        :return: MessageHandle object
        """
        # get the message handler module
        message_handler_module = importlib.import_module(f"Controller.message_handle")
        message_handler_obj = getattr(message_handler_module, f"MessageHandle")
        message_handler = message_handler_obj(data, self.server_config, self.database)
        message_handler.message_handle_main()
        return message_handler

    def __init_all_message(self, message_handler, data):
        """
        dynamic message requests handler and responses
        """
        message_handler.data = data
        message_handler.message_parser.data = data
        message_handler.message_handle_request()



    def init_db(self):
        """
        initialize DB
        """
        return {'clients_db': Clients(), 'files_db': Files()}