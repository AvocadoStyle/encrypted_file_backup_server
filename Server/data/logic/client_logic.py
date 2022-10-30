import data.utilities_data
# from data import utilities_data


class ClientLogic:
    def __init__(self, ID, Name, PublicKey, LastSeen, AESkey):
        self.ID = ID
        self.Name = Name
        self.PublicKey = PublicKey
        self.LastSeen = LastSeen
        self.AESkey = AESkey

    def check_protocol_valid(self):
        print(data.utilities_data.load_data_size())

