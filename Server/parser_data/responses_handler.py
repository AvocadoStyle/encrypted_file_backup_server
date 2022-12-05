import struct

class ResponseHandler:
    def __init__(self, message_parser):
        self.message_parser = message_parser
        self.server_config = self.message_parser.server_config
        self.code_response = None
        self.request_code = self.server_config['MESSAGE_PROPERTIES']['REQUEST_CODE']
        self.response_code = self.server_config['MESSAGE_PROPERTIES']['RESPONSE_CODE']

    def response_code_handle(self):
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
        registration handle, creates response_data_header, and response_data to send to the client
        :return:
        """
        self.code_response = self.response_code["REGISTRATION_SUCCESS_RESPONSE"]["CODE"]
        self.message_parser.code_response = self.code_response
        self.message_parser.payload_size_response = self.response_code["REGISTRATION_SUCCESS_RESPONSE"]["FIELD"] \
            ["CLIENT_ID"]
        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little', length=4))
        self.message_parser.response_data_header = struct.pack('1s2s4s', self.message_parser.version,
                                                               code_bytes,
                                                               payload_size_bytes)
        self.message_parser.response_data = struct.pack('1s2s4s16s', self.message_parser.version,
                                                         code_bytes,
                                                         payload_size_bytes,
                                                         self.message_parser.client_id)


    def __send_public_key_handle_code(self):
        """
        part of the registration - authentication, creates response _data_header, and response_data to send to
         the client with:
          - client_id
          - AES key
        :return:
        """
        self.code_response = self.response_code["SEND_AES_KEY_RESPONSE"]["CODE"]
        self.message_parser.code_response = self.code_response

        self.message_parser.payload_size_response = self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"] \
            ["CLIENT_ID"] + self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"]["AES_KEY"]
        # make header values bytes:
        # - code and
        # - payload size
        client_id_size_here = self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"] \
            ["CLIENT_ID"]
        aes_size = self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"] \
            ["AES_KEY"]
        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little', length=4))
        aes_key_bytes = bytes(self.message_parser.aes_key, 'ascii')
        self.message_parser.response_data_header = struct.pack('1s2s4s', self.message_parser.version,
                                                               code_bytes,
                                                               payload_size_bytes)
        self.message_parser.response_data = struct.pack(f'1s2s4s{client_id_size_here}s{aes_size}s',
                                                        self.message_parser.version,
                                                         code_bytes,
                                                         payload_size_bytes,
                                                         self.message_parser.client_id,
                                                         aes_key_bytes)


    def __send_file_handle_code(self):
        pass

    def __crc_valid_handle_code(self):
        pass

    def __crc_not_valid_handle_code(self):
        pass

    def __crc_not_valid_exit_handle_code(self):
        pass
