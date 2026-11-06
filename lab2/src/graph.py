import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("data/result.csv", delimiter=",")
threads_string = data[::3, 0]
times_string = data[::3, 1]

threads_column = data[1::3, 0]
times_column = data[1::3, 1]

threads_block = data[2::3, 0]
times_block = data[2::3, 1]

plt.figure(figsize=(12,8))
plt.plot(threads_string, times_string, marker="o", markersize=8, linewidth=2, label="Ленточное разбиение по строкам")
plt.plot(threads_column, times_column, marker="o", markersize=8, linewidth=2, label="Ленточное разбиение по столбцам")
plt.plot(threads_block, times_block, marker="o", markersize=8, linewidth=2, label="Ленточное разбиение по блокам")
plt.title("Dependence of execution time on the number of threads")
plt.xlabel("Num threads")
plt.ylabel("Time(s)")
plt.xticks(threads_string)
plt.grid(True)
plt.legend()
plt.show()