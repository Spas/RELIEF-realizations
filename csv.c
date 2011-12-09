/* 
 * File:   csv.c
 * Author: spas
 *
 * Created on 17 Октябрь 2011 г., 9:29
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

#define MIN_STRING_LENGTH 20

// public
void read_data_from_csv(char* file_name,
                        char*** features_names,   // reference to 1d strings array
                        float*** features_values, // reference to 2d float array
                        int** classes,            // reference to 1d int array
                        int* features_count,      // reference to features count
                        int* rows_count)          // reference to rows count
{        
    int head_string_length, head_string_element_length;
    int values_string_length, values_string_element_length;
    
    char *head_string, *feature_name;
    char *values_string, *feature_value;
    int row_index, feature_index;
    
    FILE* file = fopen(file_name , "r");
    if (file == NULL) {
        perror ("Error opening data file");
    }
    else {
        get_csv_matrix_size(file, 
                            &head_string_length,
                            &head_string_element_length,
                            &values_string_length,
                            &values_string_element_length,
                            features_count, rows_count);
        // Выделяем память под массивы features_names, classes и features_values
        (*features_names) = (char**) calloc(head_string_length * head_string_element_length, sizeof(char*));
        (*classes) = (int*) calloc((*rows_count), sizeof(int));
        (*features_values) = (float**) calloc((*rows_count) * (*features_count), sizeof(float*));

        // Также надо каждую строку надо определить как массив float-ов (или segmentation fault будет)
        while (row_index < *rows_count) {
            (*features_values)[row_index] = (float*) calloc((*features_count), sizeof(float));
            row_index++;
        }
        
        // Также необходимо выделить память под каждый элемент заголовка
        while (feature_index < *features_count) {
            (*features_names)[feature_index] = (char*) calloc(head_string_element_length, sizeof(char));
            feature_index++;
        }
        
        // Считываем и парсим строку заголовка
        head_string = (char*) calloc(head_string_length, sizeof(char));
        feature_name = (char*) calloc(head_string_element_length, sizeof(char));
        fgets(head_string, head_string_length, file);

        feature_index = 0;
        feature_name = strtok(head_string," ,");
        while (feature_name != NULL) {
            (*features_names)[feature_index] = feature_name;
            feature_name = strtok(NULL, " ,");
            feature_index++;
        }
        free(head_string);
        free(feature_name);

        // Считываем и парсим значения параметров и классов
        values_string = (char*) calloc(values_string_length, sizeof(char));
        feature_value = (char*) calloc(values_string_element_length, sizeof(char));

        row_index = 0;
        while (!feof(file)) {
            fgets(values_string, values_string_length, file);

            // Отсеивание пустых строк
            if (strlen(values_string) < MIN_STRING_LENGTH) {
                continue;
            }

            feature_index = 0;
            feature_value = strtok(values_string," ,");

            while ((feature_index <= *features_count) && (feature_value != NULL)) {
                if (feature_index == *features_count) {
                    (*classes)[row_index] = atoi(feature_value);
                } else {
                    (*features_values)[row_index][feature_index] = atof(feature_value);
                }
                feature_index++;
                feature_value = strtok(NULL," ,");
            }

            row_index++;
        }
        free(values_string);
        free(feature_value);
        fclose(file);
    }
}

void write_data_to_csv(char* file_name,
                       char** features_names,
                       float** features_values,
                       int* classes,
                       int features_count,
                       int rows_count) 
{
    int feature_index, row_index;
    
    FILE  *file = fopen(file_name , "w");
    if (file == NULL) {
        perror ("Error opening file");
    }
    else {
        feature_index = 0;
        while (feature_index < features_count) {
            if (feature_index == 0) {
                fprintf(file, "%s", features_names[feature_index]);
            } else {
                fprintf(file, ",%s", features_names[feature_index]);
            }
            feature_index++;
        }
        fprintf(file, ",class\n");

        row_index = 0;
        while (row_index < rows_count) {
            feature_index = 0;
            while (feature_index < features_count) {
                if (feature_index == 0) {
                    fprintf(file, "%f", features_values[row_index][feature_index]);
                } else {
                    fprintf(file, ",%f", features_values[row_index][feature_index]);
                }
                feature_index++;
            }
            fprintf(file, ",%d\n", classes[row_index]);
            row_index++;
        }
        fclose(file);
    }
}

// private
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

void write_weights_to_csv(FILE *file, 
                          char** features_names,
                          float* features_weights,
                          int features_count) {
    
    int feature_index = 0;
    while (feature_index < features_count) {
        if (feature_index == 0) {
            fprintf(file, "%s", features_names[feature_index]);
        } else {
            fprintf(file, ",%s", features_names[feature_index]);
        }
        feature_index++;
    }
    fputc('\n', file);
    
    feature_index = 0;
    while (feature_index < features_count) {
        if (feature_index == 0) {
            fprintf(file, "%f", features_weights[feature_index]);
        } else {
            fprintf(file, ",%f", features_weights[feature_index]);
        }
        feature_index++;
    }
    fputc('\n', file);
}