import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("data/result.csv", delimiter=",")
threads = data[:, 0]
times = data[:, 1]

plt.figure(figsize=(12,8))
plt.plot(threads, times, marker="o", markersize=8, linewidth=2, label="Время")
plt.title("Dependence of execution time on the number of threads")
plt.xlabel("Num threads")
plt.ylabel("Time(s)")
plt.xticks(threads)  
plt.grid(True)
plt.show()