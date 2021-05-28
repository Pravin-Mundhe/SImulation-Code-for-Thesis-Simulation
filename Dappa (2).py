import csv,time
import hashlib,hmac,random,math
import GENERATOR_GROUP
import AES,ML_STOREE,VEHICLE_MEMBER_SECRET
from hashlib import sha1
#--------------- Public Parameters -----------------
q=631;    # prime order value 
#G1=AdditiveAbelianGroup([q])      # Cyclic Group G1 of order 'q'
#g=GENERATOR_GROUP.generator_of_group(q)
g1=19      # rand.choice(g),Generator of Group G1
#G2=AdditiveAbelianGroup([q])      # Cyclic Group G1 of order 'q'
g2=26      # Generator of Group G1
_k=4# or rand.randint(1,632)
_yeta=3      # k and yeta-private variables 
     
y=pow(g2,_k)
e=pow(g1,_yeta)
#----------------- RSU Certificate Generation & Verification------------ 
_kj=4# or rand.randint(1,632)
_yetaj=2
y1=pow(g2,_k)
e1=pow(g1,_yeta)
 
id_rsu="jhalwa"
strs=id_rsu+`y1`+`e1`
H2=hmac.new(str(y),'',sha1)
H2.update(strs)
sig=H2.hexdigest()

#a = time.time()
strs1="jhalwa"+`y1`+`e1`
H2=hmac.new(str(y),'',sha1)
H2.update(strs1)
sig1=H2.hexdigest()

'''time.sleep(1)
b = time.time()
print(b-a)'''
#Certificate verification
#if (sig==sig1):
 #   print "valid signature"

#----------------- RSU Certificate Initialization------------
yj=456976
ej=6859
#----------------- Internal Pseudo Identity Generation ------
#eid computation
idvi="2-1-2018"
a = time.time()
key="197190621687286316448341371151102121111459122022841571444721628100173145161218218"

E=AES.AESCipher(key)
eid_vi=E.encrypt(idvi)
#IPID computation
vpi="1.1.2018-2.1.2018" 
lambda_vi="00100"

y=eid_vi+vpi
H2=hashlib.sha1()
H2.update(y)
ipidvi=H2.hexdigest()
#--------------------- TPD Initialize -----------------
ml=ML_STOREE.mlstore(eid_vi,vpi,ipidvi,lambda_vi)
ml.eid_vi
ml.vp_i
ml.ipid_vi
ml.lambda_vi 
#------------------ Member Secrets Generation -----------------
#Data of Stage1 Computation
theta=2  #rand.randint(1,q-1)
f=pow(g1,theta)             #g1=19,g2=26,f=19^2=361
ej_theta=pow(ej,theta)      
e_theta=pow(e,theta)
toht="60min"

pi2_e=`f`+`e`+`e_theta`+id_rsu+toht   #e=pow(g1,_yeta)=19^3=6859
states=str(lambda_vi)+" "+toht        #e_theta=pow(e,theta)=6859^2=47045881

H3=hashlib.sha1()
H3.update(pi2_e)
pi_i2e=H3.hexdigest() 

E=AES.AESCipher(pi_i2e)
rho_j=E.encrypt(states)

#a = time.time()
#if(toht=="60min"):
 #   print "valid timestamp" 
    
'''time.sleep(1)
b = time.time()
print(b-a)'''

#Data of Stage2 Computation
#a = time.time()
f_yeta=pow(f,_yeta)
pi2_f=`f`+`e`+`f_yeta`+id_rsu+toht      #f_yeta=f^_yeta=361^3=47045881
        
H3=hashlib.sha1()
H3.update(pi2_f)
pi_i2f=H3.hexdigest()

E1=AES.AESCipher(pi_i2f)
decrypted=E1.decrypt(rho_j)
rand=decrypted.split()  #rand=lambda_vi+" "+toht

'''time.sleep(1)
b = time.time()
print(b-a)'''
'''
#Authentication key verification
#a = time.time()
if(rand[0]==lambda_vi):
    print "valid"
time.sleep(1)
b = time.time()
print(b-a)'''

'''
#Timestamp verification
#a = time.time()
if(rand[1]==toht):
    print "valid" '''
'''time.sleep(1)
b = time.time()
print(b-a)  '''  

#a = time.time()
#MS Computation by RSU
f_yetaj=pow(f,_yetaj)

pi1=`f`+`ej`+`f_yetaj`+id_rsu+toht        
H3=hashlib.sha1()
H3.update(pi1)
pi_i1=H3.hexdigest()

