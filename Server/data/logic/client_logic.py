class ClientLogic:
    def __init__(self, ID, Name, PublicKey, LastSeen, AESkey):
        self.ID = ID

    def check_protocol_valid(self) -> bool:

