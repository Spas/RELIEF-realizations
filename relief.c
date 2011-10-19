#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "relief.h"

void use_relief(float** features_values, 
                int* classes, 
                int features_count,
                int rows_count,
                int steps_count,
                float* weights) {
    int    string_index = 0, feature_index, step_index = 0;
    float  *nearest_hit_row, *nearest_miss_row, *base_row;
    
    nearest_hit_row = (float*) calloc(features_count, sizeof(float));
    nearest_miss_row = (float*) calloc(features_count, sizeof(float));
    
    if ((steps_count == 0) || (steps_count >= rows_count)) {
        while (string_index < rows_count) {
            nearest_hit_row[0] = 1.123123;
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
        srand ( time(NULL) );
        while (step_index < steps_count) {
            string_index = rand() % rows_count;
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
            step_index++;
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
    int     feature_index;
    
    //nearest_hit_row = NULL;
    //nearest_miss_row = NULL;
    
    while (string_index < rows_count) {
        if (base_row_index == string_index) {
            string_index++;
            continue;
        }
        distance = get_rows_distance(features_values[base_row_index], features_values[string_index], features_count);
        
        if (base_row_class == classes[string_index]) {
            if ((hit_row_distance == -1) || (distance < hit_row_distance)) {
                hit_row_distance = distance;
                feature_index = 0;
                while (feature_index < features_count) {
                    nearest_hit_row[feature_index] = features_values[string_index][feature_index];
                    feature_index++;
                }
                
            }
        }
        else {
            if ((miss_row_distance == -1) || (distance < miss_row_distance)) {
                miss_row_distance = distance;
                feature_index = 0;
                while (feature_index < features_count) {
                    nearest_miss_row[feature_index] = features_values[string_index][feature_index];
                    feature_index++;
                }
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