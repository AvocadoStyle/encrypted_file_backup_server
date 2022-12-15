import struct
import os
from parser_data import cryptography


class MessageParser:
    def __init__(self, data, server_config):
        self.data = data
        self.server_config = server_config
        self.crypt_handle = cryptography.crypt_handle()
        self.client_id = None
        self.version = None
        self.code = None
        self.payload_size = None
        self.payload_content = None

        self.payload_size_response = None
        self.code_response = None
        self.name = None
        self.public_key = None

        self.file_name = None
        self.file_name_explicit_bytes = None
        self.file_name_explicit_string = None
        self.content_size_bytes = None
        self.content_size_int = None
        self.message_content = None
        self.message_content_decrypted = None
        self.aes_key = None
        self.aes_key_encrypted = None
        self.s_key_encrypted = None
        self.cksum = None
        self.date = None

        # finally
        self.response_data = None
        self.response_data_header = None
        self.response_data_payload = None
        self.MAIN_DIRECTORY = 'C:\\server_encrypted_directory'
        self.user_directory_path = None
        self.user_directory_path_with_file = None

    def __str__(self):
        return f"client id: {self.client_id} version: {self.version} code: {self.code}" \
               f"payload_size: {self.payload_size}"

    def init_header_parse(self):
        """
        parse the header content
        :return:
        """
        self.__parse_header_client_id()
        self.__parse_header_version()
        self.__parse_header_code()
        self.__parse_header_payload_size()
        print(self)

    def init_payload_parse(self):
        """
        after the header content parsed will be parsing the payload
        :return:
        """
        self.__parse_payload_content()



    # def general_parse(self, start, end):
    #     pass

    def __parse_header_client_id(self):
        client_id_size = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['CLIENT_ID']
        self.client_id = self.data[client_id_size[0]:client_id_size[1]]

    def __parse_header_version(self):
        version = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['VERSION']
        self.version = self.data[version:version+1]

    def __parse_header_code(self):
        code = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['CODE']
        self.code = self.data[code[0]:code[1]]

    def __parse_header_payload_size(self):
        payload_size = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['PAYLOAD_SIZE']
        self.payload_size = self.data[payload_size[0]:payload_size[1]]

    def __parse_payload_content(self):
        payload_start = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['TOTAL_SIZE']
        if type(self.payload_size) == bytes:
            self.payload_size = int.from_bytes(self.payload_size, byteorder='little')
        payload_end = payload_start + self.payload_size
        self.payload_content = self.data[payload_start:payload_end]

    def parse_name_and_public_key_after(self):
        """
        for the authentication request make explicit parsing name and after the public key
        :return:
        """
        payload_start = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['TOTAL_SIZE']
        if type(self.payload_size) == bytes:
            self.payload_size = int.from_bytes(self.payload_size, byteorder='little')
        payload_end = payload_start + self.payload_size
        # first we'll parse the name
        payload_config_fields = self.server_config['MESSAGE_PROPERTIES']["REQUEST_CODE"]["SEND_PUBLIC_KEY_REQUEST"] \
            ["FIELD"]
        payload_name_size = payload_start + payload_config_fields["NAME"]
        payload_public_key_size = payload_start + payload_config_fields["Public Key"]
        payload_end_should_be = payload_start + payload_name_size + payload_public_key_size
        self.name = self.data[payload_start:payload_name_size]
        self.public_key = self.data[payload_name_size: payload_end]
        return self.name, self.public_key

    def parse_content_size_file_name_message_content(self):
        """
        for the send_file request -
        parse:
        - content_size
        - file_name
        - message_content
        :return:
        """

        # fields size
        payload_send_file_request_fields = self.server_config['MESSAGE_PROPERTIES']['REQUEST_CODE'] \
            ['SEND_FILE_REQUEST']['FIELD']
        client_id_size      = payload_send_file_request_fields['CLIENT_ID']
        content_size        = payload_send_file_request_fields['CONTENT_SIZE']
        file_name_size      = payload_send_file_request_fields['FILE_NAME']


        # payload start and end
        payload_start = self.server_config['MESSAGE_PROPERTIES']['HEADER_SIZE']['TOTAL_SIZE']
        if type(self.payload_size) == bytes:
            self.payload_size = int.from_bytes(self.payload_size, byteorder='little')
        payload_end = payload_start + self.payload_size

        # parse the bytes for each payload content
        payload_now = payload_start + client_id_size
        self.content_size_bytes = self.data[payload_now:(payload_now+content_size)]
        self.content_size_int = int.from_bytes(self.content_size_bytes, byteorder='little') # make it from bytes
        payload_now = payload_now + content_size
        self.file_name = self.data[payload_now: (payload_now+file_name_size)]
        file_name_end = self.file_name.find(b'\x00')
        self.file_name_explicit_bytes = self.file_name[0:file_name_end]
        self.file_name_explicit_string = str(self.file_name_explicit_bytes, 'ascii')
        payload_now = payload_now + file_name_size
        self.message_content = self.data[payload_now: (payload_now+self.content_size_int)]
        payload_now = payload_now + self.content_size_int


    def create_client_file(self):
        with open(os.path.join(self.user_directory_path, self.file_name_explicit_string), "wb") as binary_file:
            # Write bytes to file
            binary_file.write(self.message_content_decrypted)
        print("ok")

    def create_main_directory(self):
        """
        creates the main directory that store the files that the clients sent
        if the directory already exists will do nothing.
        :return:
        """
        main_directory_name = self.MAIN_DIRECTORY
        if not self.__is_directory_exists(main_directory_name):
            self.__create_directory(main_directory_name)


    def set_user_file_name(self, path):
        """
        will set the user file name property
        :param path: the entire path of the file
        """
        with os.path.basename(path) as file_name:
            self.file_name = file_name
            self.file_name = os.path.basename(path)

    def create_user_directory(self, path):
        """
        creates the user directory if it's not exists
        :param path: user id
        """
        main_directory_name = self.MAIN_DIRECTORY
        self.user_directory_path = os.path.join(main_directory_name, path)
        if not self.__is_directory_exists(self.user_directory_path):
            self.__create_directory(self.user_directory_path)

    def __is_directory_exists(self, path):
        """
        check if the directory exists
        :param path: the path of the desired directory
        :return boolean: main directory existence
        """
        return os.path.exists(path)

    def __create_directory(self, path):
        """
        create directory
        :param path: the path of the desired directory
        :return:
        """
        os.mkdir(path)





