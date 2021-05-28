from ecdsa import SigningKey,NIST256p

class signature:

            def signature_rsu(self,strs):
                sk = SigningKey.generate(curve=NIST256p) 
                sig=sk.sign(strs)
                vk = sk.get_verifying_key()            
                if (vk.verify(sig, strs)==True):
                    return sig  
                    
#id_rsu='2-3-2016'
#strs=id_rsu+`1`+`1`
#print strs

#rsu=signature()                
#sig=rsu.signature_rsu(strs)          
