/* 
 * File:   selfdescriptiveness.h
 * Author: spas
 *
 * Created on 6 Декабрь 2011 г., 14:32
 */

#ifndef SELFDESCRIPTIVENESS_H
#define	SELFDESCRIPTIVENESS_H

#ifdef	__cplusplus
extern "C" {
#endif

// Pc - число объектов класса с в выборке (c_elements_count)
// pc(f) - из них число объектов, для которых выполняется условие f(x) = 1 (c_f_elements_count)
// Nc - число объектов всех остальных классов (nc_elements_count)
// nc(f) - из них число объектов, на которых выполняется условие f(x) = 1 (nc_f_elements_count)
    
float selection_entropy(int c_elements_count, int nc_elements_count);

float selection_entropy_opt(int c_elements_count, int nc_elements_count);
    
float heuristic_descriptiveness(int c_elements_count, 
                                int c_f_elements_count,
                                int nc_elements_count,
                                int nc_f_elements_count);

float statistical_descriptiveness_full(int* c_elements_count, int* c_f_elements_count,
                                       int classes_count,
                                       int c_total_elements, int c_f_total_elements);

float statistical_descriptiveness(int c_elements_count, 
                                  int c_f_elements_count,
                                  int nc_elements_count,
                                  int nc_f_elements_count);

float entropic_descriptiveness_full(int* c_elements_count, int* c_f_elements_count,
                                    int classes_count,
                                    int c_total_elements, int c_f_total_elements);


float entropic_descriptiveness(int c_elements_count, 
                               int c_f_elements_count,
                               int nc_elements_count,
                               int nc_f_elements_count);
float log_fact(int n);

float log2f(float n);

#ifdef	__cplusplus
}
#endif

#endif	/* SELFDESCRIPTIVENESS_H */