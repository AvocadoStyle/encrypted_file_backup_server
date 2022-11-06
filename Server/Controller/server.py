import importlib
import socket
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
        message_length = self.server_config['MESSAGE_PROPERTIES']['MESSAGE_LENGTH']
        data = conn.recv(message_length)
        self.__init_message_handler(data)
        conn.close()

    def __init_message_handler(self, data):
        """
        dynamic message handler
        :return: MessageHandle object
        """
        message_handler_module = importlib.import_module(f"Controller.message_handle")
        message_handler_obj = getattr(message_handler_module, f"MessageHandle")
        message_handler = message_handler_obj(data, self.server_config, self.database)
        message_handler.message_handle_main()

        # while connection:
        #     data = conn.recv(message_length)
        #     print(f"data is: {data}" )
        #     conn.sendall(b'you are gay!')
        #     conn.close()
        #     connection = False
        # exit(1)

    def init_db(self):
        """
        initialize DB
        """
        return {'clients_db': Clients(), 'files_db': Files()}