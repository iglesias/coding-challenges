#include <climits>
#include <map>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

void step(string& compound);

char rules['Z' - 'A' + 1]['Z' - 'A' + 1];

int main() {
    string Template;
    std::cin >> Template;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        const auto pos = line.find("->");
        rules[line.at(pos - 3) - 'A'][line.at(pos - 2) - 'A'] = line.at(pos + 3);
    }
    std::cout << "Template:     " << Template << '\n';
    //TODO simulation too slow for part two (40 steps) even for sample input,
    //it runs until 30 for sample input, the counter may need 64-bit int
    const int num_steps = 40;
    string compound = Template;
    for (int s = 0; s < num_steps; s++) {
        step(compound);
        //std::cout << "After step " << s + 1 << ": " << compound << '\n';
    }
    std::map<char, int> counter;
    int max = 0, min = INT_MAX;
    for (const auto& c : compound) {
        counter[c]++;
    }
    for (const auto& [_, count] : counter) {
        max = std::max(max, count);
        min = std::min(min, count);
    }
    std::cout << max - min << '\n';

}

void step(string& compound) {
    string new_characters;
    for (int i = 0; i < compound.size() - 1; i++)
        new_characters.push_back(rules[compound.at(i) - 'A'][compound.at(i + 1) - 'A']);
    string new_compound;
    for (int i = 0; i < compound.size(); i++) {
        new_compound.push_back(compound.at(i));
        if (i == compound.size() - 1) break;
        new_compound.push_back(new_characters.at(i));
    }
    compound = new_compound;
}
