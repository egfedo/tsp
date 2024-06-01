import itertools, math, time

n = int(input())

array = [0] * n

for i in range(n):
    array[i] = list(map(lambda x: float(x), input().split(" ")))
    array[i][i] = math.inf

min_path = math.inf
min_path_vector = []

path = [i for i in range(n)]

start = time.time_ns()

for perm in itertools.permutations(path):
    path_len = 0.0
    for i in range(1, n):
        path_len += array[perm[i-1]][perm[i]]
    path_len += array[perm[n-1]][perm[0]]

    if path_len < min_path:
        min_path = path_len
        min_path_vector = perm

end = time.time_ns()

print(int(min_path))
print(" ".join(map(lambda x: str(x + 1), min_path_vector + min_path_vector[0:1])))
# print(end - start)
