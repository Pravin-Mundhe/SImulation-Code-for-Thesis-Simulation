import newhope
alice_message = newhope.keygen(True)
bob_message = newhope.sharedb(alice_message)
newhope.shareda(bob_message)
print("Alice's key is ")
print(str(newhope.a_key))
str1 = ''
for e in newhope.a_key:
    str1+=str(e)    
foo = open("KeyExchange.txt", "w")
foo.write(str1)
foo.close()
if newhope.a_key == newhope.b_key:
    print("Successful handshake! Keys match.")
else:
    print("Error! Keys do not match.")
