import copy
import math, time
from typing import Optional


class Matrix:

    def __init__(self,
                 array: list[list[float]],
                 lines: list[tuple[int, int]],
                 lower_bound: float = 0,
                 cell_amount: int = -1):
        self.array = array
        self.lines = lines
        self.lower_bound = lower_bound
        if cell_amount < 0:
            cell_amount = len(array) * (len(array) - 1)
        self.cell_amount = cell_amount

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

    def calc_delta(self, x: int, y: int) -> float:
        min_row = math.inf
        min_col = math.inf

        for i in range(len(self.array)):
            if i != y:
                if self.array[x][i] < min_row:
                    min_row = self.array[x][i]
            if i != x:
                if self.array[i][y] < min_col:
                    min_col = self.array[i][y]

        if min_row == math.inf:
            min_row = 0.0
        if min_col == math.inf:
            min_col = 0.0

        return min_row + min_col

    def find_variant(self) -> tuple[int, int]:
        output = (-1, -1)
        max_delta = -1

        for i in range(len(self.array)):
            for j in range(len(self.array)):
                if self.array[i][j] == 0:
                    delta = self.calc_delta(i, j)
                    if delta > max_delta:
                        max_delta = delta
                        output = (i, j)

        return output

    def invalidate_lines(self, array: list[list[float]], lines: list[tuple[int, int]]):
        inv_amt = 0
        chain = [lines[len(lines)-1]]
        start_idx = 0

        while True:
            found = False
            for i in range(len(lines)):
                if lines[i][0] == chain[len(chain)-1][1]:
                    chain.append(lines[i])
                    found = True
                if lines[i][1] == chain[0][0]:
                    chain.insert(0, lines[i])
                    found = True
                    start_idx += 1
            if not found:
                break

        for a in range(start_idx, -1, -1):
            for b in range(start_idx, len(chain)):
                if array[chain[b][1]][chain[a][0]] != math.inf:
                    array[chain[b][1]][chain[a][0]] = math.inf
                    inv_amt += 1

        return inv_amt

    def add_line(self, line: tuple[int, int]):
        new_array = copy.deepcopy(self.array)

        inv_amt = 0

        for i in range(len(new_array)):
            if new_array[line[0]][i] != math.inf:
                new_array[line[0]][i] = math.inf
                inv_amt += 1
            if new_array[i][line[1]] != math.inf:
                new_array[i][line[1]] = math.inf
                inv_amt += 1

        new_lines = copy.deepcopy(self.lines)
        new_lines.append(line)

        inv_amt += self.invalidate_lines(new_array, new_lines)

        return Matrix(new_array, new_lines, self.lower_bound, self.cell_amount - inv_amt)

    def ban_line(self, line: tuple[int, int]):
        new_array = copy.deepcopy(self.array)
        new_array[line[0]][line[1]] = math.inf

        return Matrix(new_array, self.lines, self.lower_bound, self.cell_amount - 1)

    def contains_two(self) -> bool:
        return self.cell_amount == 2

    def get_final_path(self) -> list[tuple[int, int]]:

        for i in range(len(self.array)):
            for j in range(len(self.array)):
                if self.array[i][j] != math.inf:
                    self.lines.append((i, j))

        chain = [self.lines[0]]

        while True:
            found = False

            for i in range(len(self.lines)):
                if self.lines[i][0] == chain[len(chain)-1][1]:
                    chain.append(self.lines[i])
                    found = True
                    break

            if not (found and len(chain) < len(self.array)):
                break

        return list(map(lambda t: (t[0]+1, t[1]+1), chain))


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

    def __len__(self):
        return len(self.__storage)


# file = open("input.txt", "r")
# dataset = "3\n0 8 86\n47 0 16\n87 54 0\n"
# lines = dataset.strip().split("\n")
# lines = file.readlines()
n = int(input())

array = [0] * n

for i in range(n):
    array[i] = list(map(lambda x: float(x), input().split(" ")))
    array[i][i] = math.inf

start = time.time_ns()

matrix = Matrix(array, [])

queue = BinHeap()
queue.insert(matrix.lower_bound, matrix)


bestSolution = Matrix(array, [], math.inf, -1)
counter = 0

while not queue.empty():
    counter = max(len(queue), counter)
    curr = queue.pop_max()[1]

    if curr.lower_bound >= bestSolution.lower_bound:
        break

    if curr.contains_two():
        if len(curr.lines) == n - 2:
            bestSolution = curr
        continue

    best_choice = curr.find_variant()
    # print(best_choice)

    added = curr.add_line(best_choice)
    queue.insert(added.lower_bound, added)
    banned = curr.ban_line(best_choice)
    queue.insert(banned.lower_bound, banned)

best_path = bestSolution.get_final_path()

end = time.time_ns()

print(int(bestSolution.lower_bound))
print(" ".join(list(map(lambda x: str(x[0]), best_path)) + [str(best_path[0][0])]))
# print(end-start)
