#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm> 

struct Stop {
    int id;
    std::string name;
    double lat;
    double lon;
};

struct Edge {
    int stop_id;
    int trip_id;
    std::string route_id;
    int departure_time; // in seconds since midnight
    int arrival_time;   // in seconds since midnight
    int travel_time;    // in seconds
};

class TransitGraph {
private:
    std::unordered_map<std::string, int> stop_ids;
    std::unordered_map<int, Stop> stops;
    std::unordered_map<int, std::vector<Edge>> edges;

    // Define your algorithms as private member functions
    int dijkstra(int from, int to, int startTime);
    int aStar(int from, int to, int startTime);

    inline
    bool stopExists(int id) const {
        return stops.find(id) != stops.end();
    }

    inline
    bool stopExists(const std::string& name) const {
        return stop_ids.find(name) != stop_ids.end();
    }

    inline 
    void validatePoints(int from, int to){
        if(!stopExists(from) || !stopExists(to)){
            throw std::runtime_error("One or both stops do not exist.");
        }
    }

    inline 
    void validatePoints(const std::string& from, const std::string& to){
        if(!stopExists(from) || !stopExists(to)){
            throw std::runtime_error("One or both stops do not exist.");
        }
    }


public:
    inline
    void addStop(const Stop& stop) {
        stops[stop.id] = stop;
        stop_ids[stop.name] = stop.id;
    }

    inline
    void addEdge(const int from_stop_id, const Edge& edge) {
        edges[from_stop_id].push_back(edge);
    }

    // Overloaded methods to find paths with or without custom algorithm
    inline
    int findPath(const int from, const int to, int startTime, int (TransitGraph::*algorithm)(int, int, int)){
        validatePoints(from ,to);
        return (this->*algorithm)(from, to, startTime);
    } 

    inline
    int findPath(const int from, const int to, int startTime){ // Default to Dijkstra
        return findPath(from, to, startTime, &TransitGraph::dijkstra);
    } 

    inline
    int findPath(const std::string& from, const std::string& to, int startTime) {
        validatePoints(from,to);
        return findPath(stop_ids[from], stop_ids[to], startTime);  // Default to Dijkstra
    }

    inline
    int findPath(const std::string& from, const std::string& to, int startTime, int (TransitGraph::*algorithm)(int, int, int)) {
        validatePoints(from ,to);
        return findPath(stop_ids[from], stop_ids[to], startTime, algorithm); // Custom algorithm
    }
};