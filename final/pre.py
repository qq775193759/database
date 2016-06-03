def add(file, list, output, no):
    while 1:
        line = file.readline()
        if not line:
            break
        a = line.split(',')
        output.write(str(int(a[0])+10000*no))
        output.write(' ')
        output.write(a[2].replace('.',''))
        output.write(' ')
        output.write(a[3].replace('.',''))
        output.write('\n')
    

def read():
    file1 = open('output_0501.dat.plain.txt','r')
    file2 = open('output_0502.dat.plain.txt','r')
    file3 = open('output_0503.dat.plain.txt','r')
    file = open('output_all.dat.plain.txt','w')
    list = []
    add(file1, list, file, 0)
    add(file2, list, file, 1)
    add(file3, list, file, 2)
    
    
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
    
    
    
