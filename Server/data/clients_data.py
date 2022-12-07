from data.data_consumer import DataConsumer
from data.logic import ClientLogic
class Clients(DataConsumer):
    def __init__(self):
        super().__init__()
        # get the data sizes for the specific table name
        self.clients_data_size = self.data_size[self.CLASS_NAME]
        self.client_logic = None
        if self.clients_data_size is None:
            raise Exception(f"there is no data about the size of this table: {self.CLASS_NAME}")
        self.create_table()

    def create_table(self):
        """
        create Clients table once
        :return:
        """
        self.connect()
        self.connector.executescript(f"""
                CREATE TABLE IF NOT EXISTS {self.CLASS_NAME} (
                    ID BLOB PRIMARY KEY,
                    Name CHAR(255),
                    PublicKey CHAR(160),
                    LastSeen DATE,
                    AESkey BLOB
                    );
                """)
        # self.connector.executescript(f"""
        #         CREATE TABLE IF NOT EXISTS {self.CLASS_NAME} (
        #             ID BLOB PRIMARY KEY,
        #             Name CHAR(255) NOT NULL
        #             );
        #         """)
        self.close_connection()

    def insert_table(self, ID=None, Name=None, PublicKey=None, LastSeen=None, AESkey=None):
        """
        insert the client table values for one client.
        check inside if the parameters are valid for the protocol.
        :param ID:
        :param Name:
        :param PublicKey:
        :param LastSeen:
        :param AESkey:
        :return:
        """
        self.client_logic = ClientLogic(ID, Name, PublicKey, LastSeen, AESkey)
        self.client_logic.check_protocol_valid()  # @TODO
        insert_command_query = (f"""INSERT INTO {self.CLASS_NAME} VALUES(?, ?, ?, ?, ?)""", (
            self.client_logic.ID,
            self.client_logic.Name,
            self.client_logic.PublicKey,
            self.client_logic.LastSeen,
            self.client_logic.AESkey))
        self.execute_query(insert_command_query)

    def get_client_by_id(self, ID):
        """
        :param ID:
        :return:
        """
        command_query = (f"""SELECT * FROM {self.CLASS_NAME} WHERE ID = ? """, (ID,))
        return self.execute_query(command_query)

