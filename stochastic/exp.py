#%% Import the necessary libraries and generate the samples.
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from math import exp


n_experiments = 10000
scale_1 = 1 / 5  # lambda = 5
scale_2 = 2  # lambda = 1/2

print('Experiments:\t{}'.format(n_experiments))
print('X1 ~ Exp({})'.format(1 / scale_1))
print('X2 ~ Exp({})'.format(1 / scale_2))

# Generate the samples.
sample_1 = np.random.exponential(scale=scale_1, size=n_experiments)
sample_2 = np.random.exponential(scale=scale_2, size=n_experiments)

#%% Basic properties.
# Check the probability of X1 < X2.
sample_1_lt_2 = sample_1 < sample_2
count = sample_1_lt_2.sum()

print('Number of times X1 < X2')
print('Expected:', '\t', (1 / scale_1) / (1 / scale_1 + 1 / scale_2) * n_experiments)
print('Actual:', '\t', count)

# Check graphically whether min{X1, X2} ~ Exp(X1 + X2).
sample_min = np.minimum(sample_1, sample_2)
sample_min_expected = np.random.exponential(scale=(1 / (1 / scale_1 + 1 / scale_2)), size=n_experiments)

sns.kdeplot(sample_min, label=r'min$\{X_1, X_2\}$')
sns.kdeplot(sample_min_expected, label=r'$Y$')

plt.xlim(0, 1)
plt.legend(loc='best')

param_text = '\n'.join((
    r'$X_1 \sim Exp(%.2f)$' % (1 / scale_1, ),
    r'$X_2 \sim Exp(%.2f)$' % (1 / scale_2, ),
    r'$Y \sim Exp(%.2f + %.2f)$' % (1 / scale_1, 1 / scale_2)))
ax = plt.gca()
plt.text(0.7, 0.5, param_text,
         verticalalignment='top', transform=ax.transAxes,
         bbox={'boxstyle': 'round', 'facecolor': 'white'})
plt.show()


#%% Ex.1.2
print('EXERCISE 1.2')
z = sample_1
for T in [0.1, 0.2, 0.25, 0.3]:
    x = (z // T + 1) * T
    y = x - z
    Ex_expected = T / (1 - exp(-1 / scale_1 * T))
    Ey_expected = Ex_expected - scale_1
    Ex_actual = x.sum() / x.size
    Ey_actual = y.sum() / y.size
    print('For T =\t', T)
    print('E[X] expected:\t', Ex_expected)
    print('E[X] actual:\t', Ex_actual)
    print('E[X] error:\t', (Ex_expected - Ex_actual) / Ex_actual * 100, '%')
    print('E[Y] expected:\t', Ey_expected)
    print('E[Y] actual:\t', Ey_actual)
    print('E[Y] error:\t', (Ey_expected - Ey_actual) / Ey_actual * 100, '%')


#%% Ex.1.3
print('EXERCISE 1.3')
sample1_2 = np.random.exponential(scale=scale_1, size=n_experiments)
count = (np.maximum(sample_1, sample1_2) < sample_2).sum()
prob_expected = 1 - 2 / scale_2 / (1 / scale_2 + 1 / scale_1) + 1 / scale_2 / (1 / scale_2 + 2 / scale_1)

print('Expected occurrences:', prob_expected * n_experiments)
print('Actual occurrences:', count)
print('Error:', (prob_expected * n_experiments - count) / count * 100, '%')
