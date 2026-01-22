#include <algorithm>
#include <vector>
#include <random>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "VisNode.h"

struct CallGuard {
    CallGuard(VisNode* u) { Trace::push_stack(u->id); }
    ~CallGuard() { Trace::pop_stack(); }
};

int traverse_post_order(VisNode* node) {
    PRECONDITION(node != nullptr);
    CallGuard guard(node);
    node->highlight(__LINE__);

    int sz = 1;
    if (node->left) {
        Trace::snapshot(__LINE__);
        sz += traverse_post_order(node->left);
    }
    if (node->right) {
        Trace::snapshot(__LINE__);
        sz += traverse_post_order(node->right);
    }

    node->set_size(sz, __LINE__);
    node->unhighlight(__LINE__);
    return sz;
}

void assign_layout(VisNode* u, int depth, int& next_x) {
    if (!u) return;

    assign_layout(u->left, depth + 1, next_x);

    u->x = next_x++;
    u->y = depth;

    assign_layout(u->right, depth + 1, next_x);
}

VisNode* build_balanced_bst(const std::vector<int>& values, int start, int end, int& next_id, std::vector<std::unique_ptr<VisNode>>& storage) {
    if (start > end) return nullptr;
    int mid = start + (end - start) / 2;
    auto node = std::make_unique<VisNode>(next_id++, values[mid]);
    VisNode* p_node = node.get();
    storage.push_back(std::move(node));

    p_node->set_left(build_balanced_bst(values, start, mid - 1, next_id, storage));
    p_node->set_right(build_balanced_bst(values, mid + 1, end, next_id, storage));

    return p_node;
}

int main(int argc, char** argv) {
    bool use_fixed = false;
    if (argc > 1 && std::string(argv[1]) == "--fixed") {
        use_fixed = true;
    }

    std::vector<std::unique_ptr<VisNode>> storage;
    VisNode* root = nullptr;

    // Use random device for non-deterministic behavior across runs
    std::random_device rd;
    std::mt19937 gen(rd());

    if (use_fixed) {
        auto n1 = std::make_unique<VisNode>(1, 30);
        auto n2 = std::make_unique<VisNode>(2, 20);
        auto n3 = std::make_unique<VisNode>(3, 50);
        auto n4 = std::make_unique<VisNode>(4, 10);
        auto n5 = std::make_unique<VisNode>(5, 60);
        auto n6 = std::make_unique<VisNode>(6, 40);

        n1->set_left(n2.get());
        n1->set_right(n3.get());
        n2->set_left(n4.get());
        n3->set_right(n5.get());
        n3->set_left(n6.get());

        root = n1.get();

        storage.push_back(std::move(n1));
        storage.push_back(std::move(n2));
        storage.push_back(std::move(n3));
        storage.push_back(std::move(n4));
        storage.push_back(std::move(n5));
        storage.push_back(std::move(n6));
    } else {
        std::uniform_int_distribution<> size_dist(20, 60);
        std::uniform_int_distribution<> val_dist(1, 1000);

        int const n = size_dist(gen);
        std::vector<int> values;
        for (int i = 0; i < n; ++i) values.push_back(val_dist(gen));
        std::sort(values.begin(), values.end());
        //values.erase(std::unique(values.begin(), values.end()), values.end());

        int next_id = 1;
        root = build_balanced_bst(values, 0, (int)values.size() - 1, next_id, storage);
    }

    if (root) {
        int next_x = 0;
        assign_layout(root, 0, next_x);

        Trace::clear();
        Trace::enable(); // Enable output only after layout is ready
        Trace::snapshot(__LINE__);

        traverse_post_order(root);
    }

    return 0;
}
