#include <iostream>
#include <cmath>
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

void dzielenie_omp(bool* tab, int from, int to) {
    omp_set_num_threads(4);
    int dzielnik = 2;
#pragma omp parallel 
    {
#pragma omp for schedule(guided)
        for (int i = 0; i < to - from; i++) {
            dzielnik = 2;
            while (dzielnik < floor(sqrt(i + from)) + 1) {
                if ((i + from) % dzielnik == 0) {
                    tab[i] = false;
                    break;
                }
                dzielnik++;
            }
        }
    }
}

int main() {
    bool wyswietlac = false;

    int from = 2;
    int to = 20000000;

    bool* tab = create(to - from);
    
    dzielenie_omp(tab, from, to);
    
    if(wyswietlac)
        print(tab, from, to);

    delete[] tab;
 
    return 0;
}