import numpy as np
import random
from sklearn.utils import shuffle

#Use bucles(a mano) y csv, pandas es el mas sencillo y el mas elegante

#Lectura de datos

print("Cargando datos...")
#ecoli_set = pd.read_csv("data/ecoli_set.dat", delimiter=',', header=None)
#cl_set = pd.read_csv("data/iris_set.dat", delimiter=',', header=None)

cl_set = np.loadtxt("data/iris_set.dat", delimiter=',')

#rand_set = pd.read_csv("data/rand_set.dat", delimiter=',', header=None)
#ecoli_set_const_10 = pd.read_csv("data/ecoli_set_const_10.const", delimiter=',', header=None)
#ecoli_set_const_20 = pd.read_csv("data/ecoli_set_const_20.const", delimiter=',', header=None)
#cl_set_const_10 = pd.read_csv("data/iris_set_const_10.const", delimiter=',', header=None)
#cl_set_const_20 = pd.read_csv("data/iris_set_const_20.const", delimiter=',', header=None)
#rand_set_const_10 = pd.read_csv("data/rand_set_const_10.const", delimiter=',', header=None)
#rand_set_const_20 = pd.read_csv("data/rand_set_const_20.const", delimiter=',', header=None)

k=3
dimension=4
seed=532020
random.seed(seed)
#cl_set=shuffle(cl_set, random_state=seed)  #Este es el unico shuffle que funciona en matrices
#cl_set.reset_index(inplace=True, drop=True)  #Cuidado con los shuffles, que las restricciones tb

#Minimos y maximos

minimos = []
maximos = []

for i in range(0,dimension):
    minimos.append(int(min(cl_set[i])))
    maximos.append(int(max(cl_set[i])))

#Generar centroides

centroides = []
c = []

for i in range(0,k):
    aux = []
    for j in range(0,dimension):
        aux.append(random.randint(minimos[j], maximos[j]))
    centroides.append(aux.copy())
    c.append([])

#Funciones

def recalcular_centroides(c, cl_set):
    centroides = []
    for i in range(0,len(c)):
        centroides[i] = c[i].mean()

print(cl_set)

