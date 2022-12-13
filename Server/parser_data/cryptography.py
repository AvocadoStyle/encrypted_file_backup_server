from Crypto.Cipher import AES
from Crypto.PublicKey import RSA
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto import Random


class crypt_handle:
    def __init__(self, content):
        self.IV_INIT = iv_zeros = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.iv_bytes = bytes(self.IV_INIT)
        self.SIZE_OF_SYM_KEY = 16
        self.aes_key = None
        self.cipher_rsa = None
        self.cipher_content = None
        self.content = content

    def create_AES(self):
        """
        creating AES key
        :return:
        """
        # generate random Aes key
        self.aes_key = Random.get_random_bytes(self.SIZE_OF_SYM_KEY)
        # Encrypt the session key with the public RSA key
        self.cipher_rsa = AES.new(self.aes_key, AES.MODE_CBC)
        print( "Generated AES key: " + str(self.cipher_rsa.IV))
        return self.cipher_rsa.IV

    def encrypt_content_with_AES(self):
        if self.content != None:
            try:
                self.cipher_rsa.encrypt_and_digset(self.content)
            except:
                raise Exception("problem with the encryption of the content")
        else:
            raise Exception("there is no content")

    def encrypt_aes_using_public_key(self, data, key):
        """
        AES key encrypted with public key that received from the client
        :param data:
        :param key:
        :return:
        """
        try:
            key = bytes.decode(key, 'utf-8')
            RSA.import_key(key)
            recipient_key = RSA.importKey(key, 'DER')
            cipher_rsa = PKCS1_OAEP.new(recipient_key)
            enc_session_key = cipher_rsa.encrypt(data)
            return enc_session_key
        except Exception as e:
            pass

    def decrypt_cipher_content(self, ciphertext, key, content_size):
        """
        decrypt cipher content using AES key
        :param ciphertext: encoded content
        :param key: AES KEY
        :param content_size: the size of the content
        """
        try:
            decrypt_cipher = AES.new(key, AES.MODE_CBC, self.iv_bytes)
            plain_text = decrypt_cipher.decrypt(ciphertext)
            decrypted_content = plain_text[:content_size]
            return decrypted_content
        except Exception as e:
            print(e)
            return None