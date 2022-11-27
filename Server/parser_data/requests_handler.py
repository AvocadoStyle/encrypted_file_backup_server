import uuid


class RequestHandler:
    def __init__(self, message_parser, clients_db, files_db):
        self.message_parser = message_parser
        self.clients_db = clients_db
        self.files_db = files_db

        # @TODO get the response to the client to send back according to the code
        self.response_handler_obj = None
        self.response_handler = None

        self.payload_content = self.message_parser.payload_content
        self.server_config = self.message_parser.server_config

        self.request_code = self.server_config['MESSAGE_PROPERTIES']['REQUEST_CODE']
        self.response_code = self.server_config['MESSAGE_PROPERTIES']['RESPONSE_CODE']

    def initialization(self):
        self.payload_content = self.message_parser.payload_content

    def request_code_handle(self):
        """
        decide the request operation to execute
        """
        self.initialization()
        CODE = int.from_bytes(self.message_parser.code, byteorder='little')
        if CODE == self.request_code['REGISTRATION_REQUEST']['CODE']:
             self.__registration_handle_code()
        elif CODE == self.request_code['SEND_PUBLIC_KEY_REQUEST']['CODE']:
            self.__send_public_key_handle_code()
        elif CODE == self.request_code['SEND_FILE_REQUEST']['CODE']:
            self.__send_file_handle_code()
        elif CODE == self.request_code['CRC_VALID_REQUEST']['CODE']:
            self.__crc_valid_handle_code()
        elif CODE == self.request_code['CRC_NOT_VALID_REQUEST']['CODE']:
            self.__crc_not_valid_handle_code()
        elif CODE == self.request_code['CRC_NOT_VALID_EXIT_REQUEST']['CODE']:
            self.__crc_not_valid_exit_handle_code()

    def __registration_handle_code(self):
        """
        registration request - save to the Clients DB
            - new uuid generated
            - if the Name from the content does not exists in the DB it will insert it to the DB else it will throw
                an exception and send error as response
        :return:
        """
        # generate uuid
        client_id = uuid.uuid4()
        client_id_bytes = client_id.bytes

        # insert the UUID with the NAME to the DB if the name is not exists
        content = self.payload_content # name
        self.message_parser.name = content





        self.clients_db.insert_table(client_id_bytes, content)
        result = self.clients_db.get_client_by_id(client_id_bytes)
        print(f'id is: {result[0][0]} name is: {result[0][1]}')
        print(f'name maybe: {self.message_parser.name}')
        print("now returning the uuid to the client: ")
        self.message_parser.client_id = client_id_bytes

    def __send_public_key_handle_code(self):
        pass

    def __send_file_handle_code(self):
        pass

    def __crc_valid_handle_code(self):
        pass

    def __crc_not_valid_handle_code(self):
        pass

    def __crc_not_valid_exit_handle_code(self):
        pass
