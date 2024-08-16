#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> 
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

void GTFSHandler::loadStops(const std::string& folder_path, const std::unique_ptr<TransitGraph>& graph) {
    fs::path dir(folder_path);
    fs::path file_path = dir / "stops.txt";

    // Try to open the file
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open stops.txt");
    }

    std::string header;
    std::getline(file, header);

    // Split the header into individual column names
    std::istringstream header_stream(header);
    std::string column;
    std::unordered_map<std::string, size_t> header_map;
    size_t index = 0;
    
    while (std::getline(header_stream, column, ',')) {
        header_map[column] = index++;
    }

    // Ensure the required columns exist
    if (header_map.find("stop_id") == header_map.end() ||
        header_map.find("stop_name") == header_map.end() ||
        header_map.find("stop_lat") == header_map.end() ||
        header_map.find("stop_lon") == header_map.end()) {
        throw std::invalid_argument("Required columns not found in header");
    }

    //max index of our required columns, used to determine invalid rows / skipping
    uint8_t max_index = std::max({header_map["stop_id"], header_map["stop_name"], header_map["stop_lat"], header_map["stop_lon"]});

    // Go through each line and create a stop, adding only relevant fields
    uint32_t lines_read = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        // Split the line into tokens
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Ensure the line has enough columns
        if (tokens.size() <= max_index) {
            continue;  // Skip invalid rows
        }

        // Extract relevant fields
        Stop stop;
        stop.id = tokens[header_map["stop_id"]];
        stop.name = tokens[header_map["stop_name"]];
        stop.lat = std::stod(tokens[header_map["stop_lat"]]);
        stop.lon = std::stod(tokens[header_map["stop_lon"]]);

        // Add the stop to the graph
        graph->addStop(stop);
        lines_read++;
    }

    if (lines_read == 0) {
        throw std::runtime_error("No stops found in stops.txt");
    }

    std::cout << "Loaded " << lines_read << " stops successfully.\n";
}

void GTFSHandler::loadGraph(const std::string& folder_path) {
    // Check that the folder exists and has all required files
    if(!GTFSHandler::checkValidGTFSLayout(folder_path)){
        throw std::invalid_argument("Folder at " + folder_path + " not a valid GTFS");
    }

    //try to go through the files and load them
    auto newGraph = std::make_unique<TransitGraph>();
    this->loadStops(folder_path, newGraph);


    //if everything went well move g into out GTFS handler
    this->g = std::move(newGraph); // Move the new graph into 
    std::cout << "valid GTFS";
}
