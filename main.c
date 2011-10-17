/* 
 * File:   main.c
 * Author: spas
 *
 * Created on 15 Октябрь 2011 г., 8:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define TRAIN_FILE_NAME "/home/spas/IDSTU/RELIEF-C/data1_train.csv"
#define WEIGHTS_FILE_NAME "/home/spas/IDSTU/RELIEF-C/data1_train_weights.csv"
#define MIX_STRING_LENGTH 20

/**
 * Функция считывания данных из CSV файла в память. Базово применяется для считывания обучающего набора из 'data1_train.csv'
 * @param file_name адрес файла
 * @param features_names массив с именами колонок
 * @param features_values массив со значениями колонок (двумерный)
 * @param classes массив со значениями классов
 */
void read_from_csv(FILE* file, 
                   char** features_names, 
                   float** features_values,
                   int* classes,
                   int head_string_length, int head_element_length,
                   int values_string_length, int value_element_length,
                   int features_count, int rows_count);

/**
 * Запись данных о весах колонок в csv файл
 * @param file
 * @param features_names
 * @param features_weights
 */
void write_weights_to_csv(FILE *file,
                          char** features_names,
                          float* features_weights,
                          int features_count);

/**
 * @desc Получение некоторых параметров для определении объёма памяти, которая требуется для хранения данных из файла. По умолчанию разделителем считаем символ ','
 * @param file
 * @param head_string_length длина первой (заголовочной) строки
 * @param head_string_element_length наибольшая из длин названий колонки
 * @param values_string_length наибольшая длина строки со значениями
 * @param values_string_element_length наибольшая из длин значений колонок
 * @param values_rows_count количество строк в файле (-1, заголовочную не считаем)
 */
void get_csv_matrix_size(FILE* file, 
                         int* head_string_length, 
                         int* head_string_element_length,
                         int* values_string_length,
                         int* values_string_element_length,
                         int* features_count,
                         int* rows_count);

// Функции обработки данных с помощью алгоритма RELIEF
/**
 * Функция для поиска ближайшего элемента того же класса что и row (hit_row) и ближайшего элемента из другого класса (miss_row)
 * @param row базовай строка
 * @param class класс базовой строки
 * @param data массив с данными по всем строкам
 * @param classes массив с данными по классам всех строк
 * @param hit_row найденная ближайшая запись того же класса что и class
 * @param miss_row найденная ближайшая запись из класса, не совпадающего с class
 */
void get_nearest_mh(int base_row_index, float** features_values, int* classes, int rows_count, int features_count, float* nearest_hit_row, float* nearest_miss_row);

float get_rows_distance(float* row1, float* row2, int features_count);
float get_features_distance(float* row1, float* row2, int feature_number);
/**
 * Вычисление весов всех колонок с помощью алгоритма RELIEF
 * @param data - массив с данными по всем строкам
 * @param classes - массив с данными по классам всех строк
 * @param weights - массив, в который записываются значения весов колонок
 * @param steps_count
 */
void use_relief(float** features_values, int* classes, int features_count, int rows_count, int steps_count, float* weights);

/*
 * 
 */
int main(int argc, char** argv) {
    char  **features_names;
    float **features_values;
    float *features_weights;
    int   *classes;
    int   row_index = 0, feature_index = 0;
    int   head_string_length, values_string_length;
    int   head_element_length, value_element_length;
    int   features_count, rows_count;
    FILE  *file;
    
    file = fopen(TRAIN_FILE_NAME , "r");
    if (file == NULL) {
        perror ("Error opening train data file");
    }
    else {
        get_csv_matrix_size(file, 
                            &head_string_length, &head_element_length,
                            &values_string_length, &value_element_length,
                            &features_count, &rows_count);
        
        features_names = (char**) calloc(head_string_length * head_element_length, sizeof(char*));
        classes = (int*) calloc(rows_count, sizeof(int));
        features_values = (float**) calloc(rows_count * features_count, sizeof(float*));
        
        // Также надо каждую строку надо определить как массив float-ов (или segmentation fault будет)
        while (row_index < rows_count) {
            features_values[row_index] = (float*) calloc(features_count, sizeof(float));
            row_index++;
        }
        
        // Также необходимо выделить память под каждый элемент заголовка
        while (feature_index < features_count) {
            features_names[feature_index] = (char*) calloc(head_element_length, sizeof(char));
            feature_index++;
        }
        
        read_from_csv(file, features_names, features_values, classes, 
                      head_string_length, head_element_length, 
                      values_string_length, value_element_length, 
                      features_count, rows_count);
        fclose(file);
        
        // Расчёт весов всех колонок
        features_weights = (float*) calloc(features_count, sizeof(float));
        use_relief(features_values, classes, features_count, rows_count, 0, features_weights);
        
        // Щапись весов колонок в файл
        file = fopen(WEIGHTS_FILE_NAME , "w");
        if (file == NULL) {
            perror ("Error opening file");
        }
        write_weights_to_csv(file, features_names, features_weights, features_count);
        fclose(file);
        
        /*
        printf("Rows count: %d\n", rows_count);
        printf("Features count: %d\n", features_count);
        
        int row_index = 0;
        int feature_index = features_count - 1;
        printf("features_values[%d][%d] = %f\n", row_index, feature_index, features_values[row_index][feature_index]);
        printf("classes[%d] = %d\n", row_index, classes[row_index]);
         */
    }
    
    return (EXIT_SUCCESS);
}

