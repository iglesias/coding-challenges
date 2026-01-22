#pragma once

#include <vector>

#include <nlohmann/json.hpp>

/*

Right now, the class is doing two conceptually distinct jobs:

Maintaining a registry of live visual objects (nodes).
Building and storing execution snapshots (the trace).

From a design purity standpoint:

Registry →  owns and tracks objects.
Trace / Recorder / Timeline →  produces and stores states.

Registry = object set
Recorder = snapshot history

struct Registry { static std::vector<VisNode*> nodes; };
struct Trace { static void snapshot(const Registry&); };

*/

struct TreeState;
struct VisNode;

struct Trace {
    static void register_node(VisNode* n);
    static void snapshot(int line);
    static void push_stack(int id);
    static void pop_stack();
    static void enable();
    static void disable();
    /*
    There are two different notions of “clear”:

    Clear the timeline → remove past snapshots and stack.

    Clear the registry → forget what objects exist.

    For your use case, after layout:

    You want to clear only the timeline, not the registry.
    */
    static void clear();

private:
    static std::vector<VisNode*> nodes_;
    static std::vector<int> current_stack_;
    static bool enabled_;
};

struct NodeState {
    int id;
    int value;
    bool highlighted;
    int size;
    int left_id;   // -1 if null
    int right_id;  // -1 if null
    int x;
    int y;
};

struct TreeState {
    std::vector<NodeState> nodes;
    std::vector<int> call_stack;  // node ids currently on stack
    int line;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NodeState, id, value, highlighted, size, left_id, right_id, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TreeState, nodes, call_stack, line);
