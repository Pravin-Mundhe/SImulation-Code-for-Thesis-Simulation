def generator_of_group(p):
        if check_prime(p)==0:
            print "Input is not prime\n"
            return
        generators=[]
        b=[]
        for i in range(p-1):
            c=[]
            for j in range(p-1):
                temp=pow(i,j,p)
                if temp not in c:
                    c.append(temp)
            b.append(c)
        for i in range(len(b)):
            if len(b[i])==p-1:
                generators.append(i)
        return generators

def check_prime(n):
    count=0
    for i in range(1,n+1):
       if n%i==0:
           count+=1
    if count>2:
       return 0 
    else:
       return 1
       
#print generator_of_group(631)
