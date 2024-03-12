import pandas as pd 
import numpy as np

data1 = pd.read_csv("../tiempos2.csv",names=["Value","Time"], header=0)
data2 = pd.read_csv("../tiempos4.csv", names=["Value", "Time"], header=0)
tiempos1 = data1["Time"].values
tiempos2 = data2["Time"].values
suma1 = np.sum(tiempos1)
suma2 = np.sum(tiempos2)
print(suma1+suma2)