void get_csv_matrix_size(FILE* file, 
                         int* head_string_length, 
                         int* head_string_element_length,
                         int* values_string_length,
                         int* values_string_element_length,
                         int* features_count,
                         int* rows_count) {
    char  symbol;
    int   symbols_count = 0;
    int   column_symbols_count = 0;
    
    *head_string_length = 0;
    *head_string_element_length = 0;
    *values_string_length = 0;
    *values_string_element_length = 0;
    *features_count = 0;
    *rows_count = 0;
    
    if (file == NULL) {
        perror ("Cannot process null file (get_csv_matrix_size)");
    }
    else {
        // Минус текущей реализации - если первая строка файла пустая - ничего работать не будет.
        while ((symbol = fgetc(file)) != EOF) {
            if (*head_string_length == 0) {
                // Для строки заголовков также надо найти наибольшую длинну названия колонки
                if (symbol == ',') {
                    if (column_symbols_count > *head_string_element_length) {
                        *head_string_element_length = column_symbols_count;
                    }
                    column_symbols_count = 0;
                    (*features_count)++; // Инкрементируем количество колонок при нахождении каждого разделителя (базово - ',')
                }
                column_symbols_count++;
            } 
            else {
                // Для строки значения также надо найти наибольшую длинну
                if (symbol == ',') {
                    if (column_symbols_count > *values_string_element_length) {
                        (*values_string_element_length) = column_symbols_count;
                    }
                    column_symbols_count = 0;
                }
                column_symbols_count++;
            }
            if (symbol == '\n' || symbol == '\0') {
                if (*head_string_length == 0) {
                    *rows_count = 0; // Заголовочную строку не считаем
                    *head_string_length = symbols_count + 1; // +1 - запас карман не тянет, мало ли
                } 
                else {
                    (*rows_count)++; // Инкрементируем количество строк
                    if (symbols_count > *values_string_length) {
                        *values_string_length = symbols_count + 1; // Если длина текущей строки больше максимальной найдённой - заменяем
                    }
                }
                symbols_count = 0; // Начало подсчёта количества симоволов в следующей строке
                column_symbols_count = 0; // Также обнуляем длину значения в текущей ячейке (до разделителя)
            }
            symbols_count++; // Инкрементация количества символо в строке
        }
        rewind(file);
    }
}

void read_from_csv(FILE* file, 
                   char** features_names, 
                   float** features_values,
                   int* classes,
                   int head_string_length, int head_element_length,
                   int values_string_length, int value_element_length,
                   int features_count, int rows_count) {
    
    char  *head_string;
    char  *values_string;
    int   feature_index, string_index;
    char* feature_name;
    char* feature_value;
    
    
    // Запись данных из заголовочной строки в массив features_names
    head_string = (char*) calloc(head_string_length, sizeof(char));
    feature_name = (char*) calloc(head_element_length, sizeof(char));

    fgets(head_string, head_string_length, file);

    feature_index = 0;
    feature_name = strtok(head_string," ,");
    while (feature_name != NULL) {
        features_names[feature_index] = feature_name;
        feature_name = strtok(NULL, " ,");
        feature_index++;
    }
    
    
    // feature_index - сюда было записано количество колонок со значениями
    values_string = (char*) calloc(values_string_length, sizeof(char));
    feature_value = (char*) calloc(value_element_length, sizeof(char));

    while (!feof(file)) {
        fgets(values_string, values_string_length, file);
        if (strlen(values_string) < MIX_STRING_LENGTH) {
            continue;
        }
        
        feature_index = 0;
        string_index = 0;
        feature_value = strtok(values_string," ,");
        
        while ((feature_index <= features_count) && (feature_value != NULL)) {
            if (feature_index == features_count) {
                classes[string_index] = atoi(feature_value);
            } else {
                features_values[string_index][feature_index] = atof(feature_value);
            }
            feature_index++;
            feature_value = strtok(NULL," ,");
        }
        string_index++;
    }
}

