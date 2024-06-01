#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>

namespace path3 {

    struct Entry {
    public:
        float len = 0;
        int path[21] = {};
        int vertexCount = 0;
        bool visited[21] = {};
    };


    class Compare {
    public:
        bool operator()(Entry a, Entry b) {
            return a.len > b.len;
        }
    };

    long long solve(int n, const std::vector<std::vector<float>> &arr) {
        std::vector<std::vector<float>> array;
        for (int i = 0; i < n; i++) {
            array.push_back(std::vector(arr[i]));
            array[i][i] = std::numeric_limits<float>::infinity();
        }

        std::vector<std::vector<float>> newArray;
        newArray.reserve(n);
        for (int i = 0; i < n; i++) {
            newArray.emplace_back(array[i]);
        }


        std::stack<Entry> stack;
        Entry first;
        first.visited[0] = true;
        first.vertexCount++;

        stack.push(first);
        auto start = std::chrono::high_resolution_clock::now();

        Entry bestSolution;
        bestSolution.len = std::numeric_limits<float>::infinity();

        while (!stack.empty()) {
            auto curr = stack.top();
            stack.pop();


            if (curr.len >= bestSolution.len)
                continue;

            if (curr.vertexCount == n) {
                float fullLen = curr.len + newArray[curr.path[n-1]][curr.path[0]];
                if (fullLen < bestSolution.len) {
                    curr.path[n] = curr.path[0];
                    curr.len = fullLen;
                    bestSolution = curr;
                }
                continue;
            }

            for (int vertex = 0; vertex < n; vertex++) {
                if (!curr.visited[vertex]) {
                    auto newEntry = curr;
                    newEntry.path[newEntry.vertexCount] = vertex;
                    newEntry.len += newArray[newEntry.path[newEntry.vertexCount-1]][vertex];
                    newEntry.vertexCount++;
                    newEntry.visited[vertex] = true;
                    stack.push(newEntry);
                }
            }
        }


        auto stop = std::chrono::high_resolution_clock::now();

        float length = 0;
        for (int i = 1; i <= n; i++) {
            length += array[bestSolution.path[i - 1]][bestSolution.path[i]];
        }
        std::cout << length << "\n";
        for (int i = 0; i <= n; i++) {
            std::cout << bestSolution.path[i] + 1 << " ";
        }
//        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";

    return 0;
    }

}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<std::vector<float>> array;
    for (int i = 0; i < n; i++) {
        array.emplace_back();
        for (int j = 0; j < n; j++) {
            float number;
            std::cin >> number;
            array[i].push_back(number);
        }
        array[i][i] = std::numeric_limits<float>::infinity();
    }

    path3::solve(n, array);
}
