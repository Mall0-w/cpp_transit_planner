#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>
#include "transitgraph.hpp"

class GTFSHandler{
    private:
        std::unique_ptr<TransitGraph> g;
        static const std::vector<std::string> requiredFiles;

        static const bool checkValidGTFSLayout(const std::string& folderpath);

        void loadStops(const std::string& folderpath, std::unique_ptr<TransitGraph> graph);

    public:
        GTFSHandler() : g(std::make_unique<TransitGraph>()) {}

        inline
        TransitGraph* getGraph(){
            return g.get();
        }

        //given turns gtfs folder into a graph given path to folder.
        void loadGraph(const std::string& folderpath);
};