import struct
from parser_data import crc

class ResponseHandler:
    def __init__(self, message_parser):
        self.message_parser = message_parser
        self.server_config = self.message_parser.server_config
        self.code_response = None
        self.request_code = self.server_config['MESSAGE_PROPERTIES']['REQUEST_CODE']
        self.response_code = self.server_config['MESSAGE_PROPERTIES']['RESPONSE_CODE']
        self.crc_obj = crc.crc32()

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
            ["CLIENT_ID"] + self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"]["AES_KEY_ENCRYPTED"]
        # make header values bytes:
        # - code and
        # - payload size
        client_id_size_here = self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"] \
            ["CLIENT_ID"]
        aes_size = self.response_code["SEND_AES_KEY_RESPONSE"]["FIELD"] \
            ["AES_KEY_ENCRYPTED"]
        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little', length=4))
        # aes_key_bytes = bytes(self.message_parser.aes_key_encrypted, 'ascii')
        aes_key_bytes = self.message_parser.aes_key_encrypted
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
        """
        response send file response handle
        take care of:
        - response data header.
        - and response data of the payload.
            client_id       : client id
            content_size    : content size of the file sent
            file_name       : name of the file
            cksum           : checksum of the file crc calculation
        :return:
        """
        response_fields = self.response_code["FILE_RECEIVED_CRC_SUCCESS_RESPONSE"]
        self.code_response = response_fields["CODE"]

        self.message_parser.code_response = self.code_response

        client_id_size      = response_fields["FIELD"]["CLIENT_ID"]
        content_size_size   = response_fields["FIELD"]["CONTENT_SIZE"]
        file_name_size      = response_fields["FIELD"]["FILE_NAME"]
        cksum_size          = response_fields["FIELD"]["CKSUM"]

        self.message_parser.payload_size_response = client_id_size + content_size_size + file_name_size + \
            cksum_size

        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes  = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little',
                                           length=4))
        filename_bytes      = self.message_parser.file_name


        # cksum
        self.message_parser.cksum = 0
        self.crc_obj.update(self.message_parser.message_content_decrypted)
        self.message_parser.cksum = self.crc_obj.digest()


        cksum_bytes         = int.to_bytes(self.message_parser.cksum, byteorder='little',
                                           length=4)

        self.message_parser.response_data_header = struct.pack(
            f'1s2s4s',
            self.message_parser.version,
            code_bytes,
            payload_size_bytes
        )
        self.message_parser.response_data = struct.pack(
            f'{client_id_size}s{content_size_size}s{file_name_size}s{cksum_size}s',
            self.message_parser.client_id,
            self.message_parser.content_size_bytes,
            self.message_parser.file_name,
            cksum_bytes)

    def __crc_valid_handle_code(self):
        response_fields = self.response_code["MESSAGE_GOOD"]
        self.code_response = response_fields["CODE"]
        self.message_parser.code_response = self.code_response
        self.message_parser.payload_size_response = 2
        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes  = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little',
                                           length=4))
        self.message_parser.response_data_header = struct.pack(
            f'1s2s4s',
            self.message_parser.version,
            code_bytes,
            payload_size_bytes
        )
        self.message_parser.response_data = struct.pack(
            f'2s',
            b'ok')

    def __crc_not_valid_handle_code(self):
        response_fields = self.response_code["MESSAGE_GOOD"]
        self.code_response = response_fields["CODE"]
        self.message_parser.code_response = self.code_response
        self.message_parser.payload_size_response = 2
        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes  = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little',
                                           length=4))
        self.message_parser.response_data_header = struct.pack(
            f'1s2s4s',
            self.message_parser.version,
            code_bytes,
            payload_size_bytes
        )
        self.message_parser.response_data = struct.pack(
            f'2s',
            b'no')
    def __crc_not_valid_exit_handle_code(self):
        response_fields = self.response_code["MESSAGE_GOOD"]
        self.code_response = response_fields["CODE"]
        self.message_parser.code_response = self.code_response
        self.message_parser.payload_size_response = 2
        code_bytes = (int.to_bytes(self.message_parser.code_response, byteorder='big', length=2))
        payload_size_bytes  = (int.to_bytes(self.message_parser.payload_size_response, byteorder='little',
                                           length=4))
        self.message_parser.response_data_header = struct.pack(
            f'1s2s4s',
            self.message_parser.version,
            code_bytes,
            payload_size_bytes
        )
        self.message_parser.response_data = struct.pack(
            f'2s',
            b'xx')