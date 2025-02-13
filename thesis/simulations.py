import numpy as np

"""This script runs some basic simulations based on pre-calculated
    best response functions from heterogeneous parallel network games.
    
    For a more feature-rich implementation of those games including
    calculating best response functions or tolled flow on basic
    distribution functions, refer to the Parallel-Games library at:
    https://github.com/Getekid/Parallel-Games
    https://github.com/Getekid/Parallel-Games/blob/demo/demo.ipynb (Demo)
"""


def run(best_response_1, best_response_2, n_experiments=20, n_rounds=30):
    """Runs some basic simulations based on pre-calculated best response functions.

    Args:
        best_response_1 (callable): The best response function from link operator 1.
        best_response_2 (callable): The best response function from link operator 2.
        n_experiments (int): The number of experiments to run.
        n_rounds (int): The number of rounds to simulate. In each round,
            all link operators take turns and change their toll to their best response one.

    Returns:
        (np.array): A list of the tolls played in each round.
            If an equilibrium exists, the tolls sequences should converge to it.
    """
    # We only need to initialise tolls for link operator 2,
    # as 1 will immediately update theirs with their best response.
    tolls_init_2 = np.random.uniform(0, 10, n_experiments)

    tolls = np.zeros((n_experiments, n_rounds, 2))
    for e in range(n_experiments):
        for r in range(n_rounds):
            tolls[e, r, 0] = best_response_1(tolls[e, r - 1, 1] if r > 0 else tolls_init_2[e])
            tolls[e, r, 1] = best_response_2(tolls[e, r, 0])

    return tolls


class Example68Plus1:
    """This class contains the best response functions from the variation of Example 6.8
        in the thesis article, when using distribution function a(p) = p + 1.

    The game should have a Nash Equilibrium at ~(1.1464, 0.9268).
    """

    @staticmethod
    def best_response_1(t2):
        if t2 < 1.2:
            return t2 - 3 + np.sqrt(5 * (3 - t2))
        elif t2 <= 1.5:
            return t2
        elif t2 < 5:
            return t2 + 3 - np.sqrt(2 * t2 + 6)
        else:
            return t2 - 1

    @staticmethod
    def best_response_2(t1):
        if t1 <= (2 / 9) * np.sqrt(22 - 4 * np.sqrt(10)):
            return t1 - 3 + 2 * np.sqrt(3 - t1)
        elif t1 < 7:
            return t1 + 3 - (np.sqrt(10) / 2) * np.sqrt(t1 + 3)
        else:
            return t1 - 2


class Example69:
    """This class contains the best response functions from Example 6.9 in the thesis article,
        when using step distribution function a(p) = 1 (p <= 1/2), 5 (otherwise).

    The game should have no Nash Equilibrium.
    """

    @staticmethod
    def best_response_1(t2):
        if t2 <= 1:
            return (t2 + 2) / 2
        elif t2 < 1.5:
            return t2 + 0.5
        elif t2 <= 2:
            return t2
        elif t2 < 4:
            return (t2 + 2) / 2
        else:
            return t2 - 1

    @staticmethod
    def best_response_2(t1):
        if t1 <= 0.2:
            return (t1 + 1) / 2
        elif t1 < 0.4:
            return (5 * t1 + 1) / 10
        elif t1 <= 2 + 2 * np.sqrt(15) / 5:
            return t1 - 1 / 10
        elif t1 < 5:
            return (t1 + 1) / 2
        else:
            return t1 - 2


if __name__ == "__main__":
    print("-" * 40)
    print("Running Example 6.8 with a(p) = p + 1.")
    print("-" * 40)
    print(run(Example68Plus1.best_response_1, Example68Plus1.best_response_2))
    print("-" * 40)
    print("Running Example 6.9.")
    print("-" * 40)
    print(run(Example69.best_response_1, Example69.best_response_2))
