/* 
 * File:   main.c
 * Author: spas
 *
 * Created on 15 Октябрь 2011 г., 8:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "csv.h"
#include "relief.h"
#include "decision_tree.h"


#define TRAIN_FILE_NAME "/home/spas/IDSTU/RELIEF-C/data1_train.csv"
#define WEIGHTS_FILE_NAME "/home/spas/IDSTU/RELIEF-C/data1_train_weights.csv"


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
        
        time_t start_time, end_time;
        struct tm * timeinfo;
        
        time(&start_time);
        use_relief(features_values, classes, features_count, rows_count, 200, features_weights);
        time(&end_time);
        printf("Relief processing time with %i steps: %.0lf seconds\n", 200, difftime(end_time, start_time));
        
        // Запись весов колонок в файл
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