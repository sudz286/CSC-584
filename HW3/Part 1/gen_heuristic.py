import numpy as np
mat = [[str(j) + ' ' + str(i) for i in range(20)] for j in range(50)]
a = np.array(mat)
a = np.reshape(a, (1000, 1))

import csv
c = open("large_heuristic.csv", "w+")
header = [i for i in range(2)]

writer = csv.DictWriter(c, header)

data = {i: list(a[i]) for i in range(1000)}

for i in range(1000):
    temp = data[i]
    temp = temp[0].split(' ')
    temp = {i:temp[i] for i in range(2)}
    writer.writerow(temp)