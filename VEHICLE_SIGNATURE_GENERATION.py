import hashlib
class signature():
         def initialize(self,toht,alphaj,betaj,ipidvi):
                H4=hashlib.sha1()
                H4.update(ipidvi+toht)
                ppid_it1=H4.hexdigest()
                #print pi_i3
                
                H0=hashlib.sha1()
                H0.update(ppid_it1+`0`)
                pid_it0=H0.hexdigest()
                #print ppid_it0
                H0=hashlib.sha1()
                H0.update(ppid_it1+`1`)
                pid_it1=H0.digest()
                #print ppid_it1
                return ppid_it1,pid_it0
                
#vs=signature()
#vs.initialize("40mins",2,2,"2-3-2016")

                              
               
 

