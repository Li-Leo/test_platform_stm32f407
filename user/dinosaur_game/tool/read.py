import time

count = 0;

with open("a.txt", "r") as f:
    w = f.read()

    for i in w:
        if(i == ','):
            count += 1
            print(i, end='')
            if count %100 ==0:
                print()
    
    print()



    # print(f.read())
    print(count)


input()