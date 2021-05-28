from ecdsa import SigningKey,NIST256p

id_rsu='2-3-2016'
strs=id_rsu+'1'+'1'
print strs

sk = SigningKey.generate(curve=NIST256p) 
sig=sk.sign(strs)
vk = sk.get_verifying_key()            
print vk.verify(sig, strs)
                 
print sig

