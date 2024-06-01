import copy
import math, time
from typing import Optional


class Matrix:

    def __init__(self,
                 array: list[list[float]],
                 path: list[int],
                 rem: list[int],
                 lower_bound: float = 0):
        self.array = array
        self.path = path
        self.lower_bound = lower_bound
        self.rem = rem

        for i in range(len(self.array)):
            minim = math.inf
            for cell in self.array[i]:
                if cell < minim:
                    minim = cell

            if minim != math.inf:
                self.lower_bound += minim
                for j in range(len(self.array)):
                    self.array[i][j] -= minim

        for j in range(len(self.array)):
            minim = math.inf
            for i in range(len(self.array)):
                cell = self.array[i][j]
                if cell < minim:
                    minim = cell

            if minim != math.inf:
                self.lower_bound += minim
                for i in range(len(self.array)):
                    self.array[i][j] -= minim

        # print()

    def invalidate_lines(self, array: list[list[float]], vertices: list[int]):
        last = vertices[len(vertices)-1]
        for j in range(len(vertices)):
            if array[last][vertices[j]] != math.inf:
                array[last][vertices[j]] = math.inf

    def add_vertex(self, vertex: int):
        new_array = copy.deepcopy(self.array)
        new_path = copy.deepcopy(self.path)
        new_path.append(vertex)
        new_lower = self.lower_bound
        if len(new_path) >= 2:
            length = len(new_path)
            a = new_path[length-2]
            b = new_path[length-1]
            new_lower += new_array[a][b]
            for i in range(len(new_array)):
                if new_array[a][i] != math.inf:
                    new_array[a][i] = math.inf
                if new_array[i][b] != math.inf:
                    new_array[i][b] = math.inf

        self.invalidate_lines(new_array, new_path)

        new_rem = copy.copy(self.rem)
        new_rem.remove(vertex)

        return Matrix(new_array, new_path, new_rem, new_lower)

class BinHeap:
    def __init__(self):
        self.__storage = list()

    def __up_heapify(self, i):
        current = i
        while True:
            parent = (current - 1) // 2
            if parent < 0:
                return
            if self.__storage[current][0] < self.__storage[parent][0]:
                self.__storage[current], self.__storage[parent] = self.__storage[parent], self.__storage[current]
                current = parent
            else:
                return

    def __down_heapify(self, i):
        current = i
        while True:
            left = current * 2 + 1
            right = current * 2 + 2
            largest = current
            if left < len(self.__storage) and self.__storage[left][0] < self.__storage[largest][0]:
                largest = left
            if right < len(self.__storage) and self.__storage[right][0] < self.__storage[largest][0]:
                largest = right
            if largest != current:
                self.__storage[largest], self.__storage[current] = self.__storage[current], self.__storage[largest]
                current = largest
            else:
                return

    def insert(self, key: float, element):
        self.__storage.append((key, element))
        self.__up_heapify(len(self.__storage) - 1)

    def insert_pair(self, pair: (float, object)):
        self.__storage.append(pair)
        self.__up_heapify(len(self.__storage) - 1)

    def delete(self, key: float):
        i = self.__search_index(key)
        if i == -1:
            return
        self.delete_by_index(i)

    def delete_by_index(self, i: int):
        if i >= len(self.__storage):
            return
        if i == len(self.__storage) - 1:
            self.__storage.pop(len(self.__storage) - 1)
            return
        key = self.__storage[i][0]
        self.__storage[i] = self.__storage.pop(len(self.__storage) - 1)
        if self.__storage[i][0] < key:
            self.__up_heapify(i)
        if self.__storage[i][0] > key:
            self.__down_heapify(i)

    def __search_index(self, key: float) -> int:
        for i in range(len(self.__storage)):
            if self.__storage[i][0] == key:
                return i
        return -1

    def find(self, key: float) -> Optional[tuple[int, object]]:
        i = self.__search_index(key)
        if i != -1:
            return self.__storage[i]
        else:
            return None

    def find_max(self) -> Optional[tuple[int, object]]:
        if len(self.__storage) != 0:
            return self.__storage[0]
        else:
            return None

    def pop_max(self) -> Optional[tuple[int, Matrix]]:
        if len(self.__storage) == 0:
            return None
        maximum = self.__storage[0]
        self.delete_by_index(0)
        return maximum

    def layers(self):
        margin = 1
        add = 2
        out = ""
        for i in range(len(self.__storage)):
            if i == margin:
                out += "\n"
                margin += add
                add *= 2
            out += f" {self.__storage[i]} "
        return str(out)

    def empty(self) -> bool:
        return len(self.__storage) == 0


# file = open("input.txt", "r")
# dataset = "3\n0 8 86\n47 0 16\n87 54 0\n"
# lines = dataset.strip().split("\n")
# lines = file.readlines()
n = int(input())

array = [0] * n

for i in range(n):
    array[i] = list(map(lambda x: float(x), input().split(" ")))
    array[i][i] = math.inf

#start = time.time_ns()

matrix = Matrix(copy.deepcopy(array), [], [i for i in range(n)])
matrix = matrix.add_vertex(0)

queue = BinHeap()
queue.insert(matrix.lower_bound, matrix)

curr: Matrix
bestSolution = Matrix(copy.deepcopy(array), [], [], math.inf)

while not queue.empty():
    curr = queue.pop_max()[1]

    if curr.lower_bound >= bestSolution.lower_bound:
        break

    if len(curr.rem) == 0:
        bestSolution = curr
        continue

    for vertex in curr.rem:
        add = curr.add_vertex(vertex)
        queue.insert(add.lower_bound, add)

best_path = bestSolution.path

#end = time.time_ns()

print(int(sum([array[best_path[i-1]][best_path[i]] for i in range(1, len(best_path))])) +
      int(array[best_path[len(best_path)-1]][best_path[0]]))
print(" ".join(list(map(lambda x: str(x+1), best_path)) + [str(best_path[0]+1)]))
#print(end-start)
