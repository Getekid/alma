# Exercise 1


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

