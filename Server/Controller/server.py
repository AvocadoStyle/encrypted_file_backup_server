import importlib
import socket
import threading
import Controller.utilities_controller
from Controller.message_handle import MessageHandle

class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_config = Controller.utilities_controller.get_server_configuration()
        self.DISCONNECT_MSG = "disconnect me"

    def start(self):
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
        connection = True
        message_length = self.server_config['MESSAGE_PROPERTIES']['MESSAGE_LENGTH']
        data = conn.recv(message_length)
        # will get dinamic message handler
        message_handler_module = importlib.import_module(f"Controller.message_handle")
        message_handler_obj = getattr(message_handler_module, f"MessageHandle")
        message_handler = message_handler_obj(data, self.server_config)
        message_handler.message_handle_main()



        conn.close()
        # while connection:
        #     data = conn.recv(message_length)
        #     print(f"data is: {data}" )
        #     conn.sendall(b'you are gay!')
        #     conn.close()
        #     connection = False
        # exit(1)

