from data_consumer import DataConsumer


class Files(DataConsumer):
    def __init__(self):
        super().__init__()
        # get the data sizes for the specific table name
        self.files_data_size = self.data_size[self.CLASS_NAME]


