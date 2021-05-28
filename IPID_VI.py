import hashlib
import random

class ipidvi:         
    def ipid_vi(self,idvi,vpi): 
        key="00100"        
        H2=hashlib.sha1()        
        y=idvi+vpi
        H2.update(y)
        return key,H2.hexdigest()
