import struct


class MessageParser:
    def __init__(self, data, server_config):
        self.data = data
        self.server_config = server_config
        self.client_id = None
        self.version = None
        self.code = None
        self.payload_size = None
        self.payload_content_obj = None

    def __str__(self):
        return f"client id: {self.client_id} version: {self.version} code: {self.code}" \
               f"payload_size: {self.payload_size}"

    def init_header_parse(self):
        self.__parse_header_client_id()
        self.__parse_header_version()
        self.__parse_header_code()
        self.__parse_header_payload_size()
        print(self.__str__())




        # struct_header = struct.Struct(f'{header_size}s')

        # struct_header = struct.unpack(f'{header_size}s', self.data)

        # print(f"data parse@@@@: {self.data[0:3]}")
        # print(struct_header[0])
        # print(struct_header[1])
        # self.client_id =

    def init_payload_parse(self):
        pass


    def general_parse(self, start, end):
        pass

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