tohp="30min"
alphaj=2
betaj=2
hrjstr=tohp+`alphaj`+`betaj`
     
H2=hmac.new(pi_i1,'',sha1)
H2.update(hrjstr)
hrj=H2.hexdigest()

rhoj1str=tohp+" "+`alphaj`+" "+`betaj`+" "+hrj

E2=AES.AESCipher(pi_i1)
rho_j1=E2.encrypt(rhoj1str)

H4=hashlib.sha1()
H4.update(str(f))
h4f=H4.hexdigest()

'''time.sleep(1)
b = time.time()
print(b-a) '''

#V verifies MS
#a = time.time()
        
E3=AES.AESCipher(pi_i1) 
decrypted1=E3.decrypt(rho_j1)
rand1=decrypted1.split() #rand1=tohp+" "+`alphaj`+" "+`betaj`+" "+hrj

#if(rand1[3]==hrj):
    #print "valid"   
'''time.sleep(1)
b = time.time()
print(b-a)'''
#V sets MS
#a = time.time()
ms=VEHICLE_MEMBER_SECRET.Set_Secret(rand1[0],rand1[1],rand1[2])
'''time.sleep(1)
b = time.time()
print(b-a)'''
#----------------- Message Signature Generation --------------------------
#PPID_VI Generation
#a = time.time()
H3=hashlib.sha1()
H3.update(ipidvi)
ppidvi=H3.hexdigest()
'''time.sleep(1)
b = time.time()
print(b-a)'''
#Encrypted message(EM) computation
#a = time.time()
mi="congested"
key="197190621687286316448341371151102121111459122022841571444721628100173145161218218"

E=AES.AESCipher(key)
emivi=E.encrypt(mi)

'''time.sleep(1)
b = time.time()
print(b-a)'''
#Signature generation
#a = time.time()
H3=hashlib.sha1()
H3.update(emivi)
msgsig=H3.hexdigest()

'''time.sleep(1)
b = time.time()
print(b-a)'''
#Signature Verification
#a = time.time()
H3=hashlib.sha1()
H3.update(emivi)
msgsig1=H3.hexdigest()

'''if(msgsig==msgsig1):
    print "equal"
time.sleep(1)
b = time.time()
print(b-a)'''
#Decrypt to get original message
#a = time.time()
E3=AES.AESCipher(key) 
dmivi=E3.decrypt(emivi)
'''time.sleep(1)
b = time.time()
print(b-a)'''
#----------------- Identification --------------------------
#Identify eidvi from PPIDVI
#a = time.time()
H3=hashlib.sha1()
H3.update(ipidvi)
ppidvi1=H3.hexdigest()
'''if(ppidvi==ppidvi1):
    print "vehicle present"
time.sleep(1)
b = time.time()
print(b-a)'''
  
#Decrypt to get original identity
a = time.time() 
E=AES.AESCipher(key)
did_vi=E.decrypt(eid_vi)
time.sleep(1)
b = time.time()
print(b-a)
#print did_vi

'''
print "written"
dict={'q':q,'g1':g1,'g2':g2,'y':y,'e':e,'id_rsu':id_rsu,'signature':sig,
      'yj':yj,'ej':ej,'eid_vi':eid_vi,'VPi':vpi,'Lambda_vi':lambda_vi,'IPID_VI':ipidvi,      
      'f':f,'Toht':toht,'rho_j':rho_j,
      'tohp':tohp,'alphaj':alphaj,'betaj':betaj,
      'hrj':hrj,'rho_j1':rho_j1,'H4f':h4f,'ppidvi':ppidvi,'emivi':emivi,'msgsig':msgsig    
    }

w = csv.writer(open("output.csv", "w"))
for key, val in dict.items():
    w.writerow([key, val]) '''

'''
fp = open("KeyExchange.txt", "r")
key=fp.read()
fp.close()

fp = open("Data.txt", "r")
states1=fp.read()
fp.close()

E=AES.AESCipher(key)
eid_vi=E.encrypt(states1)

fp=open("Encrypted.txt","w")
fp.write(eid)
fp.close()

fp = open("Encrypted.txt", "r")
states2=fp.read()
fp.close()

E1=AES.AESCipher(key)
decrypted4=E1.decrypt(states2) 
print decrypted4

fp=open("Decrypted.txt","w")
fp.write(decrypted4)
fp.close() 
'''

