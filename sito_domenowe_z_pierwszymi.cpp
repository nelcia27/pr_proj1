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

void sito_domenowe(bool* tab, std::vector<int>* wyk, int from, int to) {
    int th = 4;
    omp_set_num_threads(th);

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int wiel = (to - from) / th;
        if ((to - from) % th > id) {
            wiel++;
        }
        bool* p_tab = new bool[wiel];
        for (int i = 0; i < wiel; i++) {
            p_tab[i] = true;
        }

        int min, max;

        min = from + (wiel)*id;
        max = from + (wiel) * (id + 1);
        if ((to - from) % th <= id) {
            min += (to - from) % th;
            max += (to - from) % th;
        }
        for (int i = 0; i < wyk->size(); i++) {
            for (int j = min / wyk->at(i); j < max / wyk->at(i) + 1; j++) {
                if (j * wyk->at(i) - min >= 0 && j * wyk->at(i) != wyk->at(i) && j * wyk->at(i) - min < wiel)
                    p_tab[j * wyk->at(i) - min] = false;
            }
        }
        for (int i = 0; i < wiel; i++) {
            tab[i + min - from] = p_tab[i];
        }
        delete[] p_tab;
    }

}

int main() {
    bool wyswietlac = true;

    int from = 10;
    int to = 49;

    bool* tab = create(floor(sqrt(to)) + 1);
    bool* tab1 = create(to - from);

    std::vector<int>* pierwsze = dzielenie_przygotowanie_tablicy_pierwszych(tab, 0, floor(sqrt(to)) + 1);
    sito_domenowe(tab1, pierwsze, from, to);

    if (wyswietlac)
        print(tab1, from, to);

    delete[] tab;
    delete[] tab1;
    return 0;
}