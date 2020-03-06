import numpy as np
#from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

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
dimension=len(cl_set[1])
seed=532020
np.random.seed(seed)
#cl_set=shuffle(cl_set, random_state=seed)  #Este es el unico shuffle que funciona en matrices
#cl_set.reset_index(inplace=True, drop=True)  #Cuidado con los shuffles, que las restricciones tb

#Minimos y maximos

minimos = np.amin(cl_set, axis=0)
maximos = np.amax(cl_set, axis=0)
rango = np.subtract(maximos,minimos)

#Generar centroides y clusters

#c = []
centroides = (rango*np.random.random((k,dimension)))+minimos
print(centroides)

    
#for i in range(0,k):
#    c.append([])

#Algoritmo k-medias

#kmeans = KMeans(n_clusters=k).fit(cl_set)
#labels = kmeans.predict(cl_set)
#centroid = kmeans.cluster_centers_

while True:
    c = np.argmin(((cl_set[:, :, None] - centroides.T[None, :, :])**2).sum(axis=1), axis=1)
    nuevos_centroides = np.array([cl_set[c == j, :].mean(axis=0) for j in range(k)])
    if(nuevos_centroides == centroides).all:
        break
    else:
        centroides = nuevos_centroides

colores=['red','green','blue']
asignar=[]

for row in c:
        asignar.append(colores[row])
print(centroides)

plt.scatter(cl_set[:,2],cl_set[:,3],c=asignar)
plt.scatter(centroides[:,2],centroides[:,3],marker='*', c=colores, s=1000)
plt.show()
#ax = Axes3D(fig)

#def recalcular_centroides(c):
#    for i in k:
#        centroides[i] = np.linalg.norm(c[i])
#    return centroides

#def infeasibility
