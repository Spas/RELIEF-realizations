#! /usr/bin/python
# -*- coding: utf-8 -*-

#
# RELIEF features extraction algoritm realization.
#
__author__="spas"
__date__ ="$11.10.2011 17:41:23$"

import csv
import math

#
# Функция для выполнения загрузки данных из CSV файла.
#
def read_from_csv(file_url):
    csv_data = list(csv.reader(open(file_url, 'rb'), delimiter=',', quotechar='|'))
    return csv_data

#
# Функция для записи результатов в CSV файлы.
#
def write_to_csv(file_url, data):
    csv_writer = csv.writer(open(file_url, 'wb'), delimiter=',', quotechar='|',
                            quoting=csv.QUOTE_MINIMAL)
    csv_writer.writerows(data)

### Значения в последней колонке будем считать классом, к которому относится
### соответствующая последовательность. Потом сделаю более общий случай.
# result['hit'] - ближайший элемент того же класса, что и base_row
# result['miss'] - ближайший элемент из другого класса
# Поиск объединил в один метод чтобы не запускать 2 раза одинаковый цикл.
def get_nearest_mh(base_row, data):
    min_distance_hit = 0
    min_distance_miss = 0
    min_distance_hit_row = None
    min_distance_miss_row = None
    for row in data:
        if row == base_row:
            continue
        distance = get_rows_distance(base_row, row)
        if row[-1] == base_row[-1]:
            if (distance < min_distance_hit) or (min_distance_hit_row is None):
                min_distance_hit = distance
                min_distance_hit_row = row
        else:
            if (distance < min_distance_miss) or (min_distance_miss_row is None):
                min_distance_miss = distance
                min_distance_miss_row = row
    return {'hit': min_distance_hit_row, 'miss': min_distance_miss_row}

# Работает с отрезанием последней колонки - считается, что там записан класс,
# к которому относится запись
def get_rows_distance(row1, row2):
    distance = 0
    last_column_index = len(row1) - 1
    for column_index in xrange(last_column_index - 1):
        distance += math.fabs(float(row2[column_index]) - float(row1[column_index]))
    return distance

# Вычисление расстояния между колонками
def get_columns_distance(row1, row2, column_number):
    return math.fabs(float(row2[column_number]) - float(row1[column_number]))

# Нахождение с помощью алгоритма RLEIEF весов всех колонок и обрезание колонок с отрицательным весом
def use_relief(data, steps_count=0):
    weights = []
    
    last_column_index = len(data[0]) - 1
    for i in xrange(last_column_index):
        weights.append(0)

    data_without_columns_names = data[1:]
    if steps_count == 0:
        for row in data_without_columns_names:
            nearest_mh = get_nearest_mh(row, data_without_columns_names)
            for column_index in xrange(last_column_index):
                weights[column_index] += get_columns_distance(row, nearest_mh['miss'], column_index) - \
                                         get_columns_distance(row, nearest_mh['hit'], column_index)
    else:
        for i in xrange(steps_count):
            pass

    weights = [data[0], weights]
    return weights

if __name__ == "__main__":
    # Загрузка обучающего набора
    data = read_from_csv('/home/spas/IDSTU/RELIEF/data1_train.csv')

    # Вызов алгоритма relief
    weights = use_relief(data)
    
    # Запись данных в CSV файл
    write_to_csv('/home/spas/IDSTU/RELIEF/data1_train_weights.csv', weights)