#include <type_traits>
#include <vector>

template<typename T> concept Trivial = std::is_trivial_v<T>;

template<Trivial T> struct list_node {
    T val;
    list_node<T> *next;
};

template<Trivial T> int size(list_node<T>* node) {
    int n = 0;
    while (node) {
        n++;
        node = node->next;
    }
    return n;
}

template<Trivial T> std::vector<list_node<T>*> split(list_node<T>* head, int k) {
    const int n = size(head);
    std::vector<list_node<T>*> ans(k, nullptr);
    list_node<T>* node = head;
    if (k >= n) for (int i = 0; i < n; i++) {
        list_node<T>* const next = node->next;
        node->next = nullptr;
        ans[i] = node;
        node = next;
    } else for (int i = 0; i < k; i++) {
        list_node<T>* next = node;
        for (int j = 0; j < n / k - 1; j++) next = next->next;
        if (i < n - (n / k) * k) next = next->next;
        list_node<T>* const backup = next->next;
        next->next = nullptr;
        ans[i] = node;
        node = backup;
    }
    return ans;
}

int main() {}
