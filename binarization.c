#include <math.h>
#include "selfdescriptiveness.h"

//
// Бинаризация числовых данных представляет собой запись их в виде отрезков исходя из оценок информативности.
// То есть сначала необходимо определить структуру данных, описывающую отрезки и лучи.
// Возможно использование INFINITY и -INFINITY для описания лучей.
//
// Структура для хранения полных данных об отрезках (дял бинаризации)
// 
struct segment_full_f {
    float start;
    float end;
    float* values;
    // Общее количество значений, которое покрывает отрезок.
    int values_count;
    // Количество элементов отрезка, соответствующих классу class.
    int class_elements_count;
    int class;
	float selfdescriptiveness;
};

struct class_data {
    int class;
    int elements_count;
};

// Функция для слияния тройки сегментов. 
// ОБЪЕДИНЯТЬ МОЖНО ТОЛЬКО 3 СОСЕДНИХ ЭЛЕМЕНТА С ЧЕРЕДОВАНИЕМ КЛАССОВ.
void concat_full_segments(struct segment_full_f* segment1,
                          struct segment_full_f* segment2,
                          struct segment_full_f* segment3,
                          struct segment_full_f* result) {
    //printf("sizeof(segment_full_f) = %d", sizeof(struct segment_full_f));
    
    if ((segment1->class != segment3->class) 
        || (segment1->class == segment2->class)
        || (segment3->class == segment2->class)) 
    {
        // Shit happened!
    }
    
    result->start = segment1->start;
    result->end = segment3->end;
    
    result->class = segment1->class;
    result->values_count = segment1->values_count + segment2->values_count + segment3->values_count;
    result->class_elements_count = segment1->class_elements_count + segment3->class_elements_count;
    result->values = (float*) malloc(result->values_count * sizeof(float));
    
    int value_index;
    int result_value_index = 0;
    for (value_index = 0; value_index < segment1->values_count; value_index++) {
        result->values[result_value_index] = segment1->values[value_index];
        result_value_index++;
    }
    for (value_index = 0; value_index < segment2->values_count; value_index++) {
        result->values[result_value_index] = segment2->values[value_index];
        result_value_index++;
    }
    for (value_index = 0; value_index < segment3->values_count; value_index++) {
        result->values[result_value_index] = segment3->values[value_index];
        result_value_index++;
    }
}

//!!! Надо ещё как-то хранить карту связей всех отрезков.

// Структура для хранения данных о параметре, значения которого разбиваются на отрезки.
struct feature_value_class {
    float feature_value;
    int class;
};

// Функция сравнения двух элементов типа struct feature_value_class. 
int compare_fvc(const void * a, const void * b) {
    if ((*(struct feature_value_class*)a).feature_value > (*(struct feature_value_class*)b).feature_value) {
        return 1;
    } else if ((*(struct feature_value_class*)a).feature_value == (*(struct feature_value_class*)b).feature_value) {
        return 0;
    } else {
        return -1;
    }
}

// Стоит строить карту разбиения на отрезки. Вопрос - в каком виде карта
// удобнее всего хранится и как можно на основе неё переформулировать задачу поиска предикатов.
// Возможно рассмотрения разных способов представления карты разбиения для разных видов целевых функций.

// Для статьи - стоит рассмотреть применение бустинга, бэггинга и МСП к задачам, которые рассмотривал Дюк в своей статье.
// Написать выкладки по тому, как улучшатся результаты.

// Функция дискретизации массива значений по соответствию ему класса и функции оценки статистической информативности.
void binarize_by_stat(struct feature_value_class* feature_values, // Массив с данными по столбцу
					  int rows_count, // Общее количество строк
					  int classes_count, // Общее количество классов
					  struct segment_full_f** result) // Указатель на массив, в который будут записаны получившиеся отрезки
{
    // Для разбиения на отрезки надо, по идее, отсортировать все значения. 
    // Попытка интеграции алгоритмов бинаризации и сортировки НЕ ДАСТ прироста в производительности.
    // То есть производительность результирующего алгоритма, вообще говоря, будет НЕ ЛУЧШЕ, чем
    // сортировки и последующей бинаризации по отдельности.

    qsort(feature_values, rows_count, sizeof(struct feature_value_class), compare_fvc);
    
