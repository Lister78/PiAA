#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Movie {
    std::string number;
    std::string title;
    double rating;
};

void open_file(const std::string& filename,std::vector<Movie>& vector, int& null_line) {
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::out);
    if (!file.good()) {
        std::cout << "File cannot be opened";
        return;
    }
    std::cout << "File opened successfully" << std::endl;
    int loop_ctr = 0;
    std::string line;
    while (std::getline(file, line)) 
    {
        if (line.back() == ',') {
            null_line++;
            std::cout << "Line without rating: " << line << std::endl;
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

int main() {
    std::vector<Movie> vec;
    int null_line = 0;
    open_file("PAA.projekt1.dane.csv",vec, null_line);
    for (const auto& movie : vec) {
        std::cout << "Number: " << movie.number << ", Title: " << movie.title << ", Rating: " << movie.rating << std::endl;
    }
    std::cout << "\nNumber of lines without rating: " << null_line;
    std::cout<<"\n"<< vec.size();
    return 0;
}
