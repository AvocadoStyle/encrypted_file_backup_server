from data import DataConsumer
from data.logic import FileLogic

class Files(DataConsumer):
    def __init__(self):
        super().__init__()
        # get the data sizes for the specific table name
        self.files_data_size = self.data_size[self.CLASS_NAME]
        self.file_logic = None
        if self.files_data_size is None:
            raise Exception(f"there is no data about the size of this table: {self.CLASS_NAME}")
        self.create_table()

    def create_table(self):
        self.connect()
        qe = f"""
                CREATE TABLE IF NOT EXISTS {self.CLASS_NAME} (
                    ID BLOB, 
                    FileName CHAR(255) PRIMARY KEY,
                    PathName CHAR(255),
                    Verified BOOLEAN
                    );
                """
        self.connector.executescript(qe)
        self.close_connection()

    def insert_table(self, ID=None, FileName=None, PathName=None, Verified=True):
        """
        insert the values into the file data.
        checks if the parameters are valid with the protocol.
        :param ID:
        :param FileName:
        :param PathName:
        :param Verified:
        :return:
        """
        self.file_logic = FileLogic(ID, FileName, PathName, Verified)
        insert_command_query = (f"""INSERT INTO {self.CLASS_NAME} VALUES(?, ?, ?, ?)""", (
                                self.file_logic.ID,
                                self.file_logic.FileName,
                                self.file_logic.PathName,
                                self.file_logic.Verified))
        # self.insert_query(insert_query_command)
        self.execute_query(insert_command_query)

    def get_file_by_path_name(self, path_name):
        """
        getting the file by the path name
        :param path_name:
        :return:
        """
        command_query = (f"""SELECT * FROM {self.CLASS_NAME} WHERE PathName = ? """, (path_name,))
        return self.execute_query(command_query)












