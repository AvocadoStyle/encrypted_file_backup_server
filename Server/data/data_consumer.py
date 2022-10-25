import json
import sqlite3


class DataConsumer:
    SQL_DB_FILENAME = "server.db"

    def __init__(self):
        """
        configure the Database table generic to the inherit classes that are used to integrate with the SQL
        tables
        """
        # will use the class name of the inherit classes to get data sizes of tables
        self.CLASS_NAME = self.__class__.__name__
        # use as connector to the sqlite3
        self.connector = None
        # table data sizes dict loading
        self.data_size = None
        self.load_data_size()

    def connect(self):
        self.connector = sqlite3.connect(self.SQL_DB_FILENAME)
        if self.connector is None:
            raise Exception

    def close_connection(self):
        self.connector.close()

    def load_data_size(self):
        with open('sql_table_field_size_configuration.json') as sql_size_data:
            self.data_size = json.load(sql_size_data)

    def create_table(self):
        pass



