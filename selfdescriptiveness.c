// Функции оценки информативности

#include <math.h>

// Pc - число объектов класса с в выборке (c_elements_count)
// pc(f) - из них число объектов, для которых выполняется условие f(x) = 1 (c_f_elements_count)
// Nc - число объектов всех остальных классов (nc_elements_count)
// nc(f) - из них число объектов, на которых выполняется условие f(x) = 1 (nc_f_elements_count)

// Эвристическая оценка информативности
float heuristic_descriptiveness(int c_elements_count, 
                                int c_f_elements_count,
                                int nc_elements_count,
                                int nc_f_elements_count) {
    return 0;
}

// Статистическая оценка информативности
float statistical_descriptiveness(int c_elements_count, 
                                  int c_f_elements_count,
                                  int nc_elements_count,
                                  int nc_f_elements_count) {
    return log_fact(c_f_elements_count + nc_f_elements_count)
           + log_fact(c_elements_count + nc_elements_count - c_f_elements_count - nc_f_elements_count)
           - log_fact(c_elements_count + nc_elements_count)
           - log_fact(c_f_elements_count)
           - log_fact(c_elements_count - c_f_elements_count)
           + log_fact(c_elements_count)
           - log_fact(nc_f_elements_count)
           - log_fact(nc_elements_count - nc_f_elements_count)
           + log_fact(nc_elements_count);
}

// Энтропийная оценка информативности
float entropic_descriptiveness(int c_elements_count, 
                                  int c_f_elements_count,
                                  int nc_elements_count,
                                  int nc_f_elements_count) {
    return 0;
}


// Вычисление энтропии выборки, где  
float selection_entropy(int c_elements_count, int nc_elements_count) {
    return 0;
}

float log2(float n) {
    return log(n) / log(2); // через замену основания логарифма
}

// Определим таблицу с логарифмами всех факториалов от 1 до n. 
// Не стоит использовать для больших n. Специально сделал в базовой версии 
// возможность определения только одной такой таблицы.
int log_factorials_table_defined;
float* log_factorials_table;
int define_log_table(int n) {
    if (!log_factorials_table_defined) {
        log_factorials_table_defined = 1;
        log_factorials_table = malloc(n * sizeof(float));
        log_factorials_table[0] = 0;
        // Вот тут можно запутаться - т.к. в рекуррентной формуле логарифмов 
        // факториалов считаются элементы с 0-го, а у нас с 1-го
        for (int i = 1; i <= n; i++) {
            log_factorials_table[i] = log_factorials_table[i - 1] + log(i + 1);
        }
    }
}

// Реализация вычисления логарифма от факториала по формуле Стирлинга если n > 10,
// и рекурсивно, если n <= 10 (даёт точность до 10-го знака)
float log_fact(int n) {
    if (n <= 10) {
        define_log_table(10);
        return log_factorials_table[n - 1];
    } else {
        // Тут используем формулу Стирлинга. 
        // С ростом n точность вычислений повышается
        return (n + 1/2) * log(n) - n + 1/2 * log(2 * M_PI) + 1/(12 * n) - 1/(360 * n^2);
    }
}