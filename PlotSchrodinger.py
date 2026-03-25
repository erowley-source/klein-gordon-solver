import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd
import os

# norme d'un nombre complexe
def norm(c):
    return np.sqrt(c[0]**2+c[1]**2)
print(os.getcwd())
df = pd.read_csv("/users/etu/21335179/Projet/solution_temporelle.dat", sep = " ")
df.columns = [i for i in range(len(df.columns))]
df.drop(len(df.columns)-1,axis=1,inplace=True)
array = df.to_numpy()

# convertit les données en paire de float
def parse(entry):
    entry = entry[1:-1].split(",")
    entry = [float(s) for s in entry]
    return entry

print(parse(array[0,0]),norm(parse(array[0,0])))



psi_2 = [[norm(parse(array[j,i])) for i in range(len(array[0]))] for j in range(len(array))]

fig, ax = plt.subplots()

x = np.linspace(0,10,201)
line, = ax.plot(x, psi_2[0])
ax.set(xlim=[0, 10], ylim=[0, 1], xlabel='Position [m]', ylabel=r'$|\Psi|^{2}$')
ax.bar(x=4.5,height=1,width=0.02,color="red")
ax.bar(x=5.5,height=1,width=0.02,color="red")

def update(frame):
    # update the line plot
    line.set_ydata(psi_2[frame])
    return (line,)


ani = FuncAnimation(fig=fig, func=update, frames=len(psi_2), interval=10,blit=True)
plt.show()

ani.save("schrodinger1.mp4")

#n=1000

#psi_2d = np.zeros((201,201))
#for i in range(len(psi_2[n])):
#    for j in range(len(psi_2[n])):
#        psi_2d[i][j] = psi_2[n][i]*psi_2[n][j]
#plt.imshow(psi_2d)
#ax.imshow(psi_2)
#plt.plot(psi_2[7])

