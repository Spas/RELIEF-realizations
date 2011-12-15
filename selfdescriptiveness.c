// Функции оценки информативности

#include <math.h>
#include "selfdescriptiveness.h"

//TODO: добавить взвешенную информативность (страница 8 лекций)


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

// Обобщение статистической оценки информативности до случая произвольного числа классов.
// TESTED 07.12.2011
float statistical_descriptiveness_full(int* c_elements_count, int* c_f_elements_count,
                                       int classes_count,
                                       int c_total_elements, int c_f_total_elements) {
    float result = 0;
    int i = 0;
   
    if ((c_total_elements == -1) || (c_f_total_elements == -1)) {
        c_total_elements = 0;
        c_f_total_elements = 0;
        while (i < classes_count) {
            printf("c_elements_count[%d] = %d\n", i, c_elements_count[i]);
            printf("c_f_elements_count[%d] = %d\n", i, c_f_elements_count[i]);
            c_total_elements += c_elements_count[i];
            c_f_total_elements += c_f_elements_count[i];
            result -= log_fact(c_elements_count[i])
                    - log_fact(c_f_elements_count[i]) 
                    - log_fact(c_elements_count[i] - c_f_elements_count[i]);
            i++;
        }
    } else {
        while (i < classes_count) {
            result -= log_fact(c_elements_count[i])
                    - log_fact(c_f_elements_count[i]) 
                    - log_fact(c_elements_count[i] - c_f_elements_count[i]);
            i++;
        }
    }
    result -= log_fact(c_f_total_elements) 
            + log_fact(c_total_elements - c_f_total_elements)
            - log_fact(c_total_elements);
    return result;
}

// Статистическая оценка информативности
// TESTED 07.12.2011
float statistical_descriptiveness(int c_elements_count, 
                                  int c_f_elements_count,
                                  int nc_elements_count,
                                  int nc_f_elements_count) {
    return - log_fact(c_f_elements_count + nc_f_elements_count)
           - log_fact(c_elements_count + nc_elements_count - c_f_elements_count - nc_f_elements_count)
           + log_fact(c_elements_count + nc_elements_count)
           + log_fact(c_f_elements_count)
           + log_fact(c_elements_count - c_f_elements_count)
           - log_fact(c_elements_count)
           + log_fact(nc_f_elements_count)
           + log_fact(nc_elements_count - nc_f_elements_count)
           - log_fact(nc_elements_count);
}

// Обобщение энтройпийно оценки информативности до случая произвольного числа классов.
// Странно, но возвращает результат, отличный от оценок для IGain 
// в таблице 1 на странице 4 www.ccas.ru/voron/download/LogicAlgs.pdf
// BAD_TEST 07.12.2011
float entropic_descriptiveness_full(int* c_elements_count, int* c_f_elements_count,
                                    int classes_count, 
                                    int c_total_elements, int c_f_total_elements) {
    int i;
    float d;
    float el1 = 0, el2 = 0, el3 = 0;
    if ((c_total_elements == -1) || (c_f_total_elements == -1)) {
        i = 0;
        c_total_elements = 0;
        c_f_total_elements = 0;
        while (i < classes_count) {
            c_total_elements += c_elements_count[i];
            c_f_total_elements += c_f_elements_count[i];
            i++;
        }
    }
    
    
    i = 0;
    while (i < classes_count) {
        d = ((float)c_elements_count[i])/c_total_elements;
        if (d != 0) {
            el1 -= d * log2f(d);
        }
        d = ((float)c_f_elements_count[i])/c_f_total_elements;
        if (d != 0) {
            el2 -= d * log2f(d);
        }

        d = ((float)(c_elements_count[i] - c_f_elements_count[i]))/(c_total_elements - c_f_total_elements);
        if (d != 0) {
            el3 -= d * log2f(d);
        }
        i++;
    }
    
    return el1 - ((float)c_f_total_elements)/c_total_elements * el2 
               - ((float)(c_total_elements - c_f_total_elements))/c_total_elements * el3;
}

// Энтропийная оценка информативности. Формулы вроде все точно записаны.
// Странно, но возвращает результат, отличный от оценок для IGain 
// в таблице 1 на странице 4 www.ccas.ru/voron/download/LogicAlgs.pdf
// BAD_TEST 07.12.2011
float entropic_descriptiveness(int c_elements_count, 
                               int c_f_elements_count,
                               int nc_elements_count,
                               int nc_f_elements_count) {
    return selection_entropy(c_elements_count, nc_elements_count)
           - ((float)(c_f_elements_count + nc_f_elements_count)) / (c_elements_count + nc_elements_count)
           * selection_entropy(c_f_elements_count, nc_f_elements_count)
           - (1 - ((float)(c_f_elements_count + nc_f_elements_count)) / (c_elements_count + nc_elements_count))
           * selection_entropy(c_elements_count - c_f_elements_count, nc_elements_count - nc_f_elements_count);
}


// Вычисление энтропии выборки. Стоит ли выделять 1 переменную для 
// промежуточных значений c_part = c_elements_count/(c_elements_count + nc_elements_count) 
// Проверено при 100000 вызовов.
// Такое вычисление даёт выигрыш по времени примерно в 2 раза (по сравнению с записью полной формулы)
// TEST 07.12.2011
float selection_entropy(int c_elements_count, int nc_elements_count) {
    float c_part = ((float)c_elements_count)/(c_elements_count + nc_elements_count);
    // Если одно из событий случается со 100% вероятностью, то вероятность 
    // второго равна 0% и его как бы нету в системе.
    if ((c_part == 1) || (c_part == 0)) {
        return 0;
    } else {
        return - c_part * log2f(c_part) - (1 - c_part) * log2f(1 - c_part);
    }
}


// Определим таблицу с логарифмами всех факториалов от 1 до n. 
// Не стоит использовать для больших n. Специально сделал в базовой версии 
// возможность определения только одной такой таблицы.
int log_fact_table_defined;
float* log_fact_table;

// TESTED 07.12.2011
int define_log_fact_table(int n) {
    if (!log_fact_table_defined) {
        log_fact_table_defined = 1;
        log_fact_table = malloc(n * sizeof(float));
        log_fact_table[0] = 0;
        // Вот тут можно запутаться - т.к. в рекуррентной формуле логарифмов 
        // факториалов считаются элементы с 0-го, а у нас с 1-го
        int i = 1;
        while (i < n) {
            log_fact_table[i] = log_fact_table[i - 1] + log(i + 1);
            i++;
        }
    }
}

int clear_log_fact_table() {
    free(log_fact_table);
}

// Реализация вычисления логарифма от факториала по формуле Стирлинга если n > 10,
// и рекурсивно, если n <= 10 (даёт точность до 10-го знака)
// TESTED 07.12.2011
float log_fact(int n) {
    float result = 0;
    if (n <= 10) {
        define_log_fact_table(10);
        result = log_fact_table[n - 1];
    } else {
        // Тут используем формулу Стирлинга. 
        // С ростом n точность вычислений повышается
        result = (n + 1.0/2) * log(n) - n + log(2 * M_PI) / 2 + 1.0/(12 * n) - 1.0/(360 * n^2);
    }
    return result;
}

// TESTED 07.12.2011
float log2f(float n) {
    return log(n) / M_LN2;
}