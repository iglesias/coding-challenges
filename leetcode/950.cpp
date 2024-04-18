#include <algorithm>
#include <bitset>
#include <cstdio>
#include <vector>

using namespace std;

class Solver {

public:
    size_t deckSize;
    static constexpr int N{1000};
    bitset<N> occupiedIndices;

    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        occupiedIndices.reset();
        deckSize = deck.size();
        ranges::sort(deck);
        vector<int> solution(deck.size());
        size_t i{0};
        for (auto const card : deck) {
            solution[i] = card;
            long int const r = occupyIndex(i);
            if (r != -1) i = nextIndex(static_cast<size_t>(r));
        }
        return solution;
    }

    long int occupyIndex(size_t const i) {
        // Using N-1-i instead of i in the bitset for printing
        // to console convenience as in this way the occupied bits
        // are printed on the left of the screen with the padding on the right
        //(bitset.set(0) e.g. with N=8 -> 00000001).
        occupiedIndices.set(N-1-i);
        if (occupiedIndices.count() == deckSize)
	    return -1;
	else
            return static_cast<long int>(nextIndex(i));
    }

    size_t nextIndex(size_t const i) {
        size_t next = (i + 1) % deckSize;
        while (occupiedIndices.test(N-1-next))
            next = (next + 1) % deckSize;
        return next;
    }
};

int main() {
    //TODO randomize input.
    vector<int> deck{17, 13, 11, 2, 3, 5, 7};
    Solver solver;
    for (auto const card : solver.deckRevealedIncreasing(deck))
        printf("%d ", card);
    printf("\n");
    //TODO print simulation revealing the deck in the right order.
}
