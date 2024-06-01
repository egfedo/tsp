#include <iostream>
#include <vector>
#include <queue>
#include <limits>

class Matrix {
public:
    std::vector<std::vector<float>> array;
    std::vector<int> path;
    std::vector<int> rem;
    size_t size;
    float lowerBound;

    Matrix(
            std::vector<std::vector<float>>& array,
            std::vector<int>& path,
            std::vector<int>& rem,
            float lowerBound
            ) : array(array), path(path), lowerBound(lowerBound), rem(rem) {

        size = array.size();
        for (int i = 0; i < size; i++) {
            float min = std::numeric_limits<float>::infinity();
            for (auto& elem : this->array[i]) {
                if (elem < min)
                    min = elem;
            }
            if (min != std::numeric_limits<float>::infinity()) {
                this->lowerBound += min;
                for (float& elem : this->array[i]) {
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
    void invalidateLines(std::vector<std::vector<float>>& array,
                         const std::vector<int>& vertices) {
        int last = vertices[vertices.size()-1];

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
            int a = newPath[len-2];
            int b = newPath[len-1];
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

class Compare {
public:
    bool operator()(Matrix a, Matrix b) {
        return a.lowerBound > b.lowerBound;
    }
};

int main() {
//    std::cout << "Enter matrix size: ";
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

    std::vector<std::vector<float>> newArray;
    newArray.reserve(n);
    for (int i = 0; i < n; i++) {
        newArray.emplace_back(array[i]);
    }


    std::priority_queue<Matrix, std::vector<Matrix>, Compare> queue;
    std::vector<int> path;
    std::vector<int> rem;
    rem.reserve(n);
    for (int i = 1; i < n; i++) {
        rem.push_back(i);
    }
    Matrix matrix(newArray, path, rem, 0);
    matrix = matrix.addVertex(0);
    queue.push(matrix);
//    auto start = std::chrono::high_resolution_clock::now();

    Matrix bestSolution(array, path, rem, std::numeric_limits<float>::infinity());

    while (!queue.empty()) {
        auto curr = queue.top();
        queue.pop();

        if (curr.lowerBound >= bestSolution.lowerBound)
            break;

        if (curr.rem.size() == 0) {
            bestSolution = curr;
            continue;
        }

        for (auto& vertex : curr.rem) {
            queue.push(curr.addVertex(vertex));
        }
    }

    std::vector<int> bestPath(bestSolution.path);

//    auto stop = std::chrono::high_resolution_clock::now();

    float length = 0;
    for (int i = 1; i < bestPath.size(); i++) {
        length += array[bestPath[i-1]][bestPath[i]];
    }
    length += array[bestPath[n-1]][bestPath[0]];
    std::cout << length << "\n";
    for (auto& vertex : bestPath) {
        std::cout << vertex + 1 << " ";
    }
    std::cout << bestPath[0] + 1 << "\n";
//    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count()  << "\n";

    return 0;
}
