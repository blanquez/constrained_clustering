import pandas as pd
import random
from sklearn.utils import shuffle

#Usé bucles(a mano) y csv, pandas es el más sencillo y el más elegante

#Lectura de datos

print("Cargando datos...")
#ecoli_set = pd.read_csv("data/ecoli_set.dat", delimiter=',', header=None)
iris_set = pd.read_csv("data/iris_set.dat", delimiter=',', header=None)
#rand_set = pd.read_csv("data/rand_set.dat", delimiter=',', header=None)
#ecoli_set_const_10 = pd.read_csv("data/ecoli_set_const_10.const", delimiter=',', header=None)
#ecoli_set_const_20 = pd.read_csv("data/ecoli_set_const_20.const", delimiter=',', header=None)
iris_set_const_10 = pd.read_csv("data/iris_set_const_10.const", delimiter=',', header=None)
iris_set_const_20 = pd.read_csv("data/iris_set_const_20.const", delimiter=',', header=None)
#rand_set_const_10 = pd.read_csv("data/rand_set_const_10.const", delimiter=',', header=None)
#rand_set_const_20 = pd.read_csv("data/rand_set_const_20.const", delimiter=',', header=None)

k=3
seed=532020
random.seed(seed)
iris_set=shuffle(iris_set)  #Este es el unico shuffle que funciona en matrices
iris_set.reset_index(inplace=True, drop=True)
min_x=int(min(iris_set[0]))
min_y=int(min(iris_set[1]))
max_x=int(max(iris_set[0]))
max_y=int(max(iris_set[1]))

#Generar centroides

centroides = []
c = []
for i in range(0,k):
    centroides.append([random.randint(min_x, max_x), random.randint(min_y, max_y)])
    c.append([])

print(c)

