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
 * Функция считывания данных из CSV файла в память. Базово применяется для считывания обучающего набора из 'data1_train.csv'
 * @param file_name адрес файла
 * @param features_names указатель на массив с именами колонок
 * @param features_values указатель на двумерный массив со значениями параметров
 * @param classes указатель на массив со значениями класса
 * @param features_count указатель на переменную с количеством параметров
 * @param rows_count указатель на количество строк со значениями параметров
 */
void read_data_from_csv(char* file_name,
                        char*** features_names,   // reference to 1d strings array
                        float*** features_values, // reference to 2d float array
                        int** classes,            // reference to 1d int array
                        int* features_count,      // reference to features count
                        int* rows_count);         // reference to rows count

/**
 * Функция записи данных в CSV файл из памяти (аналогично read_data_from_csv, но в обратную сторону).
 * @param file_name адрес файла
 * @param features_names массив с именами колонок
 * @param features_values двумерный массив со значениями параметров
 * @param classes указатель массив со значениями класса
 * @param features_count количество параметров
 * @param rows_count количество строк
 */
void write_data_to_csv(char* file_name,
                       char** features_names,
                       float** features_values,
                       int* classes,
                       int features_count,
                       int rows_count);

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

#ifdef	__cplusplus
}
#endif

#endif	/* CSV_H */

