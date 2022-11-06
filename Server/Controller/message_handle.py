import uuid
from parser_data import MessageParser


class MessageHandle:
    def __init__(self, data, server_config, database):
        """
        runtime MessageHandle in the server, will parse and manage the server received messages.
        control the server requests and responses by the HEADER and PAYLOAD content.
        :param data: the data received from the client in bytes b'...'
        :param server_config: the server necessary configurations from the server
        :param database: the database tables of the server clients and files tables
        """
        self.data = data
        self.server_config = server_config
        self.clients_db = database['clients_db']
        self.files_db = database['files_db']
        self.message_parser = MessageParser(self.data, self.server_config)

    def message_handle_main(self):
        """
        gets the message and direct it to the right handler
        """
        print(f"data is: {self.data}")
        print(f"will parse the HEADER of the request")
        self.message_parser.init_header_parse()
        # self.__header_handle()

    def __header_handle(self):
        pass

    # def register_handle(self, data):
    #     """
    #     handle a register request
    #     :return:
    #     """
    #
    #
    #
    #
    #     client_exist = self.clients_db.get_client_by_id(self, )
    #
    #     if client_exist:
    #         pass  # will not create a new registration
    #     else:
    #         uuid_id = uuid.uuid4()
    #         id_str = str(uuid_id)
    #         id_bytes = uuid_id.bytes
    #