    /* Проверка предварительной сортировки */
    int index;
    for (index = 0; index < rows_count; index++) {
        printf("(%f,%d), ", feature_values[index].feature_value, feature_values[index].class);
    }
    printf("\n");
    
    // Выделяем память под информацию разбиении значений на отрезки.
    (*result) = (struct segment_full_f*) malloc(rows_count * sizeof(struct segment_full_f));
    
    struct segment_full_f tmp_segment;
    int row_index, segment_index, segment_value_index, tmp_segment_value_index, class_index;
    int segments_count;
    
    tmp_segment.start = -INFINITY;
    
    segment_index = 0;
    segment_value_index = 0;
    tmp_segment_value_index = 0;
    
    // tmp_segment_values введен для экономии памяти - не надо для каждого отрезка 
    // под массив значений выделять память, как будто каждый отрезок содержит все элементы.
    // Сократим затраты памяти на хранение таких массивов с rows_count^2 до rows_count. 
    // 1 дополнительный проход по массиву (суммарная длинна прохода получится rows_count)
    // погоды не сделает.
    float* tmp_segment_values = (float*) malloc(rows_count * sizeof(float));
    struct class_data* classes_elements_data = malloc(classes_count * sizeof(struct class_data));
    
    for (row_index = 0; row_index < rows_count - 1; row_index++) {
		tmp_segment_values[tmp_segment_value_index] = feature_values[row_index].feature_value;
		tmp_segment_value_index++;
		
        if (feature_values[row_index].class != feature_values[row_index + 1].class) {
            // Конец отрезка устанавливаем как середину между текущим и следующим элементами.
            tmp_segment.end = (feature_values[row_index].feature_value + feature_values[row_index + 1].feature_value)/2;
            // Сохраняем количество элементов отрезка.
            tmp_segment.values_count = tmp_segment_value_index;
            // При базовом разбиении отрезок содержит только элементы, принадлежащие его классу.
            tmp_segment.class_elements_count = tmp_segment_value_index;
            
            // Записываем все значения признака на отрезке в соответствующий массив.
            tmp_segment.values = (float*) malloc(tmp_segment_value_index * sizeof(float));
			for (segment_value_index = 0; segment_value_index < tmp_segment_value_index; segment_value_index++) {
				tmp_segment.values[segment_value_index] = tmp_segment_values[segment_value_index];
			}
			
            // Записываем класс элементов отрезка.
            tmp_segment.class = feature_values[row_index].class;
            // Записываем получившуюся структуру, описывающую отрезок, в массив.
            (*result)[segment_index] = tmp_segment;
            segment_index++;
            
            // Начинаем следующий отрезок.
            tmp_segment.start = tmp_segment.end;
            tmp_segment_value_index = 0;
        }
        
        for (class_index = 0; class_index < classes_count; class_index++) {
            if (classes_elements_data[class_index].class == 0) {
                classes_elements_data[class_index].class = feature_values[row_index].class;
                classes_elements_data[class_index].elements_count++;
                break;
            } else if (classes_elements_data[class_index].class == feature_values[row_index].class) {
                classes_elements_data[class_index].elements_count++;
                break;
            }
        }
    }
    tmp_segment_values[tmp_segment_value_index] = feature_values[row_index].feature_value;
	tmp_segment_value_index++;
    // Не забываем сохранить последний отрезок.
    tmp_segment.end = INFINITY;
    tmp_segment.values_count = tmp_segment_value_index;
    tmp_segment.class_elements_count = tmp_segment_value_index;
    tmp_segment.values = (float*) malloc(tmp_segment_value_index * sizeof(float));
    for (segment_value_index = 0; segment_value_index < tmp_segment_value_index; segment_value_index++) {
		tmp_segment.values[segment_value_index] = tmp_segment_values[segment_value_index];
	}
    tmp_segment.class = feature_values[row_index].class;
    for (class_index = 0; class_index < classes_count; class_index++) {
        if (classes_elements_data[class_index].class == 0) {
            classes_elements_data[class_index].class = feature_values[row_index].class;
            classes_elements_data[class_index].elements_count++;
            break;
        } else if (classes_elements_data[class_index].class == feature_values[row_index].class) {
            classes_elements_data[class_index].elements_count++;
            break;
        }
    }
    (*result)[segment_index] = tmp_segment;
    segments_count = segment_index + 1;
    /* Проверка результатов разбиения */
    printf("\n\nТекущее разбиение:\n");
    for (index = 0; index < segments_count; index++) {
        printf("Segment: [%f, %f], class = %d, values_count = %d, class_values_count = %d,", (*result)[index].start, 
                                                  (*result)[index].end,
                                                  (*result)[index].class,
												  (*result)[index].values_count,
												  (*result)[index].class_elements_count
  			);
        printf("values = [");
        for (segment_value_index = 0; segment_value_index < (*result)[index].values_count; segment_value_index++) {
            if (segment_value_index != 0) {
               printf(","); 
            }
            printf("%f", (*result)[index].values[segment_value_index]);
        }
        printf("]");
        printf("\n");
    }
    /* Проверка подсчёта количества элементов в каждом классе */
    /*printf("Elements of %d class: %d\n", classes_elements_data[0].class, classes_elements_data[0].elements_count);
    printf("Elements of %d class: %d\n", classes_elements_data[1].class, classes_elements_data[1].elements_count);
    free(tmp_segment_values);
     */
    
