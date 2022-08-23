import csv
import random
fh = open("graph.txt", "r")
c = open("large_graph.csv", "w+")
header = [i for i in range(1000)]
writer = csv.DictWriter(c, header)

count = 0
mat = [[0 for i in range(1000)] for i in range(1000)]

# data = dict({i:str(0) for i in range(1000)})
index = 0
while True:
    count += 1
    line = fh.readline()

    if count <= 3:
        continue

    if not line:
        break

    # print("Line{}: {}".format(count, line.strip()))
    # data.append(line.strip())
    line = line.strip()
    line = set(line.split(' '))


    for i in range(1000):
        if str(i) not in line:
            continue
        else:
            num = random.randint(1,100)
            mat[index][i] = str(num)
            mat[i][index] = str(num)
    index += 1
    
    # writer.writerow(data)

data = dict({i:str(0) for i in range(1000)})
    
for i in range(1000):
    for j in range(1000):
        data[j] = str(mat[i][j])
    writer.writerow(data)
    data = dict({i:str(0) for i in range(1000)})
    





# for d in data:
#     line = set(d.split(' '))
#     for i in range(1000):
#         if str(i) not in line:
#             print("inf", end = '')
#         else:
#             print(i,end = '')
#     print('')


# with open("large_graph.csv", "w") as csv:
#     for i in range(1000):
#         csv.write(str(i))
#         csv.write('\n')

