from data_consumer import DataConsumer


class Files(DataConsumer):
    def __init__(self):
        super().__init__()
        # get the data sizes for the specific table name
        self.files_data_size = self.data_size[self.CLASS_NAME]
        if self.files_data_size is None:
            raise Exception(f"there is no data about the size of this table: {self.CLASS_NAME}")

    def create_table(self):
        self.connect()
        self.connector.executescript(f"""
                CREATE TABLE {self.CLASS_NAME} (
                    FOREIGN KEY(ID) REFERENCES Clients(ID),
                    FileName CHAR(255) NOT NULL,
                    PathName CHAR(255) NOT NULL,
                    Verified BOOLEAN
                    );
                """)
        self.close_connection()

    def insert_table(self, file_logic):
        self.connect()
        self.connector.executescript(f"""
        INSERT INTO {self.CLASS_NAME} VALUES({ID}, {FileName}, {PathName}, {Verified});
        """)
        self.close_connection()





