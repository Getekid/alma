import numpy as np


# Helper functions

# Calculate the c-RMSD between two coordinate sets.
def c_rmsd(x, y):
    # calculates c-RMSD for x,y sets of 3D coordinates.
    n = np.shape(x)[0]
    x_c = np.mean(x, axis=0)
    y_c = np.mean(y, axis=0)
    x = x - x_c
    y = y - y_c
    u, sig, vt = np.linalg.svd(x.transpose() @ y)
    q = u @ vt
    if np.linalg.det(q) < 0:
        q = np.array([u[:, 0], u[:, 1], -u[:, 2]]) @ vt
    return np.linalg.norm((x @ q) - y) / np.sqrt(n)


# Calculate the Gram matrix G from a set of coordinates and a distant matrix.
def gram(x, dist):
    g = np.full((9, 9), 0.0)
    for i in range(9):
        for j in range(9):
            g[i, j] = (np.linalg.norm(x[i]) ** 2 + np.linalg.norm(x[j]) ** 2) / 2 - dist[i + 1, j + 1]
    # Similar way of calculating G using only the coordinates.
    # for i in range(9):
    #     for j in range(9):
    #         g[i, j] = x[i].transpose() @ x[j]
    return g


# Calculate the Gram matrix G from a set of coordinates and a distant matrix.
def gram_cutoff(x, dist, t):
    g = np.full((9, 9), 0.0)
    d = 0
    for i in range(9):
        for j in range(9):
            if dist[i + 1, j + 1] > t:
                d = dist[i + 1, j + 1]
            g[i, j] = (np.linalg.norm(x[i]) ** 2 + np.linalg.norm(x[j]) ** 2) / 2 - d
    return g


def get_three_max(array):
    a = np.sort(array)
    return np.array([a[-1], a[-2], a[-3]])


# Exercise C

# Take the coordinates from the 3 backbone atoms (N - CA - C)
# of the three residues of the calcium binding site, i.e. Val276-Ala277-Asp278.
coord = np.array([
    [19.456, 43.867, 53.820],  # ATOM   1818  N   VAL A 276
    [19.120, 44.932, 52.867],  # ATOM   1819  CA  VAL A 276
    [19.180, 46.289, 53.576],  # ATOM   1820  C   VAL A 276
    [20.251, 47.037, 53.302],  # ATOM   1825  N   ALA A 277
    [20.462, 48.366, 53.881],  # ATOM   1826  CA  ALA A 277
    [19.599, 49.411, 53.172],  # ATOM   1827  C   ALA A 277
    [18.326, 49.468, 53.563],  # ATOM   1830  N   ASP A 278
    [17.345, 50.411, 52.999],  # ATOM   1831  CA  ASP A 278
    [16.733, 51.352, 54.051],  # ATOM   1832  C   ASP A 278
])

# Calculate the Cayley-Menger (border) matrix B.
B = np.full((10, 10), 1.0)
B[0][0] = 0.0
for i in range(1, 10):
    for j in range(1, 10):
        B[i, j] = (np.linalg.norm(coord[i-1] - coord[j-1])**2) / 2

print(B)

# Exercise D

# Check that rank(B) = 5.
rankB = np.linalg.matrix_rank(B)
print(rankB)

# Calculate the Gram matrix G and then use SVD to generate the 3D point coordinates.
G = gram(coord, B)
print(G)
v, sigma, vh = np.linalg.svd(G)
new_coord = np.sqrt(np.diag(sigma[:3])) @ vh[:3, :]
print(new_coord.transpose())
print(c_rmsd(coord, new_coord.transpose()))

# Exercise E

# Alter the linear space start, end and number of points to adjust the search.
for pert in np.linspace(0.0, 1.0, 101):
    Bp = B * pert
    G = gram(coord, Bp)
    v, sigma, vh = np.linalg.svd(G)
    s = get_three_max(sigma)
    new_coord = np.sqrt(np.diag(s)) @ vh[:3, :]
    print('For perturbation ', pert, ' we get c-RMSD: ', c_rmsd(coord, new_coord.transpose()))
# c-RMSD < 1 is achieved for max perturbation ~88,61%.

# Exercise F
for T in [30, 20, 15, 10, 5, 4, 3, 2, 1]:
    G = gram_cutoff(coord, B, T)
    v, sigma, vh = np.linalg.svd(G)
    s = get_three_max(sigma)
    new_coord = np.sqrt(np.diag(s)) @ vh[:3, :]
    print('For cutoff ', T, ' we get c-RMSD: ', c_rmsd(coord, new_coord.transpose()))
