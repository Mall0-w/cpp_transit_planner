#include <iostream>
#include <vector>
#include <string>
#include "filehandler.hpp"
#include "timehandler.hpp"

void strToUpper(std::string& str){
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return std::toupper(c); });
}

int main(){
    
    std::string folder_path;
    GTFSHandler g;

    do{
        std::cout << "Please input the path to your gtfs" << std::endl;
        std::getline(std::cin, folder_path);
        try{
            g.loadGraph(folder_path);
        }catch(const std::exception& e){
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
    }while(g.getGraph() == nullptr);
    

    int path = -1;
    do{
        std::cout << "Please input starting stop" << std::endl;
        std::string to;
        std::string from;
        std::getline(std::cin, from);
        std::cout << "Please input destination stop" << std::endl;
        std::getline(std::cin, to);
        strToUpper(to);
        strToUpper(from);

        try{
            path = g.getGraph()->findPath(from, to, getCurrentSecondsInDay());
        }catch(const std::runtime_error& e){
            std::cerr << "Runtime error: " << e.what() << std::endl;
        }
    }while(path == -1);

    std::cout << path << std::endl;

    return 0;
}