    // Оценка информативности текущего разбиения.
    /*int* c_elements_count, int* c_f_elements_count,
   int classes_count,
   int c_total_elements, int c_f_total_elements*/
    int* c_elements_count = (int*) malloc(segments_count * sizeof(int));
    int* c_f_elements_count = (int*) malloc(segments_count * sizeof(int));
    
    for (segment_index = 0; segment_index < segments_count; segment_index++) {
        c_f_elements_count[segment_index] = (*result)[segment_index].class_elements_count;
    }
    for (class_index = 0; class_index < classes_count; class_index++) {
        c_elements_count[class_index] = classes_elements_data[class_index].elements_count;
    }
    
    float segments_selfdescriptiveness;
    
    // Пока для 2-х классов
    printf("\nОценка информативности отрезков:\n");
    for (segment_index = 0; segment_index < segments_count; segment_index++) {
        if (classes_elements_data[0].class == (*result)[segment_index].class) {
            (*result)[segment_index].selfdescriptiveness = statistical_descriptiveness(
                    classes_elements_data[0].elements_count, // Всего элементов того же класса, что и класс, соответствующий сегменту.
                    (*result)[segment_index].class_elements_count, // Количество элементов класса, выделяемых отрезком.
                    classes_elements_data[1].elements_count, // Всего элементов другого класса.
                    (*result)[segment_index].values_count - (*result)[segment_index].class_elements_count); //  Число ошибочно выделенных элементов
        } else {
            (*result)[segment_index].selfdescriptiveness = statistical_descriptiveness(classes_elements_data[1].elements_count, // Всего элементов того же класса, что и класс, соответствующий сегменту
                                                                       (*result)[segment_index].class_elements_count,
                                                                       classes_elements_data[0].elements_count,
                                                                       (*result)[segment_index].values_count - (*result)[segment_index].class_elements_count);

        }
        printf("Информативность отрезка [%f, %f] = %f\n", (*result)[segment_index].start, (*result)[segment_index].end, (*result)[segment_index].selfdescriptiveness);
    }
    
    
    // Далее выполняем слияние троек сегментов пока информативность 
    // их объединений больше, чем информативность их по отдельности.
    // Возможен ввод ограничения на количество зон разбиения. (точнее - необходим)
    
    struct segment_full_f new_segment;
    concat_full_segments(&((*result)[1]), &((*result)[2]), &((*result)[3]), &new_segment);
    
    new_segment.selfdescriptiveness = statistical_descriptiveness(
                    classes_elements_data[1].elements_count, // Всего элементов того же класса, что и класс, соответствующий сегменту.
                    new_segment.class_elements_count, // Количество элементов класса, выделяемых отрезком.
                    classes_elements_data[0].elements_count, // Всего элементов другого класса.
                    new_segment.values_count - new_segment.class_elements_count); //  Число ошибочно выделенных элементов
    
    
    printf("\nОтрезок [%f, %f], класс %d, %d значений, %d элементов класса, информативность %f\n", new_segment.start,
                                                                              new_segment.end,
                                                                              new_segment.class,
                                                                              new_segment.values_count,
                                                                              new_segment.class_elements_count,
																			  new_segment.selfdescriptiveness
	);
}

// А вот такая структура для описания обучающих данных для классификации:
/*
struct train_data {
    int rows_count;
    int features_count;
    
    char** features_names;
    float** features_data;
    int* classes;
};
 * */
