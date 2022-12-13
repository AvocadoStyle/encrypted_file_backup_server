import uuid
import datetime


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
        """
        registration request - save to the Clients DB
            - if the client name does not exists it will send back error code that registration failed
            - new AES key from public key added to the DB
        :return:
        """
        # name + pk
        content = self.payload_content
        name, public_key = self.message_parser.parse_name_and_public_key_after()
        self.message_parser.name = name
        self.message_parser.public_key = public_key

        print(f"name is: {name} public_key is: {public_key}" )
        aes_key = self.__generate_aes_key(public_key)
        self.message_parser.aes_key = aes_key
        date = datetime.datetime.now().__str__()
        self.clients_db.insert_table(self.message_parser.client_id, name, public_key, date, aes_key)


    def __generate_aes_key(self, public_key):
        """
        generates AES key from public key that received from the client
        :param public_key: public key from the client.
        :return: AES key based on the public key.
        """
        return "12345"



    def __send_file_handle_code(self):
        content = self.payload_content
        self.message_parser.parse_content_size_file_name_message_content()

        # create main directory
        self.message_parser.create_main_directory()

        # create directory_name from the client_id
        client_id = self.message_parser.client_id
        user_directory_name = str(int.from_bytes(client_id, byteorder='little'))
        self.message_parser.create_user_directory(user_directory_name)
        # self.message_parser.set_user_file_name(self.message_parser.file_name)
        self.message_parser.create_client_file()

        # save the data in the file DB
        self.files_db.insert_table(self.message_parser.client_id, self.message_parser.file_name,
                                   self.message_parser.user_directory_path, True)






    def __crc_valid_handle_code(self):
        pass

    def __crc_not_valid_handle_code(self):
        pass

    def __crc_not_valid_exit_handle_code(self):
        pass

