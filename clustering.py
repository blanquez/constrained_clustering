import csv

ecoli_set=[]
iris_set=[]
rand_set=[]
ecoli_set_const_10=[]
ecoli_set_const_20=[]
iris_set_const_10=[]
iris_set_const_20=[]
rand_set_const_10=[]
rand_set_const_20=[]

f = open('data/ecoli_set.dat', 'r')
reader = csv.reader(f)
for row in reader:
    ecoli_set.append(row)
f.close()

f = open('data/ecoli_set_const_10.dat', 'r')
reader = csv.reader(f)
for row in reader:
    ecoli_set_const_10.append(row)
f.close()

f = open('data/ecoli_set_const_20.dat', 'r')
reader = csv.reader(f)
for row in reader:
    ecoli_set_const_20.append(row)
f.close()

f = open('data/iris_set.dat', 'r')
reader = csv.reader(f)
for row in reader:
    iris_set.append(row)
f.close()

f = open('data/iris_set_const_10.dat', 'r')
reader = csv.reader(f)
for row in reader:
    iris_set_const_10.append(row)
f.close()

f = open('data/iris_set_const_20.dat', 'r')
reader = csv.reader(f)
for row in reader:
    iris_set_const_20.append(row)
f.close()

f = open('data/rand_set.dat', 'r')
reader = csv.reader(f)
for row in reader:
    rand_set.append(row)
f.close()

f = open('data/rand_set_const_10.dat', 'r')
reader = csv.reader(f)
for row in reader:
    rand_set_const_10.append(row)
f.close()

f = open('data/rand_set_const_20.dat', 'r')
reader = csv.reader(f)
for row in reader:
    rand_set_const_20.append(row)
f.close()

print(ecoli_set)

