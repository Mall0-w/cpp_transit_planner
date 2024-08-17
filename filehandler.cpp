#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include "filehandler.hpp"
#include "timehandler.hpp"

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
    std::cout << "finding stops.txt" << std::endl;;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open stops.txt");
    }

    std::cout << "stops.txt found, loading stops into graph" << std::endl;
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
        stop.id = std::stoi(tokens[header_map["stop_id"]]);
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

    std::cout << "Loaded " << lines_read << " stops from stops.txt successfully.\n";
}

void GTFSHandler::loadStopTimes(const std::string& folder_path, const std::unique_ptr<TransitGraph>& graph) {
    fs::path dir(folder_path);
    fs::path file_path = dir / "stop_times.txt";

    // Try to open the file
    std::cout << "finding stop_times.txt" << std::endl;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open stop_times.txt");
    }

    std::cout << "stop_times.txt found, loading into graph" << std::endl;
    std::string header;
    std::getline(file, header);

    // Split the header into individual column names
    std::istringstream header_stream(header);
    std::string column;
    std::unordered_map<std::string, uint8_t> header_map;
    uint8_t index = 0;
    
    while (std::getline(header_stream, column, ',')) {
        header_map[column] = index++;
    }
    // Ensure the required columns exist
    if (header_map.find("trip_id") == header_map.end() ||
        header_map.find("arrival_time") == header_map.end() ||
        header_map.find("departure_time") == header_map.end() ||
        header_map.find("stop_id") == header_map.end() ||
        header_map.find("stop_sequence") == header_map.end()) {
        throw std::invalid_argument("Required columns not found in header");
    }

    uint8_t max_index = std::max({header_map["trip_id"], header_map["arrival_time"], header_map["departure_time"], header_map["stop_id"], header_map["stop_sequence"]});
    // Parse each row in the file
    std::string line;
    std::unordered_map<int, std::vector<Edge>> trip_edges;  // Store edges for each trip
    uint32_t lines_read = 0;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::vector<std::string> row(max_index + 1);
        uint8_t col_idx = 0;

        // Split row into columns
        while (col_idx < row.size() && std::getline(line_stream, row[col_idx++], ','));

        // Skip rows with missing data
        if (col_idx < max_index + 1) continue;

        int trip_id = std::stoi(row[header_map["trip_id"]]);
        int stop_id = std::stoi(row[header_map["stop_id"]]);
        int stop_sequence = std::stoi(row[header_map["stop_sequence"]]);
        int arrival_time = timeToSeconds(row[header_map["arrival_time"]]);
        int departure_time = timeToSeconds(row[header_map["departure_time"]]);

        // Create an edge
        Edge edge;
        edge.trip_id = trip_id;
        edge.stop_id = stop_id;
        edge.arrival_time = arrival_time;
        edge.departure_time = departure_time;

        // Store edges grouped by trip_id
        trip_edges[trip_id].push_back(edge);
        lines_read++;
    }

    std::cout << "read " << lines_read << " lines" << std::endl;
    // Create edges in the graph for each trip's consecutive stops
    lines_read = 0;
    for (const auto& trip_pair : trip_edges) {
        const auto& edges = trip_pair.second;
        for (size_t i = 0; i < edges.size() - 1; ++i) {
            lines_read++;
            const auto& current_stop = edges[i];
            const auto& next_stop = edges[i + 1];

            Edge edge;
            edge.stop_id = next_stop.stop_id;
            edge.trip_id = next_stop.trip_id;
            edge.arrival_time = next_stop.arrival_time;
            edge.departure_time = current_stop.departure_time;
            edge.travel_time = next_stop.arrival_time - current_stop.departure_time;

            // Add edge to the graph
            graph->addEdge(current_stop.stop_id, edge);
        }
    }

    std::cout << "added " << lines_read << " edges" << std::endl;
    return;
}

void GTFSHandler::loadGraph(const std::string& folder_path) {
    // Check that the folder exists and has all required files
    
    if(!GTFSHandler::checkValidGTFSLayout(folder_path)){
        throw std::invalid_argument("Folder at " + folder_path + " not a valid GTFS");
    }

    //try to go through the files and load them
    auto newGraph = std::make_unique<TransitGraph>();
    this->loadStops(folder_path, newGraph);

    this->loadStopTimes(folder_path, newGraph);

    //if everything went well move g into out GTFS handler
    this->g = std::move(newGraph); // Move the new graph into 
    std::cout << "valid GTFS" << std::endl;
}
