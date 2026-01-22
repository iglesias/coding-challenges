#include "Trace.h"

#include "VisNode.h"

#include <iostream>

std::vector<VisNode*> Trace::nodes_{};
std::vector<int> Trace::current_stack_{};
bool Trace::enabled_ = false;

void Trace::register_node(VisNode* n) {
    nodes_.push_back(n);
}

void Trace::snapshot(int line) {
    if (!enabled_) return;
    TreeState s;
    s.line = line;
    s.nodes.reserve(nodes_.size());
    for (auto* n : nodes_) {
        NodeState ns;
        ns.id = n->id;
        ns.value = n->value;
        ns.highlighted = n->highlighted;
        ns.size = n->size;
        ns.left_id = n->left ? n->left->id : -1;
        ns.right_id = n->right ? n->right->id : -1;
        ns.x = n->x;
        ns.y = n->y;
        s.nodes.push_back(ns);
    }
    s.call_stack = current_stack_;
    std::cout << nlohmann::json(s).dump() << std::endl;
}

void Trace::push_stack(int id) {
    current_stack_.push_back(id);
}

void Trace::pop_stack() {
    PRECONDITION(current_stack_.size());
    current_stack_.pop_back();
}

void Trace::enable() { enabled_ = true; }
void Trace::disable() { enabled_ = false; }

void Trace::clear() {
    current_stack_.clear();
}
