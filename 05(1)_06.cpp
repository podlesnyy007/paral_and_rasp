#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

// Функция f(x, y, z)
double f(double x, double y, double z) {
    return cos(x + y + z) / (exp(x + y + z) + 1);
}

int main() {
    setlocale(LC_ALL, "Rus");
    int N = 1000; // Число разбиений
    double h = 1.0 / N; // Шаг сетки
    double B = 0.0; // Переменная для хранения суммы значений функции

    // Параллельный цикл с динамической балансировкой
#pragma omp parallel for reduction(+:B) schedule(dynamic)
    for (int i = 0; i < N; i++) {
        double x = h * i;
        for (int j = 0; j < N; j++) {
            double y = h * j;
            for (int k = 0; k < N; k++) {
                double z = h * k;
                if (y * y + z * z <= x * x) { // Условие принадлежности точек конусу
                    B += f(x, y, z);
                }
            }
        }
    }

    B *= h * h * h; // Умножаем на объем элементарного куба
    cout << "Среднее значение функции: " << B << endl;

    return 0;
}