void write_weights_to_csv(FILE *file, 
                          char** features_names,
                          float* features_weights,
                          int features_count) {
    
    int feature_index = 0;
    while (feature_index < features_count) {
        if (feature_index == 0) {
            fputs(features_names[feature_index], file);
        } else {
            fprintf(file, ",%s", features_names[feature_index]);
        }
        fputs("\n", file);
        feature_index++;
    }
    
    feature_index = 0;
    while (feature_index < features_count) {
        if (feature_index == 0) {
            fprintf(file, "%f", features_weights[feature_index]);
        } else {
            fprintf(file, ",%f", features_weights[feature_index]);
        }
        feature_index++;
    }
    fputs("n", file);
}


void use_relief(float** features_values, 
                int* classes, 
                int features_count,
                int rows_count,
                int steps_count,
                float* weights) {
    int    string_index = 0, feature_index;
    float  *nearest_hit_row, *nearest_miss_row, *base_row;
    
    nearest_hit_row = (float*) calloc(features_count, sizeof(float));
    nearest_miss_row = (float*) calloc(features_count, sizeof(float));
    
    if ((steps_count == 0) || (steps_count >= rows_count)) {
        while (string_index < rows_count) {
            get_nearest_mh(string_index,
                           features_values,
                           classes,
                           rows_count,
                           features_count,
                           nearest_hit_row,
                           nearest_miss_row);
            feature_index = 0;
            base_row = features_values[string_index];
            while (feature_index < features_count) {
                weights[feature_index] += fabs(nearest_miss_row[feature_index] - base_row[feature_index]) -
                                          fabs(nearest_hit_row[feature_index] - base_row[feature_index]);
                feature_index++;
            }
            string_index++;
        }
    } else {
        string_index = 0;
        get_nearest_mh(string_index,
                       features_values,
                       classes,
                       rows_count,
                       features_count,
                       nearest_hit_row,
                       nearest_miss_row);
        feature_index = 0;
        base_row = features_values[string_index];
        while (feature_index < features_count) {
            weights[feature_index] += fabs(nearest_miss_row[feature_index] - base_row[feature_index]) -
                                      fabs(nearest_hit_row[feature_index] - base_row[feature_index]);
            feature_index++;
        }
    }
}

void get_nearest_mh(int base_row_index, 
                    float** features_values,
                    int* classes,
                    int rows_count,
                    int features_count,
                    float* nearest_hit_row,
                    float* nearest_miss_row) {
    float   hit_row_distance = -1; // Т.к. расстояние всегда положительно - то при условии равенства -1 можно установить первое значение
    float   miss_row_distance = -1;
    float   distance = 0;
    int     string_index = 0;
    int     base_row_class = classes[base_row_index];
    
    nearest_hit_row = NULL;
    nearest_miss_row = NULL;
    
    while (string_index < rows_count) {
        if (base_row_index == string_index) {
            string_index++;
            continue;
        }
        distance = get_rows_distance(features_values[base_row_index], features_values[string_index], features_count);
        
        if (base_row_class == classes[string_index]) {
            if ((hit_row_distance == -1) || (distance < hit_row_distance)) {
                hit_row_distance = distance;
                nearest_hit_row = features_values[string_index];
            }
        }
        else {
            if ((miss_row_distance == -1) || (distance < miss_row_distance)) {
                miss_row_distance = distance;
                nearest_miss_row = features_values[string_index];
            }
        }
        string_index++;
    }
}

float get_rows_distance(float* row1, float* row2, int features_count) {
    int feature_index = 0;
    float distance = 0;
    while (feature_index < features_count) {
        distance += fabs(row1[feature_index] - row2[feature_index]);
        feature_index++;
    }
    return distance;
}

float get_features_distance(float* row1, float* row2, int feature_number) {
    return fabs(row1[feature_number] - row2[feature_number]);
}