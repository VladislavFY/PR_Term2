#include "prototypes8.h"

void free_students(Student *students) {
    if (students == NULL) {
        return;
    }
    free(students);
}

int read_students(FILE *f, Student **students, int *count) {
    Student temp;
    Student *new_students;
    int capacity = 10;

    if (f == NULL || students == NULL || count == NULL) {
        return -1;
    }

    *count = 0;
    *students = (Student *)malloc(capacity * sizeof(Student));
    if (*students == NULL) {
        return -2;
    }

    while (fscanf(f, "%127s %d %d", temp.Name, &temp.Group, &temp.School) == 3) {
        if (*count >= capacity) {
            capacity *= 2;
            new_students = (Student *)realloc(*students, capacity * sizeof(Student));
            if (new_students == NULL) {
                free(*students);
                *students = NULL;
                *count = 0;
                return -2;
            }
            *students = new_students;
        }

        (*students)[*count] = temp;
        (*count)++;
    }

    return 0;
}

int write_students(FILE *f, Student *students, int count) {
    int i;

    if (f == NULL || students == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        fprintf(f, "%s %d %d", students[i].Name, students[i].Group, students[i].School);
        if (i + 1 < count) {
            fprintf(f, "\n");
        }
    }

    return 0;
}

int has_schoolmate_in_group(Student *students, int count, int index, int group) {
    int i;

    if (students == NULL || index < 0 || index >= count) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (i == index) {
            continue;
        }

        if (students[i].Group == group && students[i].School == students[index].School) {
            return 1;
        }
    }

    return 0;
}

int group_has_school(Student *students, int count, int group, int school, int skip_index) {
    int i;

    if (students == NULL) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (i == skip_index) {
            continue;
        }

        if (students[i].Group == group && students[i].School == school) {
            return 1;
        }
    }

    return 0;
}

void redistribute_students(Student *students, int count) {
    int i;
    int j;

    if (students == NULL || count <= 1) {
        return;
    }

    /*
      Один проход по студентам A:
      если у A есть одношкольник в его группе,
      ищем студента B из другой группы, удовлетворяющего условиям,
      и меняем местами номера групп.
    */
    for (i = 0; i < count; ++i) {
        int group_a = students[i].Group;
        int school_a = students[i].School;

        if (!has_schoolmate_in_group(students, count, i, group_a)) {
            continue;
        }

        for (j = 0; j < count; ++j) {
            int group_b;
            int school_b;
            int temp_group;

            if (i == j) {
                continue;
            }

            group_b = students[j].Group;
            school_b = students[j].School;

            if (group_b == group_a) {
                continue;
            }

            /*
              Условие 1:
              у студента B в группе A не должно быть одношкольников.
              То есть в группе A, кроме самого A, не должно быть студентов
              из школы school_b.
            */
            if (group_has_school(students, count, group_a, school_b, i)) {
                continue;
            }

            /*
              Условие 2:
              в группе B не должно быть одношкольников студента A.
              То есть в группе B, кроме самого B, не должно быть студентов
              из школы school_a.
            */
            if (group_has_school(students, count, group_b, school_a, j)) {
                continue;
            }

            /*
              Нашли подходящего B.
              Меняем местами номера групп студентов A и B.
            */
            temp_group = students[i].Group;
            students[i].Group = students[j].Group;
            students[j].Group = temp_group;

            break;
        }
    }
}

int main_function(void) {
    FILE *f_in;
    FILE *f_out;
    Student *students = NULL;
    int count = 0;
    int status;

    f_in = fopen("data.dat", "r");
    if (f_in == NULL) {
        return 1;
    }

    status = read_students(f_in, &students, &count);
    fclose(f_in);

    if (status != 0) {
        free_students(students);
        return 2;
    }

    redistribute_students(students, count);

    f_out = fopen("data.res", "w");
    if (f_out == NULL) {
        free_students(students);
        return 3;
    }

    write_students(f_out, students, count);
    fclose(f_out);

    free_students(students);
    return 0;
}
