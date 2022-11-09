import data.utilities_data
# from data import utilities_data


class ClientLogic:
    def __init__(self, ID=None, Name=None, PublicKey=None, LastSeen=None, AESkey=None):
        self.ID = ID
        self.Name = Name
        self.PublicKey = PublicKey
        self.LastSeen = LastSeen
        self.AESkey = AESkey
        self.check_protocol_valid()

    def check_protocol_valid(self):
        """
        check protocl validation of the input
        :return:
        """
        print(data.utilities_data.load_data_size())

