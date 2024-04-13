#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>

struct Movie {
    std::string number;
    std::string title;
    double rating;
};

void open_file(const std::string& filename, std::vector<Movie>& vector, int& null_line) {
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::out);
    if (!file.good()) {
        std::cout << "File cannot be opened";
        return;
    }
    std::cout << "File opened successfully" << std::endl;
    int loop_ctr = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.back() == ',') {
            null_line++;
        } else {
            std::istringstream movies_things(line);
            Movie movie;
            std::getline(movies_things, movie.number, ',');
            std::getline(movies_things, movie.title, ',');
            if (movie.title.front() == '"') {
                std::string temp;
                do {
                    std::getline(movies_things, temp, ',');
                    movie.title += temp;	
                } while (movie.title[movie.title.length()-1] != '"' && std::getline(movies_things, temp, ','));
            }
            movies_things >> movie.rating;
            vector.push_back(movie);
            loop_ctr++;
        }
    }
    file.close();
}

void quicksort(std::vector<Movie>& vec, int left, int right) {
    int i = left, j = right;
    double pivot = vec[(left + right) / 2].rating;
    Movie temp;

    while (i <= j) {
        while (vec[i].rating < pivot)
            i++;
        while (vec[j].rating > pivot)
            j--;
        if (i <= j) {
            temp = vec[i];
            vec[i] = vec[j];
            vec[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j)
        quicksort(vec, left, j);
    if (i < right)
        quicksort(vec, i, right);
}

void merge(std::vector<Movie>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Movie> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = vec[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (L[i].rating <= R[j].rating) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(std::vector<Movie>& vec, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergesort(vec, left, mid);
    mergesort(vec, mid + 1, right);
    merge(vec, left, mid, right);
}

int partition(std::vector<Movie>& vec, int low, int high) {
    double pivot = vec[high].rating;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (vec[j].rating < pivot) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return (i + 1);
}

void insertionSort(std::vector<Movie>& vec, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        Movie key = vec[i];
        int j = i - 1;

        while (j >= low && vec[j].rating > key.rating) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = key;
    }
}

void introsortUtil(std::vector<Movie>& vec, int low, int high, int depthLimit) {
    if (high - low > 16) {
        if (depthLimit == 0) {
            mergesort(vec, low, high);
            return;
        }
        depthLimit = depthLimit - 1;
        int pivot = partition(vec, low, high);
        introsortUtil(vec, low, pivot - 1, depthLimit);
        introsortUtil(vec, pivot + 1, high, depthLimit);
    } else {
        insertionSort(vec, low, high);
    }
}

void introsort(std::vector<Movie>& vec, int low, int high) {
    int depthLimit = 2 * log(high - low);
    introsortUtil(vec, low, high, depthLimit);
}

double calculate_median(const std::vector<Movie>& vec) {
    int size = vec.size();
    if (size == 0) return 0.0;
    if (size % 2 == 0) {
        return (vec[size / 2 - 1].rating + vec[size / 2].rating) / 2.0;
    } else {
        return vec[size / 2].rating;
    }
}

double calculate_mean(const std::vector<Movie>& vec) {
    if (vec.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& movie : vec) {
        sum += movie.rating;
    }
    return sum / vec.size();
}

int main() {
    std::vector<Movie> original_vec;
    int null_line = 0;
    open_file("PAA.projekt1.dane.csv", original_vec, null_line);

    int to_sort = 2000000; 
    if (to_sort > original_vec.size())
        to_sort = original_vec.size(); 

    std::vector<Movie> sorted_vec_q(original_vec.begin(), original_vec.begin() + to_sort);
    std::vector<Movie> sorted_vec_m(original_vec.begin(), original_vec.begin() + to_sort);
    std::vector<Movie> sorted_vec_i(original_vec.begin(), original_vec.begin() + to_sort);

    auto start_q = std::chrono::high_resolution_clock::now();
    quicksort(sorted_vec_q, 0, to_sort - 1);
    auto end_q = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds_q = end_q - start_q;

    auto start_m = std::chrono::high_resolution_clock::now();
    mergesort(sorted_vec_m, 0, to_sort - 1);
    auto end_m = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds_m = end_m - start_m;

    auto start_i = std::chrono::high_resolution_clock::now();
    introsort(sorted_vec_i, 0, to_sort - 1);
    auto end_i = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds_i = end_i - start_i;


    for (int i = 0; i < to_sort; ++i) {
        const auto& movie_q = sorted_vec_q[i];
        const auto& movie_m = sorted_vec_m[i];
        const auto& movie_i = sorted_vec_i[i];
        std::cout << "QuickSort: Number: " << movie_q.number << ", Title: " << movie_q.title << ", Rating: " << movie_q.rating << std::endl;
        std::cout << "MergeSort: Number: " << movie_m.number << ", Title: " << movie_m.title << ", Rating: " << movie_m.rating << std::endl;
        std::cout << "IntroSort: Number: " << movie_i.number << ", Title: " << movie_i.title << ", Rating: " << movie_i.rating << std::endl;
    }
    
    std::cout << "\nNumber of lines without rating in whole file: " << null_line << std::endl;
    std::cout << "\nTotal number of movies in this sorting: " << sorted_vec_q.size() << std::endl;
    
    double median_q = calculate_median(sorted_vec_q);
    double mean_q = calculate_mean(sorted_vec_q);
    
    double median_m = calculate_median(sorted_vec_m);
    double mean_m = calculate_mean(sorted_vec_m);
    
    double median_i = calculate_median(sorted_vec_i);
    double mean_i = calculate_mean(sorted_vec_i);
    
    std::cout<<"\n\n\nSORTING ALGORITHMS"<<std::endl;
    std::cout << "QuickSort - Median rating of sorted movies: " << median_q << ", Mean rating of sorted movies: " << mean_q << ", Sorting time: " << elapsed_seconds_q.count() << "s\n";
    std::cout << "Merge Sort: Sorting time: " << elapsed_seconds_m.count() << "s\n";
    std::cout << "IntroSort:  Sorting time: " << elapsed_seconds_i.count() << "s\n";
    
    return 0;
}
