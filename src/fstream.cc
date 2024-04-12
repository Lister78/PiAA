#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

struct Movie {
    std::string title;
    int ranking;
};

std::vector<Movie> readCSV(const std::string& filename) {
    std::vector<Movie> movies;
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return movies;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string title;
        int ranking;
        std::string discard;
        if (std::getline(iss, discard, ',') && std::getline(iss, title, ',') && std::getline(iss, discard, ',') && iss >> ranking) {
            movies.push_back({title, ranking});
        }
    }
    file.close();
    return movies;
}

template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

void quickSort(std::vector<Movie>& movies, int low, int high) {
    if (low < high) {
        int pivot = movies[high].ranking;
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (movies[j].ranking <= pivot) {
                ++i;
                swap(movies[i], movies[j]);
            }
        }
        swap(movies[i + 1], movies[high]);
        int partitionIndex = i + 1;
        quickSort(movies, low, partitionIndex - 1);
        quickSort(movies, partitionIndex + 1, high);
    }
}


void merge(std::vector<Movie>& movies, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<Movie> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = movies[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = movies[middle + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].ranking <= R[j].ranking) {
            movies[k] = L[i];
            ++i;
        } else {
            movies[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        movies[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        movies[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<Movie>& movies, int low, int high) {
    if (low < high) {
        int middle = low + (high - low) / 2;
        mergeSort(movies, low, middle);
        mergeSort(movies, middle + 1, high);
        merge(movies, low, middle, high);
    }
}

void bucketSort(std::vector<Movie>& movies) {
    const int maxRanking = 10; // zakładam, że rankingi są w przedziale od 1 do 10
    std::vector<std::vector<Movie>> buckets(maxRanking + 1);
    for (const auto& movie : movies) {
        buckets[movie.ranking].push_back(movie);
    }
    movies.clear();
    for (auto& bucket : buckets) {
        movies.insert(movies.end(), bucket.begin(), bucket.end());
    }
}


template<typename SortFunc>
std::chrono::milliseconds measureSortTime(std::vector<Movie>& movies, SortFunc sortFunc, int low, int high) {
    auto start = std::chrono::steady_clock::now();
    sortFunc(movies, low, high);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

int main() {
    std::vector<Movie> movies = readCSV("projekt2_dane.csv");

    movies.erase(std::remove_if(movies.begin(), movies.end(), [](const Movie& movie) {
        return movie.ranking == 0;
    }), movies.end());

    std::vector<int> datasetSizes = {10000, 100000, 500000, 1000000, static_cast<int>(movies.size())};
    for (int size : datasetSizes) {
        std::vector<Movie> subset(movies.begin(), movies.begin() + size);
        
        auto timeQuick = measureSortTime(subset, quickSort, 0, subset.size() - 1);
        std::cout << "QuickSort time for " << size << " movies: " << timeQuick.count() << " ms\n";

        // Pomiar czasu dla mergesort
        auto timeMerge = measureSortTime(subset, mergeSort, 0, subset.size() - 1);
        std::cout << "MergeSort time for " << size << " movies: " << timeMerge.count() << " ms\n";

        // Pomiar czasu dla bucket sort
        auto timeBucket = measureSortTime(subset, bucketSort, 0, subset.size() - 1);
        std::cout << "BucketSort time for " << size << " movies: " << timeBucket.count() << " ms\n";

        // Obliczenie średniej i mediany rankingu
        int sum = 0;
        std::vector<int> rankings;
        for (const auto& movie : subset) {
            sum += movie.ranking;
            rankings.push_back(movie.ranking);
        }
        double mean = static_cast<double>(sum) / size;
        std::sort(rankings.begin(), rankings.end());
        double median;
        if (size % 2 == 0) {
            median = (rankings[size / 2 - 1] + rankings[size / 2]) / 2.0;
        } else {
            median = rankings[size / 2];
        }
        std::cout << "Mean ranking: " << mean << ", Median ranking: " << median << std::endl;
    }

    return 0;
}
