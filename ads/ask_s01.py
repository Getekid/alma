# Exercise 1
import math
import numpy as np


class Multistage:
    # Class implementing the Multistage algorithm.

    def __init__(self, baskets, support_threshold):
        # Initiate the class
        self.baskets = baskets
        self.support_threshold = support_threshold

        # Count the support for the singletons and the pairs.
        self.singletons_support = [0 for i in range(6)]
        self.pairs_support = [0 for j in range(15)]
        # Also hash the pairs into buckets.
        self.hash_buckets = [0 for k in range(11)]
        self.hash_buckets_multistage = [0 for m in range(9)]

    @property
    def singletons_support(self):
        return self._singletons_support

    @singletons_support.setter
    def singletons_support(self, value):
        self._singletons_support = value

    @property
    def pairs_support(self):
        return self._pairs_support

    @pairs_support.setter
    def pairs_support(self, value):
        self._pairs_support = value

    @property
    def hash_buckets(self):
        return self._hash_buckets

    @hash_buckets.setter
    def hash_buckets(self, value):
        self._hash_buckets = value

    @property
    def hash_buckets_multistage(self):
        return self._hash_buckets_multistage

    @hash_buckets_multistage.setter
    def hash_buckets_multistage(self, value):
        self._hash_buckets_multistage = value

    # Get the index using the Triangular-Matrix method.
    @staticmethod
    def triangular_matrix_index(i, j):
        k = (i - 1) * (6 - i / 2) + j - i
        return int(k)

    # Calculate the hash for the first pass.
    @staticmethod
    def hash_first_pass(pair):
        return (pair[0] * pair[1]) % 11

    # Calculate the hash for the second pass.
    @staticmethod
    def hash_second_pass(pair):
        return (pair[0] + pair[1]) % 9

    # Given a list with counts, return a boolean list of whether the respective item is frequent.
    def get_is_frequent(self, count_list):
        is_frequent = []
        for count in count_list:
            is_frequent.append(True if count >= self.support_threshold else False)
        return is_frequent

    def run(self):
        # First pass.
        for basket in self.baskets:
            for i in range(3):
                item = basket[i]
                index = item - 1
                self.singletons_support[index] += 1
                # Store the two pair counts using the Triangular-Matrix method.
                for j in range(i+1, 3):
                    item2 = basket[j]
                    triangular_index = self.triangular_matrix_index(item, item2) - 1
                    # print(item, item2, triangular_index)
                    self.pairs_support[triangular_index] += 1

                    # But also map the pair to the hash table.
                    hash_index = self.hash_first_pass([item, item2])
                    # print('Items', f'({item},{item2})', 'map to bucket', hash_index)
                    self.hash_buckets[hash_index] += 1

        # Second pass.
        singletons_is_freq = self.get_is_frequent(self.singletons_support)
        hash_buckets_is_freq = self.get_is_frequent(self.hash_buckets)
        for basket in self.baskets:
            for i in range(3):
                for j in range(i+1, 3):
                    if singletons_is_freq[basket[i] - 1] and singletons_is_freq[basket[j] - 1]\
                            and hash_buckets_is_freq[self.hash_first_pass([basket[i], basket[j]])]:
                        self.hash_buckets_multistage[self.hash_second_pass([basket[i], basket[j]])] += 1


# Exercise 6.3.1
# Dataset and requirements from Exercise 6.3.1.
items = range(1, 7)
data = [
    [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6],
    [1, 3, 5], [2, 4, 6], [1, 3, 4], [2, 4, 5],
    [3, 5, 6], [1, 2, 4], [2, 3, 5], [3, 4, 6],
]
mg = Multistage(data, 4)
mg.run()

# Question (a).
print(mg.singletons_support)
print(mg.pairs_support)

# Question (c).
print(mg.hash_buckets)

# Exercise 6.3.2
print(mg.hash_buckets_multistage)


# Exercise 4
print()
# Exercise 5.3.1
print('Exercise 5.3.1')
trans = np.array([
    [0, 2/5, 4/5, 0],
    [4/15, 0, 0, 2/5],
    [4/15, 0, 0, 2/5],
    [4/15, 2/5, 0, 0]
])

