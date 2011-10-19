/* 
 * File:   relief.h
 * Author: spas
 *
 * Created on 17 Октябрь 2011 г., 11:29
 */

#ifndef RELIEF_H
#define	RELIEF_H

#ifdef	__cplusplus
extern "C" {
#endif

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


#ifdef	__cplusplus
}
#endif

#endif	/* RELIEF_H */

