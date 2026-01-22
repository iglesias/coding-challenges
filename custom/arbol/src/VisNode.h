#pragma once

#include <cassert>

#include "Trace.h"

#define PRECONDITION(cond) assert(cond)

struct VisNode {
    int id;
    int value;
    VisNode* left = nullptr;
    VisNode* right = nullptr;

    bool highlighted = false;
    int size = 0;

    // layout
    int x = 0;
    int y = 0;

    VisNode(int id_, int value_);

    void set_left(VisNode* n, int line = -1);
    void set_right(VisNode* n, int line = -1);

    void highlight(int line = -1);
    void unhighlight(int line = -1);

    void set_size(int sz, int line = -1);
};

