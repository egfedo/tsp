#include <iostream>
#include <chrono>

int main() {
    int n = 0;
    std::cin >> n;
    auto** array = new float*[n];
    for (int i = 0; i < n; i++) {
        array[i] = new float[n];
        for (int j = 0; j < n; j++) {
            std::cin >> array[i][j];
        }
        array[i][i] = 0;
    }
    float minPath = INFINITY;
    std::vector<int> minPathVector;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> path;
    path.reserve(n);
    for (int i = 0; i < n; i++) {
        path.push_back(i);
    }
    do {
        float pathLen = 0.0f;
        for (int i = 1; i < n; i++) {
            pathLen += array[path[i-1]][path[i]];
        }
        pathLen += array[path[n-1]][path[0]];
        if (pathLen < minPath) {
            minPath = pathLen;
            minPathVector = std::vector(path);
        }
    } while (std::next_permutation(path.begin(), path.end()));

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << minPath << "\n";
    for (int i = 0; i < n; i++) {
        std::cout << (minPathVector[i] + 1) << " ";
    }
    std::cout << (minPathVector[0] + 1) << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";

    return 0;
}
