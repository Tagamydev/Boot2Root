from itertools import permutations

# Generate and print all permutations of the numbers 1 to 6
for perm in permutations([1, 2, 3, 4, 5, 6]):
    print(' '.join(map(str, perm)))
