#include <algorithm>
#include <bitset>
#include <vector>

using namespace std;

class Solution {

public:
    int deckSize;
    static constexpr int N{1000};
    bitset<N> occupiedIndices;

    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        bitset<N> occupiedIndices = bitset<N>();
        deckSize = static_cast<int>(deck.size());
        ranges::sort(deck);
        vector<int> solution(deck.size());
        int i{0};
        for (auto const card : deck) {
            solution[i] = card;
            i = occupyIndex(i);
            if (i != -1) i = nextIndex(i);
        }
        return solution;
    }

    int occupyIndex(int const i) {
        // Using N-1-i instead of i in the bitset for printing
        // to console convenience as in this way the occupied bits
        // are printed on the left of the screen with the padding on the right
        //(bitset.set(0) e.g. with N=8 -> 00000001).
        occupiedIndices.set(N-1-i);
        if (occupiedIndices.count() == deckSize) return -1;
        return nextIndex(i);
    }

    int nextIndex(int const i) {
        int next = (i + 1) % deckSize;
        while (occupiedIndices.test(N-1-next))
            next = (next + 1) % deckSize;
        return next;
    }
};

//TODO add test/ex
int main() {}
