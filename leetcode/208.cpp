#include <array>
#include <cassert>
#include <queue>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

namespace dts {

class trie {
public:
    trie() : data{} {}

    ~trie()
    {
        std::vector<node const *> to_delete;
        std::queue<node const *> q;
        for (node const * a_node : data) if (a_node) {
            to_delete.push_back(a_node);
            q.push(a_node);
        }
        while(!q.empty()) {
            node const * current_node = q.front();
            q.pop();
            if(!current_node) continue;
            for (node const * next_node : current_node->nexts) if (next_node) {
                to_delete.push_back(next_node);
                q.push(next_node);
            }
        }
        for (node const * a_node : to_delete) delete a_node;
    }

    void insert(std::string_view word)
    {
        next_level_t* ptr = &data;
        for (size_t i = 0; i < word.length(); i++) {
            if (!(*ptr)[static_cast<size_t>(word[i] - 'a')])
              (*ptr)[static_cast<size_t>(word[i] - 'a')] = new node();
            if (i == word.length() - 1) break;
            ptr = &((*ptr)[static_cast<size_t>(word[i] - 'a')])->nexts;
        }
        ((*ptr)[static_cast<size_t>(word[word.length() - 1] - 'a')])->leaf = true;
    }

    bool contains(std::string_view word)
    {
        next_level_t* ptr = &data;
        for (size_t i = 0; i < word.length(); i++) {
            if (!(*ptr)[static_cast<size_t>(word[i] - 'a')]) return false;
            if (i == word.length() - 1) break;
            ptr = &((*ptr)[static_cast<size_t>(word[i] - 'a')])->nexts;
        }
        return ((*ptr)[static_cast<size_t>(word[word.length() - 1] - 'a')])->leaf;
    }

    bool starts_with(std::string_view prefix)
    {
        next_level_t* ptr = &data;
        for (size_t i = 0; i < prefix.length(); i++) {
            if (!(*ptr)[static_cast<size_t>(prefix[i] - 'a')]) return false;
            ptr = &((*ptr)[static_cast<size_t>(prefix[i] - 'a')])->nexts;
        }
        return true;
    }

private:
    struct node;
    using next_level_t = std::array<node*, 'z' - 'a' + 1>;
    struct node {
        node() : nexts{}, leaf{false} {}
        next_level_t nexts;
        bool leaf; };  /* struct node */

    next_level_t data; };  /* struct trie */ }  // namespace dts

TEST(trie, AppleApp)
{
    dts::trie trie;
    trie.insert("apple");
    EXPECT_TRUE(trie.contains("apple"));
    EXPECT_FALSE(trie.contains("app"));
    EXPECT_TRUE(trie.starts_with("app"));
    trie.insert("app");
    EXPECT_TRUE(trie.contains("app"));
    EXPECT_TRUE(trie.starts_with("app"));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
