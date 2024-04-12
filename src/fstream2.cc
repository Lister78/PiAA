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

void open_file(const std::string& filename, int how_many, std::vector<Movie>& vector, int& null_line) 
{
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::out);
    if (!file.good())
        std::cout << "File cannot be open";
    else 
	{
        std::cout << "File opened succesfully" << std::endl;
        int loop_ctr = 0;
        std::string line;
		std::string temp;
        while (std::getline(file, line) && loop_ctr <= how_many) 
		{
            int length = line.length();
            if(line[length-1]==',')
            {
                null_line++;
                std::cout<<"Line without rating: "<<line<<std::endl;
            }
            else
            {
            std::istringstream movies_things(line);
            Movie movie;
            std::getline(movies_things, movie.number, ',');
			std::getline(movies_things, movie.title, ',');
            if(movie.title[0]=='"')
            {
			do
            {
                std::getline(movies_things, temp, ',');
                movie.title += temp;	
			} while (movie.title[movie.title.length()-1] != '"');
            }
            movies_things >> movie.rating;
            vector.push_back(movie);
	        loop_ctr++;
            }
    	}	
    }
}



int main() {
    std::vector<Movie> vec;
    int null_line = 0;
 	open_file("PAA.projekt1.dane.csv", 1011000, vec, null_line);
    for (const auto& movie : vec)
        std::cout << "Number: " << movie.number << ", Title: " << movie.title << ", Rating: " << movie.rating << std::endl;
    
	
	std::cout << "\nNumber of lines without rating: " << null_line;
    return 0;
}
