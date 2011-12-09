#include <math.h>

#include "selfdescriptiveness_test.h"
#include "selfdescriptiveness.h"

void full_descriptiveness_test() {
    int classes_count = 2;
    int* c_elements_count = malloc(classes_count * sizeof(int));
    c_elements_count[0] = 200;
    c_elements_count[1] = 100;
    
    int* c_f_elements_count = malloc(classes_count * sizeof(int));
    c_f_elements_count[0] = 100;
    c_f_elements_count[1] = 0;
    
    printf("IGain = %f\n", entropic_descriptiveness_full(c_elements_count, c_f_elements_count, classes_count, 300, 100));
}

void selfdescriptiveness_test() {
    int c_elements_count;
    int nc_elements_count;
    
    float statistical_selfdescriptiveness;
    float entropy_selfdescriptiveness;
    float additional_selfdescriptiveness;
    
    int c_f_data[6];
    int nc_f_data[6];
    int i;
    
    c_elements_count =  200;
    nc_elements_count = 100;
    
    c_f_data[0] = 50;
    nc_f_data[0] = 0;
    
    c_f_data[1] = 100;
    nc_f_data[1] = 50;
    
    c_f_data[2] = 50;
    nc_f_data[2] = 9;
    
    c_f_data[3] = 5;
    nc_f_data[3] = 0;
    
    c_f_data[4] = 100;
    nc_f_data[4] = 0;
    
    c_f_data[5] = 140;
    nc_f_data[5] = 20;
    
    i = 0;
    printf("p | n | stat | ICGain | sqrt\n");
    while (i < 6) {
        statistical_selfdescriptiveness = statistical_descriptiveness(c_elements_count, c_f_data[i], nc_elements_count, nc_f_data[i]);
        entropy_selfdescriptiveness = entropic_descriptiveness(c_elements_count, c_f_data[i], nc_elements_count, nc_f_data[i]);
        additional_selfdescriptiveness = sqrt(c_f_data[i]) - sqrt(nc_f_data[i]);
        
        printf("%d | %d | %f | %f | %f\n", c_f_data[i], nc_f_data[i], statistical_selfdescriptiveness, entropy_selfdescriptiveness, additional_selfdescriptiveness);
        i++;
    }
}