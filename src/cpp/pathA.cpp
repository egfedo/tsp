#include <iostream>
#include <vector>
#include <queue>


namespace path2 {
    
    class Matrix {
    public:
        std::vector<std::vector<float>> array;
        std::vector<int> path;
        std::vector<int> rem;
        size_t size;
        float lowerBound;

        Matrix(
                std::vector<std::vector<float>> &array,
                std::vector<int> &path,
                std::vector<int> &rem,
                float lowerBound
        ) : array(array), path(path), lowerBound(lowerBound), rem(rem) {

            size = array.size();
            for (int i = 0; i < size; i++) {
                float min = std::numeric_limits<float>::infinity();
                for (auto &elem: this->array[i]) {
                    if (elem < min)
                        min = elem;
                }
                if (min != std::numeric_limits<float>::infinity()) {
                    this->lowerBound += min;
                    for (float &elem: this->array[i]) {
                        elem -= min;
                    }
                }
            }

            for (int j = 0; j < size; j++) {
                float min = std::numeric_limits<float>::infinity();
                for (int i = 0; i < size; i++) {
                    if (this->array[i][j] < min)
                        min = this->array[i][j];
                }

                if (min != std::numeric_limits<float>::infinity()) {
                    this->lowerBound += min;
                    for (int i = 0; i < size; i++) {
                        this->array[i][j] -= min;
                    }
                }
            }
        }

    private:
        void invalidateLines(std::vector<std::vector<float>> &array,
                             const std::vector<int> &vertices) {
            int last = vertices[vertices.size() - 1];

            for (int j = 0; j < vertices.size(); j++) {
                array[last][vertices[j]] = std::numeric_limits<float>::infinity();
            }
        }

    public:
        Matrix addVertex(int vertex) {

            std::vector<std::vector<float>> newArray;
            std::vector<int> newPath(path);
            std::vector<int> newRem(rem);

            newArray.reserve(size);
            for (int i = 0; i < size; i++) {
                newArray.emplace_back(array[i]);
            }

            float newLower = lowerBound;

            newPath.push_back(vertex);
            if (newPath.size() >= 2) {
                int len = newPath.size();
                int a = newPath[len - 2];
                int b = newPath[len - 1];
                newLower += newArray[a][b];

                for (int i = 0; i < size; i++) {
                    newArray[a][i] = std::numeric_limits<float>::infinity();
                    newArray[i][b] = std::numeric_limits<float>::infinity();
                }
            }

            invalidateLines(newArray, newPath);

            newRem.erase(std::remove(newRem.begin(), newRem.end(), vertex), newRem.end());

            return Matrix(newArray, newPath, newRem, newLower);
        }

    };



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


        std::priority_queue<Entry, std::vector<Entry>, Compare> queue;
        Entry first;
        first.visited[0] = true;
        first.vertexCount++;

        queue.push(first);
        auto start = std::chrono::high_resolution_clock::now();

        Entry bestSolution;
        bestSolution.len = std::numeric_limits<float>::infinity();

        while (!queue.empty()) {
            auto curr = queue.top();
            queue.pop();


            if (curr.len >= bestSolution.len)
                break;

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
                    queue.push(newEntry);
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

        return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
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

    path2::solve(n, array);
}
