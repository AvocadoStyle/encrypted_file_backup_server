[1mdiff --git a/Server/Controller/message_handle.py b/Server/Controller/message_handle.py[m
[1mindex 517e2cb..087b5ac 100644[m
[1m--- a/Server/Controller/message_handle.py[m
[1m+++ b/Server/Controller/message_handle.py[m
[36m@@ -29,7 +29,7 @@[m [mclass MessageHandle:[m
         self.__payload_handle()[m
 [m
         # handle the requests[m
[31m-        self.__request_handle()[m
[32m+[m[32m        return self.__request_handle()[m
         response_to_client_pack_header_and_payload = None  # @TODO get the response msg and send it to client[m
 [m
     def __header_handle(self):[m
[36m@@ -49,7 +49,7 @@[m [mclass MessageHandle:[m
         will handle the request[m
         :return:[m
         """[m
[31m-        self.request_handler.request_code_handle()[m
[32m+[m[32m        return self.request_handler.request_code_handle()[m
 [m
 [m
 [m
[1mdiff --git a/Server/Controller/server.py b/Server/Controller/server.py[m
[1mindex 53c017d..4d3958c 100644[m
[1m--- a/Server/Controller/server.py[m
[1m+++ b/Server/Controller/server.py[m
[36m@@ -38,8 +38,10 @@[m [mclass Server:[m
         """[m
         connection = True[m
         message_length = self.server_config['MESSAGE_PROPERTIES']['MESSAGE_LENGTH'][m
[32m+[m[32m        # data received and handled[m
         data = conn.recv(message_length)[m
         self.__init_message_handler(data)[m
[32m+[m[32m        conn.send()[m
         conn.close()[m
 [m
     def __init_message_handler(self, data):[m
[1mdiff --git a/Server/parser_data/requests_handler.py b/Server/parser_data/requests_handler.py[m
[1mindex 61d6fd2..3b88016 100644[m
[1m--- a/Server/parser_data/requests_handler.py[m
[1m+++ b/Server/parser_data/requests_handler.py[m
[36m@@ -26,19 +26,20 @@[m [mclass RequestHandler:[m
         """[m
         self.initialization()[m
         CODE = int.from_bytes(self.message_parser.code, byteorder='little')[m
[31m-[m
[32m+[m[32m        response = None[m
         if CODE == self.request_code['REGISTRATION_REQUEST']['CODE']:[m
[31m-            self.__registration_handle_code()[m
[32m+[m[32m             response = self.__registration_handle_code()[m
         elif CODE == self.request_code['SEND_PUBLIC_KEY_REQUEST']['CODE']:[m
[31m-            self.__send_public_key_handle_code()[m
[32m+[m[32m            response = self.__send_public_key_handle_code()[m
         elif CODE == self.request_code['SEND_FILE_REQUEST']['CODE']:[m
[31m-            self.__send_file_handle_code()[m
[32m+[m[32m            response = self.__send_file_handle_code()[m
         elif CODE == self.request_code['CRC_VALID_REQUEST']['CODE']:[m
[31m-            self.__crc_valid_handle_code()[m
[32m+[m[32m            response = self.__crc_valid_handle_code()[m
         elif CODE == self.request_code['CRC_NOT_VALID_REQUEST']['CODE']:[m
[31m-            self.__crc_not_valid_handle_code()[m
[32m+[m[32m            response = self.__crc_not_valid_handle_code()[m
         elif CODE == self.request_code['CRC_NOT_VALID_EXIT_REQUEST']['CODE']:[m
[31m-            self.__crc_not_valid_exit_handle_code()[m
[32m+[m[32m            response = self.__crc_not_valid_exit_handle_code()[m
[32m+[m[32m        return response[m
 [m
     def __registration_handle_code(self):[m
         """[m
[36m@@ -60,6 +61,8 @@[m [mclass RequestHandler:[m
         self.clients_db.insert_table(client_id_bytes, content)[m
         result = self.clients_db.get_client_by_id(client_id_bytes)[m
         print(f'id is: {result[0][0]} name is: {result[0][1]}')[m
[32m+[m[32m        print("now returning the uuid to the client: ")[m
[32m+[m[32m        return client_id_bytes[m
 [m
 [m
     def __send_public_key_handle_code(self):[m
