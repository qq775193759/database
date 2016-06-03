def add(file, output, no):
    while 1:
        line = file.readline()
        if not line:
            break
        a = line.split(',')
        next_no = int(a[0])+no
        output.write(str(next_no))
        output.write(' ')
        output.write(a[2].replace('.',''))
        output.write(' ')
        output.write(a[3].replace('.',''))
        output.write('\n')
    return next_no
        
def add_some(file, output, no, num):
    while 1:
        line = file.readline()
        if not line:
            break
        a = line.split(',')
        next_no = int(a[0])+no
        if num == next_no:
            break;
        output.write(str(next_no))
        output.write(' ')
        output.write(a[2].replace('.',''))
        output.write(' ')
        output.write(a[3].replace('.',''))
        output.write('\n')
    return next_no
    

def read():
    file1 = open('output_0501.dat.plain.txt','r')
    file2 = open('output_0502.dat.plain.txt','r')
    file3 = open('output_0503.dat.plain.txt','r')
    file = open('output_all.dat.plain.txt','w')
    next = add(file1, file, -1)
    next = add(file2, file, next)
    add(file3, file, next)
    
    
def read_test():
    file1 = open('output_0501.dat.plain.txt','r')
    file = open('output_test.dat.plain.txt','w')
    add_some(file1, file, -1, 1000)
    
    
def check():
    file = open('output_all.dat.plain.txt','r')
    while 1:
        line = file.readline()
        if not line:
            break
        a = line.split(' ')
        if(len(a[1]) != 8):
            print a[0]
            print a[1]
        if(len(a[2]) != 10):
            print a[0]
            print a[2]
            
read()
    
    
    
