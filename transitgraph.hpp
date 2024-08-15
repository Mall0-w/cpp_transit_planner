#include <string>
#include <unordered_map>

struct Stop {
    std::string id;
    std::string name;
    double lat;
    double lon;
};

struct Edge {
    std::string trip_id;
    std::string route_id;
    int departure_time; // in seconds since midnight
    int arrival_time;   // in seconds since midnight
    int travel_time;    // in seconds
};

class TransitGraph {
private:
    std::unordered_map<std::string, Stop> stops;
    std::unordered_map<std::string, std::vector<Edge>> adjacency_list;

public:
    inline
    void addStop(const Stop& stop) {
        stops[stop.id] = stop;
    }
    inline
    void addEdge(const std::string& from_stop_id, const std::string& to_stop_id, const Edge& edge) {
        adjacency_list[from_stop_id].push_back(edge);
    }
};