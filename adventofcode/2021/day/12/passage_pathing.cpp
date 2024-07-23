#include <iostream>
#include <set>
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>

using std::string;
using std::vector;

namespace dts {

struct graph {
    std::unordered_map<string, vector<string>> adj;

    void add_edge(const string& u, const string& v) {
        if (adj.contains(u))
            adj[u].push_back(v);
        else
            adj.emplace(u, vector{v});
    }
};

}

void search(const string& node, dts::graph& graph);

int main() {
    string line;
    dts::graph graph;
    while (std::getline(std::cin, line)) {
        const auto pos = line.find('-');
        const string u = line.substr(0, pos);
        const string v = line.substr(pos + 1);
        graph.add_edge(u, v);
        graph.add_edge(v, u);
    }
    search("start", graph);
}

int search(const string& node, dts::graph& graph, vector<string>& path,
           std::set<string>& seen_small_caves, std::optional<string> small_cave_twice) {
    int num_paths{0};
    path.push_back(node);
    if (node == "end") {
        path.pop_back();
        return 1;
    }
    if ('a' <= node.at(0) and node.at(0) <= 'z') {
        if (seen_small_caves.contains(node))
            // small_cave_twice must be occupied
            small_cave_twice = node;
        else
            seen_small_caves.insert(node);
    }
    for (const string& next_node : graph.adj.at(node)) {
        if (next_node == "start") continue;
        if (seen_small_caves.contains(next_node) and small_cave_twice.has_value()) continue;
        num_paths += search(next_node, graph, path, seen_small_caves, small_cave_twice);
    }
    if (small_cave_twice.has_value() and small_cave_twice.value() == node)
        small_cave_twice = std::nullopt;
    else
        seen_small_caves.erase(node);
    path.pop_back();
    return num_paths;
}

void search(const string& node, dts::graph& graph) {
    std::set<string> seen_small_caves;
    vector<string> path;
    std::cout << search(node, graph, path, seen_small_caves, std::nullopt) << '\n';
}
