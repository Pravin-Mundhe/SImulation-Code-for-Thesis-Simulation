import pyaes
import sys

fo = open("KeyExchange.txt", "r")
key=fo.read()
plaintext = "Text may be any length you wish, no padding is required"

# key must be bytes, so we convert it
key = (key.encode('utf-8'))[0:32]

aes = pyaes.AESModeOfOperationCTR(key)    
ciphertext = aes.encrypt(plaintext)

aes1 = pyaes.AESModeOfOperationCTR(key)
# decrypted data is always binary, need to decode to plaintext
decrypted = aes1.decrypt(ciphertext).decode('utf-8')

print ("Data after decryption:\n"+decrypted)
# True
print (decrypted == plaintext)





















