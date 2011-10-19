/* 
 * File:   csv.h
 * Author: spas
 *
 * Created on 17 Октябрь 2011 г., 11:28
 */

#ifndef CSV_H
#define	CSV_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @desc Получение некоторых параметров для определении объёма памяти, которая требуется для хранения данных из файла. По умолчанию разделителем считаем символ ','
 * @param file
 * @param head_string_length длина первой (заголовочной) строки
 * @param head_string_element_length наибольшая из длин названий колонки
 * @param values_string_length наибольшая длина строки со значениями
 * @param values_string_element_length наибольшая длина строки со значением
 * @param features_count наибольшая из длин значений колонок
 * @param rows_count количество строк со значениями параметров
 */
void get_csv_matrix_size(FILE* file, 
                         int* head_string_length, 
                         int* head_string_element_length,
                         int* values_string_length,
                         int* values_string_element_length,
                         int* features_count,
                         int* rows_count);
    
/**
 * Функция считывания данных из CSV файла в память. Базово применяется для считывания обучающего набора из 'data1_train.csv'
 * @param file_name адрес файла
 * @param features_names массив с именами колонок
 * @param features_values матрица со значениями параметров
 * @param classes массив со значениями классов
 * @param head_string_length длина первой (заголовочной) строки
 * @param head_string_element_length наибольшая из длин названий колонки
 * @param values_string_length наибольшая длина строки со значениями
 * @param values_string_element_length наибольшая длина строки со значением
 * @param features_count наибольшая из длин значений колонок
 * @param rows_count количество строк со значениями параметров
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
 * @param features_weights массив с весами колонок
 * @param features_count наибольшая из длин значений колонок
 */
void write_weights_to_csv(FILE *file,
                          char** features_names,
                          float* features_weights,
                          int features_count);


#ifdef	__cplusplus
}
#endif

#endif	/* CSV_H */

