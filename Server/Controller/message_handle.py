

class MessageHandle:
    def __init__(self, data, server_config):
        """
        runtime MessageHandle in the server, will parse and manage the server received messages
        :param data: the data received from the client in bytes b'...'
        :param server_config: the server necessary configurations from the server
        """
        self.data = data
        self.server_config = server_config

    def message_handle_main(self):
        print(self.data)
