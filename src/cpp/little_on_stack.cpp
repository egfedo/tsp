#include <iostream>
#include <list>
#include <queue>
#include <chrono>
#include <stack>

class Matrix {
public:
    float lowerBound;
private:
    int size;
    std::vector<std::vector<float>> array;
    std::vector<std::pair<int, int>> lines;

    int cellAmount;

public:
    Matrix(int size, std::vector<std::vector<float>>& array,
           std::vector<std::pair<int, int>>& lines,
           float lowerBound = 0,
           int cellAmount = -1) :
            size(size), array(array), lines(lines),
            lowerBound(lowerBound) {
        if (cellAmount == -1) {
            cellAmount = size * (size - 1);
        }
        this->cellAmount = cellAmount;

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

    float calcDelta(int x, int y) {
        float minRow = std::numeric_limits<float>::infinity();
        float minColumn = std::numeric_limits<float>::infinity();
        for (int i = 0; i < size; i++) {
            if (i != y) {
                if (array[x][i] < minRow)
                    minRow = array[x][i];
            }
            if (i != x) {
                if (array[i][y] < minColumn)
                    minColumn = array[i][y];
            }
        }
        if (minRow == std::numeric_limits<float>::infinity()) {
            minRow = 0.0f;
        }
        if (minColumn == std::numeric_limits<float>::infinity()) {
            minColumn = 0.0f;
        }

        return minRow + minColumn;
    }

    std::pair<int, int> findVariant() {
        std::pair<int, int> output = {-1, -1};
        float maxDelta = -1;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (array[i][j] == 0) {
                    float delta = calcDelta(i, j);
                    if (delta > maxDelta) {
                        maxDelta = delta;
                        output = {i, j};
                    }
                }
            }
        }
        return output;
    }

    static int invalidateLines(std::vector<std::vector<float>>& array,
                               const std::vector<std::pair<int, int>>& lines) {
        int invAmount = 0;
        std::vector<std::pair<int, int>> chain;
        chain.push_back(lines[lines.size()-1]);
        int startIndex = 0;

        bool found = false;
        do {
            found = false;
            for (auto & line : lines) {
                if (line.first == chain[chain.size()-1].second) {
                    chain.push_back(line);
                    found = true;
                }

                if (line.second == chain[0].first) {
                    chain.insert(chain.begin(), line);
                    found = true;
                    startIndex++;
                }
            }
        } while (found);

        for (auto a = startIndex; a >= 0; a--) {
            for (auto b = startIndex; b < chain.size(); b++) {
                if (array[chain[b].second][chain[a].first] != std::numeric_limits<float>::infinity()) {
                    array[chain[b].second][chain[a].first] = std::numeric_limits<float>::infinity();
                    invAmount++;
                }
            }
        }
        return invAmount;
    }

    Matrix addLine(const std::pair<int, int>& line) {
        std::vector<std::vector<float>> output;
        output.reserve(size);
        for (int i = 0; i < size; i++) {
            output.emplace_back(array[i]);
        }

        int infAmt = 0;
        for (int i = 0; i < size; i++) {
            if (output[line.first][i] != std::numeric_limits<float>::infinity()) {
                output[line.first][i] = std::numeric_limits<float>::infinity();
                infAmt++;
            }
            if (output[i][line.second] != std::numeric_limits<float>::infinity()) {
                output[i][line.second] = std::numeric_limits<float>::infinity();
                infAmt++;
            }
        }
        std::vector<std::pair<int, int>> newLines(lines);
        newLines.push_back(line);

        infAmt += invalidateLines(output, newLines);

        return Matrix(size, output, newLines, lowerBound, cellAmount - infAmt);
    }

    Matrix banLine(std::pair<int, int> line) {
        std::vector<std::vector<float>> output;
        output.reserve(size);
        for (int i = 0; i < size; i++) {
            output.emplace_back(array[i]);
        }
        output[line.first][line.second] = std::numeric_limits<float>::infinity();
        std::vector<std::pair<int, int>> newLines(lines);

        return {size, output, newLines, lowerBound, cellAmount - 1};
    }

    bool containsTwo() {
        return cellAmount == 2;
    }

    std::vector<int> getFinalPath() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (array[i][j] != std::numeric_limits<float>::infinity())
                    lines.emplace_back(i, j);
            }
        }

        std::vector<std::pair<int, int>> chain;
        chain.push_back(lines[0]);

        bool found = false;
        do {
            for (auto & line : lines) {
                if (line.first == chain[chain.size()-1].second) {
                    chain.push_back(line);
                    found = true;
                    break;
                }
            }
        } while (found && chain.size() < size);

        std::vector<int> out;
        for (auto& elem : chain) {
            out.push_back(elem.first+1);
        }
        return out;
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


    std::stack<Matrix> stack;
//    std::priority_queue<Matrix, std::vector<Matrix>, Compare> queue;
    std::vector<std::pair<int, int>> lines;
    stack.emplace(n, array, lines, 0, -1);
//    queue.emplace();
    auto start = std::chrono::high_resolution_clock::now();

    Matrix bestSolution(n, array, lines, std::numeric_limits<float>::infinity(), -1);

    while (!stack.empty()) {
        auto curr = stack.top();
        stack.pop();

        if (curr.lowerBound >= bestSolution.lowerBound)
            continue;

        if (curr.containsTwo()) {
            bestSolution = curr;
            continue;
        }

        auto bestChoice = curr.findVariant();
        stack.push(curr.banLine(bestChoice));
        stack.push(curr.addLine(bestChoice));
    }

    auto bestPath = bestSolution.getFinalPath();

    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << bestSolution.lowerBound << "\n";
    for (auto& elem : bestPath) {
        std::cout << elem << " ";
    }
    std::cout << bestPath[0] << "\n";
//    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count()  << "\n";

    return 0;
}
