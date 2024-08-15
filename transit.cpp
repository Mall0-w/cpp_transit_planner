#include <iostream>
#include <vector>
#include <string>
#include "filehandler.hpp"

int main(){
    std::cout << "Please input the path to your gtfs" << std::endl;
    std::string folder_path;
    std::getline(std::cin, folder_path);
    GTFSHandler g;

    try{
        g.loadGraph(folder_path);
    }catch(const std::exception& e){
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}