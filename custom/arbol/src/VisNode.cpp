#include "VisNode.h"

VisNode::VisNode(int id_, int value_) : id(id_), value(value_) {
    Trace::register_node(this);
}

void VisNode::set_left(VisNode* n, int line) {
    PRECONDITION(n != this);
    left = n;
    Trace::snapshot(line);
}

void VisNode::set_right(VisNode* n, int line) {
    PRECONDITION(n != this);
    right = n;
    Trace::snapshot(line);
}

void VisNode::highlight(int line) {
    highlighted = true;
    Trace::snapshot(line);
}

void VisNode::unhighlight(int line) {
    highlighted = false;
    Trace::snapshot(line);
}

void VisNode::set_size(int sz, int line) {
    size = sz;
    Trace::snapshot(line);
}
