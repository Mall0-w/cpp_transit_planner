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
    

    std::vector<Edge> path;
    std::string to;
    std::string from;

    TransitGraph* graph = g.getGraph();;
    do{

        std::cout << "Please input starting stop (type \"exit\" to quit)" << std::endl;
        std::getline(std::cin, from);
        if(from.compare("exit") == 0)
            break;
        std::cout << "Please input destination stop" << std::endl;
        std::getline(std::cin, to);
        strToUpper(to);
        strToUpper(from);

        try{
            
            path = graph->findPath(from, to, getCurrentSecondsInDay());
            
            if(path.size() == 0){
                std::cout << "No transit path exists between these two stations." << std::endl;
                continue;
            }

            std::cout << "travel path: " << std::endl;
            Stop current = graph->getStop(from);
            for(const auto& point : path){
                std::cout << "From " << current.name << " [" << secondsToTime(point.departure_time) << "]" <<
                 " to " << graph->getStop(point.stop_id).name << " [" << secondsToTime(point.arrival_time) << "]" 
                 << std::endl;
                
                current = graph->getStop(point.stop_id);
            }

        }catch(const std::runtime_error& e){
            std::cerr << "Runtime error: " << e.what() << std::endl;
        }
    }while(true);

    return 0;
}