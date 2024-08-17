#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm> 

struct Stop {
    unsigned int id;
    std::string name;
    double lat;
    double lon;
};

struct Edge {
    unsigned int stop_id;
    unsigned int trip_id;
    std::string route_id;
    unsigned int departure_time; // in seconds since midnight
    unsigned int arrival_time;   // in seconds since midnight
    unsigned int travel_time;    // in seconds
};

class TransitGraph {
private:
    std::unordered_map<std::string, unsigned int> stop_ids;
    std::unordered_map<unsigned int, Stop> stops;
    std::unordered_map<unsigned int, std::vector<Edge>> edges;

    // Define your algorithms as private member functions
    std::vector<Edge> dijkstra(unsigned int from, unsigned int to, unsigned int startTime);
    std::vector<Edge> aStar(unsigned int from, int to, unsigned int startTime);

    inline
    bool stopExists(unsigned int id) const {
        return stops.find(id) != stops.end();
    }

    inline
    bool stopExists(const std::string& name) const {
        return stop_ids.find(name) != stop_ids.end();
    }

    inline 
    void validatePoints(unsigned int from, unsigned int to){
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
    const Stop& getStop(unsigned int id){
        return stops[id];
    }

    inline 
    const Stop& getStop(std::string name){
        return getStop(stop_ids[name]);
    }

    inline
    void addEdge(const unsigned int from_stop_id, const Edge& edge) {
        edges[from_stop_id].push_back(edge);
    }

    // Overloaded methods to find paths with or without custom algorithm
    inline
    std::vector<Edge> findPath(const int unsigned from, const unsigned int to, unsigned int startTime, std::vector<Edge> (TransitGraph::*algorithm)(unsigned int, unsigned int, unsigned int)){
        validatePoints(from ,to);
        return (this->*algorithm)(from, to, startTime);
    } 

    inline
    std::vector<Edge> findPath(const unsigned int from, const unsigned int to, unsigned int startTime){ // Default to Dijkstra
        return findPath(from, to, startTime, &TransitGraph::dijkstra);
    } 

    inline
    std::vector<Edge> findPath(const std::string& from, const std::string& to, unsigned int startTime) {
        validatePoints(from,to);
        return findPath(stop_ids[from], stop_ids[to], startTime);  // Default to Dijkstra
    }

    inline
    std::vector<Edge> findPath(const std::string& from, const std::string& to, unsigned int startTime, std::vector<Edge> (TransitGraph::*algorithm)(unsigned int, unsigned int, unsigned int)) {
        validatePoints(from ,to);
        return findPath(stop_ids[from], stop_ids[to], startTime, algorithm); // Custom algorithm
    }
};