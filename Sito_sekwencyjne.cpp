#include <iostream>
#include <cmath>
#include <vector>

bool* create(int ile) {
    bool* tab = new bool[ile];
    for (int i = 0; i < ile; i++)
        tab[i] = true;
    return tab;
}

int print(const bool* tab, int from, int to) {
    int counter = 0;
    int counter_glob = 0;
    for (int i = 0; i < to - from; i++) {
        if (tab[i]) {
            counter++;
            counter_glob++;
            std::cout << i + from << " ";
        }
        if (counter == 10) {
            counter = 0;
            std::cout << "\n";
        }
    }
    std::cout << "\n";
    std::cout << "W zakresie od " << from << " do " << to << " znaleziono " << counter_glob << " liczb pierwszych.\n";

    return counter_glob;
}

void sito(bool* tab, int from, int to) {
    std::vector<int> wyk;
    for (int i = 2; i <= (int)sqrt(to); i++) {
        wyk.push_back(i);
    }
    for (int i = 0; i < wyk.size(); i++) {
        for (int j = 2; j < to / wyk[i] + 1; j++) {
            if (j * wyk[i] - from + 1 > 0)
                tab[j * wyk[i] - from] = false;
        }
    }
}

int main() {
    bool wyswietlac = true;

    int from = 10;
    int to = 49;

    bool* tab = create(to - from);

    sito(tab, from, to);

    if (wyswietlac)
        print(tab, from, to);

    delete[] tab;
    return 0;
}