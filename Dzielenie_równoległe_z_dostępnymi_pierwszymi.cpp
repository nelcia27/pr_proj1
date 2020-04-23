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

void dzielenie_z_gotowymi_pierwszymi_omp(bool* tab, std::vector <int>* pierwsze, int from, int to) {
    omp_set_num_threads(4);
#pragma omp parallel 
    {
#pragma omp for schedule(guided)
        for (int i = 0; i < to - from; i++) {
            for (int j = 0; j < pierwsze->size(); j++) {
                if ((i + from) % pierwsze->at(j) == 0) {
                    tab[i] = false;
                    break;
                }
            }
        }
    }
}

int main() {
    bool wyswietlac = false;

    int from = 10;
    int to = 200000000;

    bool* tab = create(to - from);
    bool* tab1 = create(to - from);

    std::vector<int>* pierwsze = dzielenie_przygotowanie_tablicy_pierwszych(tab, 0, floor(sqrt(to)) + 1);
    dzielenie_z_gotowymi_pierwszymi_omp(tab1, pierwsze, from, to);

    if (wyswietlac)
        print(tab1, from, to);

    delete[] tab;
    delete[] tab1;
    return 0;
}