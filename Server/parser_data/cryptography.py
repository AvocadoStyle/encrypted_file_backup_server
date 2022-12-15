from Crypto.PublicKey import RSA
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto import Random

class crypt_utility:
    def __init__(self):
        self.IV_INIT = iv_zeros = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.iv_bytes = bytes(self.IV_INIT)
        self.SIZE_OF_SYM_KEY = 16

        self.aes_key = None
        self.cipher = None


    def create_AES(self):
        """
        creating AES key
        :return:
        """
        try:
            self.aes_key = Random.get_random_bytes(self.SIZE_OF_SYM_KEY)
            self.cipher = AES.new(self.aes_key, AES.MODE_CBC)
            return self.cipher.iv
        except Exception as e:
            raise Exception(f'cannot create AES key with error: {e}')

    def encrypt_data(self, cipher, data):
        """
        encrypt any data with given key
        :return:
        """
        try:
            return cipher.encrypt(data)
        except Exception as e:
            raise Exception(f'cannot encrypt data: {e}')

    def decrypt_data_with_aes_key(self, cipher_content, aes_key, content_size):
        """
        decrypt content using AES key
        :param cipher_content:
        :param key:
        :param content_size:
        :return:
        """
        decrypt_cipher = AES.new(aes_key, AES.MODE_CBC, self.iv_bytes)
        content = decrypt_cipher.decrypt(cipher_content)
        decrypted_content = content[:content_size]
        return decrypted_content



class crypt_handle(crypt_utility):
    def __init__(self):
        super().__init__()
        self.encrypted_aes_key_with_rsa_key = None

    def encrypt_AESkey_with_rsa_key_to_send_to_the_client(self, content, rsa_key):
        """
        encrypt the AESkey using the public key from the client.
        the client will use private key to decrypt the content and use the AES key to encrypt the content
        to send
        :param content:
        :param rsa_key:
        :return:
        """
        try:
            RSA.import_key(rsa_key)
            key = RSA.importKey(rsa_key)
            cipher_rsa = PKCS1_OAEP.new(key)
            self.encrypted_aes_key_with_rsa_key = self.encrypt_data(cipher_rsa, content)
            return self.encrypted_aes_key_with_rsa_key
        except Exception as e:
            raise Exception(f'could not encrypt the aes key with the rsa public key: {e}')