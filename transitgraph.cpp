#include "transitgraph.hpp"
#include <climits>
#include <queue>
#define SECONDS_IN_DAY 86400


//asumes both from and to exist already
std::vector<Edge> TransitGraph::dijkstra(const unsigned int from, const unsigned int to, const unsigned int startTime){

    struct Tracker{
        const Stop* s;
        bool visited = false;
        unsigned int time_traveled = UINT_MAX;
        Tracker* prev = nullptr;
        const Edge* edge_taken = nullptr;
    };

    std::unordered_map<int, Tracker> trackers;
    for (const auto& [id, stop] : stops) {
        trackers[id] = Tracker{&stop, false, UINT_MAX, nullptr};
    }
    
    trackers[from] = Tracker{&stops[from], false, startTime, nullptr};
    //declare a priority queue
    auto cmp = [](const Tracker* a, const Tracker* b) {
        return a->time_traveled > b->time_traveled; // Min-heap based on time_traveled
    };

    std::priority_queue<Tracker*, std::vector<Tracker*>, decltype(cmp)> pq(cmp);
    //adding the start node
    pq.push(&trackers[from]);
    
    Tracker* current = &trackers[from];
    unsigned int new_time;
    while(!pq.empty() && current->s->id != to){
        current = pq.top();
        pq.pop();

        if(current->s->id == to)
            break;
        
        if(current->visited)
            continue;

        current->visited = true;
        for(const auto& edge : edges[current->s->id]){
            Tracker& neighbour = trackers[edge.stop_id];
            //if have already visited that edge, ignore it
            if(neighbour.visited)
                continue;

            //make weight time passed thanks to the stop
            //[when that bus leaves, plus a day if we missed it.]

            //weight should be departing time plus how long it took to get to that stop.
            new_time = edge.departure_time < current->time_traveled ? edge.arrival_time + SECONDS_IN_DAY : edge.arrival_time;
            if(new_time < neighbour.time_traveled){
                neighbour.time_traveled = new_time;
                neighbour.prev = current;
                neighbour.edge_taken = &edge;
                pq.push(&neighbour);
            }

        }
    }

    std::vector<Edge> path;
    current = &trackers[to];
    
    if (current->time_traveled == UINT_MAX) {
        return path;  // No path found
    }

    while (current->prev != nullptr) {
        path.push_back(*current->edge_taken);
        current = current->prev;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}