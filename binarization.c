#include <math.h>
#include "selfdescriptiveness.h"

//
// Бинаризация числовых данных представляет собой запись их в виде отрезков исходя из оценок информативности.
// То есть сначала необходимо определить структуру данных, описывающую отрезки и лучи.
// Возможно использование INFINITY и -INFINITY для описания лучей.
//
// Структура для хранения полных данных об отрезках (дял бинаризации)
// 
struct segment_full_f {
    float start;
    float end;
    float* values;
    // Общее количество значений, которое покрывает отрезок.
    int values_count;
    // Количество элементов отрезка, соответствующих классу class.
    int class_elements_count;
    int class;
};


// Функция для слияния тройки сегментов.
struct segment_full_f concat_full_segments(struct segment_full_f segment1,
                                           struct segment_full_f segment2,
                                           struct segment_full_f segment3) {
    struct segment_full_f result;
    if ((segment1.end == segment2.start) 
        && (segment2.end == segment3.start)) 
    {
        result.start = segment1.start;
        result.end = segment3.end;
    }
    
}

//!!! Надо ещё как-то хранить карту связей всех отрезков.

// Структура для хранения данных о параметре, значения которого разбиваются на отрезки.
struct feature_value_class {
    float feature_value;
    int class;
};

int compare_fvc(const void * a, const void * b) {
    if ((*(struct feature_value_class*)a).feature_value > (*(struct feature_value_class*)b).feature_value) {
        return 1;
    } else if ((*(struct feature_value_class*)a).feature_value == (*(struct feature_value_class*)b).feature_value) {
        return 0;
    } else {
        return -1;
    }
}

// Функция дискретизации массива значений по соответствию ему класса и функции оценки статистической информативности.
void binarize_by_stat(struct feature_value_class* feature_values, int rows_count, struct segment_full_f** result) {
    // Для разбиения на отрезки надо, по идее, отсортировать все значения. 
    // Попытка интеграции алгоритмов бинаризации и сортировки НЕ ДАСТ прироста в производительности.
    // То есть производительность результирующего алгоритма, вообще говоря, будет НЕ ЛУЧШЕ, чем
    // сортировки и последующей бинаризации по отдельности.

    qsort(feature_values, rows_count, sizeof(struct feature_value_class), compare_fvc);
    
    /* Проверка предварительной сортировки */
    int index;
    for (index = 0; index < rows_count; index++) {
        printf("(%f,%d), ", feature_values[index].feature_value, feature_values[index].class);
    }
    printf("\n");
    
    // Выделяем память под информацию разбиении значений на отрезки.
    (*result) = (struct segment_full_f*) malloc(rows_count * sizeof(struct segment_full_f));
    
    struct segment_full_f tmp_segment;
    int row_index, segment_index, segment_value_index, segment_values_count;
    
    tmp_segment.start = -INFINITY;
    
    segment_index = 0;
    segment_values_count = 0;
    
    // tmp_segment_values введен для экономии памяти - не надо для каждого отрезка 
    // под массив значений выделять память, как будто каждый отрезок содержит все элементы.
    // Сократим затраты памяти на хранение таких массивов с rows_count^2 до rows_count. 
    // 1 дополнительный проход по массиву (суммарная длинна прохода получится rows_count)
    // погоды не сделает.
    float* tmp_segment_values = malloc(rows_count * sizeof(float));
    
    for (row_index = 0; row_index < rows_count - 1; row_index++) {
        if (feature_values[row_index].class != feature_values[row_index + 1].class) {
            // Конец отрезка устанавливаем как середину между текущим и следующим элементами.
            tmp_segment.end = (feature_values[row_index].feature_value + feature_values[row_index + 1].feature_value)/2;
            // Сохраняем количество элементов отрезка.
            tmp_segment.values_count = segment_values_count;
            // При базовом разбиении отрезок содержит только элементы, принадлежащие его классу.
            tmp_segment.class_elements_count = segment_values_count;
            // Записываем все значения признака на отрезке в соответствующий массив.
            tmp_segment.values = (float*)malloc(segment_values_count * sizeof(float));
            for (segment_value_index = 0; segment_value_index < segment_values_count; segment_value_index++) {
                tmp_segment.values[segment_value_index] = tmp_segment_values[segment_value_index];
            }
            // Записываем класс элементов отрезка.
            tmp_segment.class = feature_values[row_index].class;
            // Записываем получившуюся структуру, описывающую отрезок, в массив.
            (*result)[segment_index] = tmp_segment;
            segment_index++;
            
            // Начинаем следующий отрезок.
            tmp_segment.start = tmp_segment.end;
            segment_values_count = 0;
        }
        tmp_segment_values[segment_values_count] = feature_values[row_index].feature_value;
        segment_values_count++;
    }
    // Не забываем сохранить последний отрезок.
    tmp_segment.end = INFINITY;
    tmp_segment.values_count = segment_values_count;
    tmp_segment.class_elements_count = segment_values_count;
    tmp_segment.values = (float*)malloc(segment_values_count * sizeof(float));
    for (segment_value_index = 0; segment_value_index < segment_values_count; segment_value_index++) {
        tmp_segment.values[segment_value_index] = tmp_segment_values[segment_value_index];
    }
    tmp_segment.class = feature_values[row_index].class;
    (*result)[segment_index] = tmp_segment;
    segment_index++;
    
    /* Проверка результатов разбиения */
    for (index = 0; index < segment_index; index++) {
        printf("Segment: [%f, %f], class = %d\n", (*result)[index].start, 
                                                  (*result)[index].end,
                                                  (*result)[index].class);
    }
    
    free(tmp_segment_values);
    
    // Далее выполняем слияние троек сегментов пока информативность 
    // их объединений больше, чем информативность их по отдельности.
    // Возможен ввод ограничения на количество зон разбиения.
    
    
}

// А вот такая структура для описания обучающих данных для классификации:
/*
struct train_data {
    int rows_count;
    int features_count;
    
    char** features_names;
    float** features_data;
    int* classes;
};
 * */