import numpy as np


# Excercise 4 - 10.4.2 (MMDS)
m = np.array([
  [2, -1, -1, 0, 0, 0, 0, 0, 0],
  [-1, 3, -1, 0, 0, 0, 0, -1, 0],
  [-1, -1, 3, -1, 0, 0, 0, 0, 0],
  [0, 0, -1, 3, -1, -1, 0, 0, 0],
  [0, 0, 0, -1, 3, -1, -1, 0, 0],
  [0, 0, 0, -1, -1, 2, 0, 0, 0],
  [0, 0, 0, 0, -1, 0, 3, -1, -1],
  [0, -1, 0, 0, 0, 0, -1, 3, -1],
  [0, 0, 0, 0, 0, 0, -1, -1, 2]
])
print(m)

#eig1 = np.linalg.eig(m)
#print(np.round(eig1[0], 2))
#print(np.round(eig1[1], 2))

eig2 = np.linalg.eigh(m)
#print(np.round(eig2[0], 2))
#print(np.round(eig2[1], 2))
print(np.round(eig2[1][1], 2))
