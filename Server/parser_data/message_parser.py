import struct


class MessageParser:
    def __init__(self, data, server_config):
        self.data = data
        self.server_config = server_config
        self.client_id = None
        self.version = None
        self.code = None
        self.payload_size = None
        self.payload_content = None

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
        self.payload_size = int.from_bytes(self.payload_size, byteorder='big')
        payload_end = payload_start + self.payload_size
        self.payload_content = self.data[payload_start:payload_end]















