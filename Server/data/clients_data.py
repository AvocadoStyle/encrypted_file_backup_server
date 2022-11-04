from data.data_consumer import DataConsumer

class Clients(DataConsumer):
    def __init__(self):
        super().__init__()
        # get the data sizes for the specific table name
        self.clients_data_size = self.data_size[self.CLASS_NAME]
        if self.clients_data_size is None:
            raise Exception(f"there is no data about the size of this table: {self.CLASS_NAME}")

    def create_table(self):
        self.connect()
        self.connector.executescript(f"""
                CREATE TABLE IF NOT EXISTS {self.CLASS_NAME} (
                    ID CHAR(16) NOT NULL PRIMARY KEY,
                    Name CHAR(255) NOT NULL,
                    PublicKey CHAR(160) NOT NULL,
                    LastSeen DATE,
                    AESkey
                    );
                """)
        self.close_connection()

    def insert_table(self, client_logic):
        insert_command_query = f"""
            INSERT INTO {self.CLASS_NAME} VALUES({client_logic.ID}, "{client_logic.Name}",
             "{client_logic.PublicKey}", "{client_logic.LastSeen}", "{client_logic.AESkey}");
            """
        self.insert_query(insert_command_query)

    def get_client_by_id(self, ID):
        command_query = f"""
            SELECT * FROM {self.CLASS_NAME}
            WHERE ID = {ID}
            """
        return self.execute_query(command_query)
