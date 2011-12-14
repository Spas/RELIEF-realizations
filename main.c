/* 
 * File:   main.c
 * Author: spas
 *
 * Created on 15 Октябрь 2011 г., 8:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "binarization.h"
#include "selfdescriptiveness.h"
#include "selfdescriptiveness_test.h"


#define TRAIN_FILE_NAME "/home/spas/IDSTU/RELIEF-C/data1_train.csv"
#define WEIGHTS_FILE_NAME "/home/spas/IDSTU/RELIEF-C/data1_train_weights.csv"

struct test_struct {
    int x;
    int* arr;
};

int main(int argc, char** argv) {
    struct feature_value_class data[10];
    data[0].feature_value = 1.0;
    data[0].class = 1;
    data[1].feature_value = 2.0;
    data[1].class = 1;
    data[2].feature_value = 3.0;
    data[2].class = 2;
    data[3].feature_value = 4.0;
    data[3].class = 1;
    data[4].feature_value = 3.5;
    data[4].class = 2;
    data[5].feature_value = 1.5;
    data[5].class = 1;
    data[6].feature_value = 6.0;
    data[6].class = 2;
    data[7].feature_value = 7.0;
    data[7].class = 2;
    data[8].feature_value = 8.0;
    data[8].class = 2;
    data[9].feature_value = 5.5;
    data[9].class = 2;
    
    struct segment_full_f* result;
    binarize_by_stat((struct feature_value_class*)data, 10, &result);
    
    
    return (EXIT_SUCCESS);
}