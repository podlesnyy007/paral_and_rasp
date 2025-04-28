#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

// Функция f(x, y, z)
double f(double x, double y, double z) {
    return (z * exp(x + y)) / cos(x + y + z);
}

int main() {
    setlocale(LC_ALL, "Rus");
    int N = 1500; // Число разбиений
    double h = 1.0 / N; // Шаг сетки
    double A = 0.0; // Переменная для хранения суммы значений функции

    // Параллельный цикл со статической балансировкой
#pragma omp parallel for reduction(+:A) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                double x = h * i;
                double y = h * j;
                double z = h * k;
                A += f(x, y, z);
            }
        }
    }

    A *= h * h * h; // Умножаем на объем элементарного куба
    cout << "Среднее значение функции: " << A << endl;

    return 0;
}
