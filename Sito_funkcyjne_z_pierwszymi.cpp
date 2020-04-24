#include <iostream>
#include <cmath>
#include <vector>
#include <omp.h>


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

std::vector<int>* dzielenie_przygotowanie_tablicy_pierwszych(bool* tab, int from, int to) {
    std::vector < int >* pierwsze = new std::vector<int>;
    int dzielnik = 2;
    int j = 0;
    for (int i = from; i < to; i++) {
        dzielnik = 2;
        while (dzielnik < floor(sqrt(i)) + 1) {
            if (i % dzielnik == 0) {
                tab[i] = false;
                break;
            }
            dzielnik++;
        }
        if (tab[i] == true) {
            if (i != 0 && i != 1)
                pierwsze->push_back(i);
            if (j <= to)
                j++;
            else
                break;
        }
    }

    return pierwsze;
}

void sito_funkcyjne(bool* tab, std::vector<int>* pierwsze, int from, int to) {
    int th = 4;
    omp_set_num_threads(th);

    std::vector<int> wyk;
    for (int i = 2; i <= (int)sqrt(to); i++) {
        wyk.push_back(i);
    }

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int wiel = pierwsze->size() / th;
        if ((pierwsze->size()) % th > id) {
            wiel++;
        }

        int min, max;

        min = (wiel)*id;
        max = (wiel) * (id + 1);
        if ((pierwsze->size()) % th <= id) {
            min += (pierwsze->size()) % th;
            max += (pierwsze->size()) % th;
        }

        std::vector<int> p;
        for (int i = min; i < max; i++) {
            p.push_back(pierwsze->at(i));
        }


        for (int i = 0; i < p.size(); i++) {
            for (int j = 2; j < to / p.at(i) + 1; j++) {
                if (j * p.at(i) - from + 1 > 0)
                    tab[j * p.at(i) - from] = false;
            }
        }
    }

}

int main() {
    bool wyswietlac = true;

    int from = 10;
    int to = 49;

    bool* tab = create(floor(sqrt(to)) + 1);
    bool* tab1 = create(to - from);

    std::vector<int>* pierwsze = dzielenie_przygotowanie_tablicy_pierwszych(tab, 0, floor(sqrt(to)) + 1);
    sito_funkcyjne(tab1, pierwsze, from, to);

    if (wyswietlac)
        print(tab1, from, to);

    delete[] tab;
    delete[] tab1;
    return 0;
}