from data_consumer import DataConsumer


class Clients(DataConsumer):
    def __init__(self):
        super().__init__()
        # get the data sizes for the specific table name
        self.clients_data_size = self.data_size[self.CLASS_NAME]
        if self.clients_data_size is None:
            raise Exception(f"there is no data about the size of this table: {self.CLASS_NAME}")

    def create_table(self):
        pass

    def add_client(self):
        self.connect()
        pass

    def add_public_key(self):
        pass

    def generate_uuid(self):
        pass

    def generate_public_key(self):
        pass

    def generate_AES_key(self):
        pass

    def update_last_seen(self):
        pass