# Question (a)
print('(a)')
taxation = np.array([1/5, 0, 0, 0])
v = np.array([1, 0, 0, 0])
for i in range(1000):
    v = (trans @ v) + taxation
print(v)

# Question (b)
print('(b)')
taxation = np.array([1/10, 0, 1/10, 0])
v = np.array([1, 0, 1, 0])
for i in range(1000):
    v = (trans @ v) + taxation
print(v)

# Exersice 5.3.2
print()
print('Exersice 5.3.2 (a)')
taxation = np.array([0, 1/5, 0, 0])
v = np.array([0, 1, 0, 0])
for i in range(1000):
    v = (trans @ v) + taxation
print(v)

# Exercise 5
print()


def d2(point_a, point_b):
    return math.sqrt((point_a[0] - point_b[0]) ** 2 + (point_a[1] - point_b[1]) ** 2)


data = [(4, 10), (7, 10), (4, 8), (6, 8), (12, 6), (10, 5), (3, 4), (11, 4), (9, 3), (12, 3), (2, 2), (5, 2)]

# Exercise 7.2.2
print('Exercise 7.2.2')
format_row = "{:>8}" * (len(data) + 1)
print(format_row.format('', *list(map(lambda x: str(x), data))))
for point_row in data:
    distances = []
    for point_col in data:
        distances.append(round(d2(point_col, point_row), 2))
    print(format_row.format(str(point_row), *distances))


# Exercise 7.2.3
class Cluster:
    def __init__(self, points):
        self.points = list(set(points))
        self.centroid = self.get_centroid()

    def get_centroid(self):
        x = (sum([point[0] for point in self.points])) / len(self.points)
        y = (sum([point[1] for point in self.points])) / len(self.points)
        return x, y

    def get_radius(self):
        radii = [d2(self.centroid, point) for point in self.points]
        return max(radii)

    def get_diameter(self):
        diameter = 0
        for point_a in self.points:
            for point_b in self.points:
                if point_a == point_b:
                    continue
                d = d2(point_a, point_b)
                diameter = d if d > diameter else diameter
        return diameter

    def __repr__(self):
        return str(self.points)


class HClustering:
    def __init__(self, points):
        self.points = points
        self.clusters = [Cluster([point]) for point in points]
        self.criterion = ''

    def cluster_step(self):
        radius_min = 1000
        diameter_min = 1000
        merge = {}
        for cluster_a in self.clusters:
            for cluster_b in self.clusters:
                if cluster_a == cluster_b:
                    continue
                cluster_new = Cluster(cluster_a.points + cluster_b.points)

                if self.criterion == 'radius':
                    radius = cluster_new.get_radius()
                    if radius < radius_min:
                        merge = {'a': cluster_a, 'b': cluster_b, 'new': cluster_new}
                        radius_min = radius
                elif self.criterion == 'diameter':
                    diameter = cluster_new.get_diameter()
                    if diameter < diameter_min:
                        merge = {'a': cluster_a, 'b': cluster_b, 'new': cluster_new}
                        diameter_min = diameter

        criterion_value = radius_min if self.criterion == 'radius' else diameter_min
        criterion_value = round(criterion_value, 2)
        print('Merging clusters {0} and {1}. New cluster has centroid {2} and {3} {4}'.format(
            merge['a'].points, merge['b'].points, merge['new'].get_centroid(), self.criterion, criterion_value))

        self.clusters.remove(merge['a'])
        self.clusters.remove(merge['b'])
        self.clusters.append(merge['new'])

    def cluster(self):
        while len(self.clusters) > 3:
            self.cluster_step()


print()
print('Exercise 7.2.3')

print('(a) - radius')
hcl = HClustering(data)
hcl.criterion = 'radius'
hcl.cluster()
print('Final clusters:', hcl.clusters)

print('(b) - diameter')
hcl = HClustering(data)
hcl.criterion = 'diameter'
hcl.cluster()
print('Final clusters:', hcl.clusters)
