#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

const int N = 100; // Количество точек по каждой оси
const double h = 1.0 / N; // Шаг сетки

// Определяем функцию f(x, y, z)
double f(double x, double y, double z) {
    return 0.5 + sin(x * x + y * y) * cos(z);
}

// Функция для вычисления среднего значения с заданной стратегией
double compute_average(int schedule_type) {
    double sum = 0.0; // Переменная для хранения суммы значений функции
    int count = 0; // Счетчик точек

    // Параллельный цикл с редукцией для суммирования значений
#pragma omp parallel for reduction(+:sum, count) schedule(static)
    for (int i = 0; i <= N; ++i) {
        double x = h * i; // Вычисляем координату x
        for (int j = 0; j <= N - i; ++j) {
            double y = h * j; // Вычисляем координату y
            double max_z = sqrt((N - i) * (N - i) - j * j); // Максимальное значение z для текущих x и y
            for (int k = -static_cast<int>(max_z); k <= static_cast<int>(max_z); ++k) {
                double z = h * k; // Вычисляем координату z
                sum += f(x, y, z); // Добавляем значение функции к сумме
                count++; // Увеличиваем счетчик
            }
        }
    }

    return sum / count; // Возвращаем среднее значение
}

int main() {
    setlocale(LC_ALL, "Rus");

    double results[3]; // Массив для хранения результатов

    // Измеряем время выполнения с балансировкой нагрузки по умолчанию
    auto start_default = std::chrono::high_resolution_clock::now();
    results[0] = compute_average(0); // Вызов функции с настройкой по умолчанию
    auto end_default = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> default_duration = end_default - start_default;

    // Измеряем время выполнения с статической балансировкой нагрузки
    auto start_static = std::chrono::high_resolution_clock::now();
    results[1] = compute_average(1); // Вызов функции с статической настройкой
    auto end_static = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> static_duration = end_static - start_static;

    // Измеряем время выполнения с динамической балансировкой нагрузки
    auto start_dynamic = std::chrono::high_resolution_clock::now();
    results[2] = compute_average(2); // Вызов функции с динамической настройкой
    auto end_dynamic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dynamic_duration = end_dynamic - start_dynamic;

    // Вывод результатов на экран
    std::cout << "Результат расписания по умолчанию: " << results[0] << ", Время: " << default_duration.count() << " секунды\n";
    std::cout << "Результат статического расписания: " << results[1] << ", Время: " << static_duration.count() << " секунды\n";
    std::cout << "Результат динамического расписания: " << results[2] << ", Время: " << dynamic_duration.count() << " секунды\n";

    // Вычисляем ускорения
    double speedup_static = default_duration.count() / static_duration.count();
    double speedup_dynamic = default_duration.count() / dynamic_duration.count();

    // Вывод ускорений на экран
    std::cout << "Ускорение (статическое): " << speedup_static << "\n";
    std::cout << "Ускорение (динамическое): " << speedup_dynamic << "\n";

    return 0;
}
