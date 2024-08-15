#include <iostream>
#include <filesystem>
#include <vector>
#include "filehandler.hpp"

namespace fs = std::filesystem;
// Define the required files for GTFS
const std::vector<std::string> GTFSHandler::requiredFiles = {
    "stops.txt",
    "stop_times.txt",
    "routes.txt",
    "trips.txt"
};


const bool GTFSHandler::checkValidGTFSLayout(const std::string& folder_path){
    //check that folder exists
    fs::path dir(folder_path);
    if (!fs::exists(dir) || !fs::is_directory(dir)){
        std::cerr << "Folder does not exist: " << folder_path << std::endl;
        return false;
    }
        

    //check that folder has our relevant text files
    //this means routes.txt, stop_times.txt, stops.txt, and trips.txt for now
    for (const auto& filename : GTFSHandler::requiredFiles) {
        fs::path file_path = dir / filename;
        if (!fs::exists(file_path)) {
            std::cerr << "File does not exist: " << file_path << std::endl;
            return false;
        }
    }
    return true;
}

//we can assume that the folderpath and file already exists because a prerequisite for calling laodstops is checkValidGTFSLayout;
void GTFSHandler::loadStops(const std::string& folder_path, std::unique_ptr<TransitGraph> graph){
    fs::path dir(folder_path);
    fs::path file_path = dir / "stops.txt";

    uint32_t lines_read = 0;
    //check that has valid format stop_id,stop_code,stop_name,stop_desc,stop_lat,stop_lon,zone_id,stop_url,location_type,parent_station,stop_timezone,wheelchair_boarding

    //if header is valid, go through each line and create a stop.  then add that stop to our graph

    //if everything went smoothly, return

    return;
}

void GTFSHandler::loadGraph(const std::string& folder_path) {
    // Check that the folder exists and has all required files
    if(!GTFSHandler::checkValidGTFSLayout(folder_path)){
        throw std::invalid_argument("Folder at " + folder_path + " not a valid GTFS");
    }

    //try to go through the files and load them
    auto newGraph = std::make_unique<TransitGraph>();



    //if everything went well move g into out GTFS handler
    this->g = std::move(newGraph); // Move the new graph into 
    std::cout << "valid GTFS";
}
