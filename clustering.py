import pandas as pd
import matplotlib.pyplot as plt

cluster0 = pd.read_csv("data/cluster0.out", delimiter=',', header=None)
cluster1 = pd.read_csv("data/cluster1.out", delimiter=',', header=None)
cluster2 = pd.read_csv("data/cluster2.out", delimiter=',', header=None)
centroides = pd.read_csv("data/centroides.out", delimiter=',', header=None)

#rand_set = pd.read_csv("data/rand_set.dat", delimiter=',', header=None)
#ecoli_set_const_10 = pd.read_csv("data/ecoli_set_const_10.const", delimiter=',', header=None)
#ecoli_set_const_20 = pd.read_csv("data/ecoli_set_const_20.const", delimiter=',', header=None)
#cl_set_const_10 = pd.read_csv("data/iris_set_const_10.const", delimiter=',', header=None)
#cl_set_const_20 = pd.read_csv("data/iris_set_const_20.const", delimiter=',', header=None)
#rand_set_const_10 = pd.read_csv("data/rand_set_const_10.const", delimiter=',', header=None)
#rand_set_const_20 = pd.read_csv("data/rand_set_const_20.const", delimiter=',', header=None)

print(cluster0)
print(cluster1)
print(cluster2)
print(centroides)

plt.scatter(cluster0.iloc[:,2],cluster0.iloc[:,3], c='red')
plt.scatter(cluster1.iloc[:,2],cluster1.iloc[:,3], c='green')
plt.scatter(cluster2.iloc[:,2],cluster2.iloc[:,3], c='blue')
plt.scatter(centroides.iloc[:,2],centroides.iloc[:,3],marker='*', s=1000)
plt.show()
