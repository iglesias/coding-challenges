#include <iostream>
#include <stack>
#include <string>
#include <vector>

int score(char c) {
    switch (c) {
        case ')':
            return 3;
        case ']':
            return 57;
        case '}':
            return 1197;
        default:
            return 25137;
    }
}

bool match(char c, char d) {
    if (c == ')')
        return d == '(';
    else if (c == ']')
        return d == '[';
    else if (c == '}')
        return d == '{';
    else
        return d == '<';
}

int main() {
    std::string line;
    int part_one{0};
    std::vector<uint64_t> autocomplete_scores;
    while (std::getline(std::cin, line)) {
        std::stack<char> S;
        for (const char c : line)
            if (c == '(' or c == '[' or c == '{' or c == '<') {
                S.push(c);
            } else {
                if (match(c, S.top())) {
                    S.pop();
                } else {
                    part_one += score(c);
                    while (!S.empty()) S.pop();
                    break;
                }
            }
        if (!S.empty()) autocomplete_scores.push_back(0);
        while (not S.empty()) {
            autocomplete_scores[autocomplete_scores.size() - 1] *= 5;
            if (S.top() == '(')
                autocomplete_scores[autocomplete_scores.size() - 1] += 1;
            else if (S.top() == '[')
                autocomplete_scores[autocomplete_scores.size() - 1] += 2;
            else if (S.top() == '{')
                autocomplete_scores[autocomplete_scores.size() - 1] += 3;
            else if (S.top() == '<')
                autocomplete_scores[autocomplete_scores.size() - 1] += 4;
            S.pop();
        }
    }
    std::cout << part_one << '\n';
    std::nth_element(autocomplete_scores.begin(), autocomplete_scores.begin() + autocomplete_scores.size() / 2, autocomplete_scores.end());
    std::cout << autocomplete_scores[autocomplete_scores.size() / 2] << '\n';
}
