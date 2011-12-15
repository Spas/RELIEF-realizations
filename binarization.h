/* 
 * File:   binarization.h
 * Author: spas
 *
 * Created on 9 Декабрь 2011 г., 13:13
 */

#ifndef BINARIZATION_H
#define	BINARIZATION_H

#ifdef	__cplusplus
extern "C" {
#endif

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

struct feature_value_class {
    float feature_value;
    int class;
};

void binarize_by_stat(struct feature_value_class* feature_values, int rows_count, int classes_count, struct segment_full_f** result);


#ifdef	__cplusplus
}
#endif

#endif	/* BINARIZATION_H */

