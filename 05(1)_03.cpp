#include <iostream>
#include <cmath>
#include <omp.h> // Библиотека OpenMP для параллельных вычислений
#include <chrono> // Библиотека для работы с временем

const int N = 100; // Количество точек по каждой оси
const double h = 1.0 / N; // Шаг

// Функция f(x, y, z)
double f(double x, double y, double z) {
    return 0.5 + sin(x * x + y * y) * cos(z);
}

// Функция для последовательного вычисления
double sequential_computation() {
    double A = 0.0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                double x = h * (i + 0.5);
                double y = h * (j + 0.5);
                double z = h * (k + 0.5);
                A += f(x, y, z); // Вызываем функцию f и добавляем результат к A
            }
        }
    }
    return A * h * h * h; // Умножаем на объем (h^3)
}

// Функция для параллельного вычисления с разными стратегиями
double parallel_computation(const char* schedule_type) {
    double A = 0.0;

    // Параллельный цикл с редукцией для суммирования значений A
    if (strcmp(schedule_type, "default") == 0) {
#pragma omp parallel for reduction(+:A)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    double x = h * (i + 0.5);
                    double y = h * (j + 0.5);
                    double z = h * (k + 0.5);
                    A += f(x, y, z);
                }
            }
        }
    }
    else if (strcmp(schedule_type, "static") == 0) {
#pragma omp parallel for reduction(+:A) schedule(static)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    double x = h * (i + 0.5);
                    double y = h * (j + 0.5);
                    double z = h * (k + 0.5);
                    A += f(x, y, z);
                }
            }
        }
    }
    else if (strcmp(schedule_type, "dynamic") == 0) {
#pragma omp parallel for reduction(+:A) schedule(dynamic)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    double x = h * (i + 0.5);
                    double y = h * (j + 0.5);
                    double z = h * (k + 0.5);
                    A += f(x, y, z);
                }
            }
        }
    }

    return A * h * h * h; // Умножаем на объем
}

int main() {
    setlocale(LC_ALL, "Rus");

    // Измеряем время последовательного вычисления
    auto start_seq = std::chrono::high_resolution_clock::now(); // Запоминаем время начала
    double sequential_result = sequential_computation(); // Выполняем последовательные вычисления
    auto end_seq = std::chrono::high_resolution_clock::now(); // Запоминаем время окончания
    std::chrono::duration<double> seq_duration = end_seq - start_seq; // Вычисляем продолжительность

    // Измеряем время параллельного вычисления по умолчанию
    auto start_par_default = std::chrono::high_resolution_clock::now();
    double parallel_result_default = parallel_computation("default");
    auto end_par_default = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> par_default_duration = end_par_default - start_par_default;

    // Измеряем время параллельного вычисления статически
    auto start_par_static = std::chrono::high_resolution_clock::now();
    double parallel_result_static = parallel_computation("static");
    auto end_par_static = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> par_static_duration = end_par_static - start_par_static;

    // Измеряем время параллельного вычисления динамически
    auto start_par_dynamic = std::chrono::high_resolution_clock::now();
    double parallel_result_dynamic = parallel_computation("dynamic");
    auto end_par_dynamic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> par_dynamic_duration = end_par_dynamic - start_par_dynamic;

    // Вычисляем ускорение
    double speedup_default = seq_duration.count() / par_default_duration.count();
    double speedup_static = seq_duration.count() / par_static_duration.count();
    double speedup_dynamic = seq_duration.count() / par_dynamic_duration.count();

    // Вывод результатов
    std::cout << "Последовательный результат: " << sequential_result << ", Время: " << seq_duration.count() << " секунды\n";
    std::cout << "Параллельный результат (по умолчанию) " << parallel_result_default << ", Время: " << par_default_duration.count() << " секунды, Ускорение: " << speedup_default << "\n";
    std::cout << "Параллельный результат (статический):  " << parallel_result_static << ", Время: " << par_static_duration.count() << " секунды, Ускорение: " << speedup_static << "\n";
    std::cout << "Параллельный результат (динамический): " << parallel_result_dynamic << ", Время: " << par_dynamic_duration.count() << " секунды, Ускорение " << speedup_dynamic << "\n";

    return 0;
}
