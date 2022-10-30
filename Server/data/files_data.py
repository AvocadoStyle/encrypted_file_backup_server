from data import DataConsumer

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
                CREATE TABLE IF NOT EXISTS {self.CLASS_NAME} (
                    FOREIGN KEY(ID) REFERENCES Clients(ID),
                    FileName CHAR(255) NOT NULL,
                    PathName CHAR(255) NOT NULL,
                    Verified BOOLEAN
                    );
                """)
        self.close_connection()

    def insert_table(self, file_logic):
        insert_query_command = f"""
        INSERT INTO {self.CLASS_NAME} VALUES({file_logic.ID}, {file_logic.FileName}, {file_logic.PathName},
            {file_logic.Verified});
        """
        self.insert_query(insert_query_command)







