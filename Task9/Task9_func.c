#include "prototypes9.h"

void free_students(Student **students, int count) {
    int i;

    if (students == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free(students[i]);
    }

    free(students);
}

void free_groups(Group groups[], int size) {
    int i;

    for (i = 0; i < size; ++i) {
        free(groups[i].students);
        groups[i].students = NULL;
        groups[i].n = 0;
        groups[i].average_rating = 0.0;
    }
}

int read_students(FILE *f, Student ***students, int *count) {
    Student temp;
    Student **new_students;
    Student *new_student;
    int capacity = 10;

    if (f == NULL || students == NULL || count == NULL) {
        return -1;
    }

    *count = 0;
    *students = (Student **)malloc(capacity * sizeof(Student *));
    if (*students == NULL) {
        return -2;
    }

    while (fscanf(f, "%127s %d %lf", temp.Name, &temp.Group, &temp.Rating) == 3) {
        if (*count >= capacity) {
            capacity *= 2;
            new_students = (Student **)realloc(*students, capacity * sizeof(Student *));
            if (new_students == NULL) {
                free_students(*students, *count);
                *students = NULL;
                *count = 0;
                return -2;
            }
            *students = new_students;
        }

        new_student = (Student *)malloc(sizeof(Student));
        if (new_student == NULL) {
            free_students(*students, *count);
            *students = NULL;
            *count = 0;
            return -2;
        }

        *new_student = temp;
        (*students)[*count] = new_student;
        (*count)++;
    }

    return 0;
}

int write_group_file(const char *filename, Group *group) {
    FILE *f;
    int i;

    if (filename == NULL || group == NULL) {
        return -1;
    }

    f = fopen(filename, "w");
    if (f == NULL) {
        return -1;
    }

    for (i = 0; i < group->n; ++i) {
        fprintf(f, "%s %d %.2lf",
                group->students[i]->Name,
                group->students[i]->Group,
                group->students[i]->Rating);

        if (i + 1 < group->n) {
            fprintf(f, "\n");
        }
    }

    fclose(f);
    return 0;
}

void sort_students_by_group(Student **students, int count) {
    int i;
    int j;
    Student *temp;

    if (students == NULL || count <= 1) {
        return;
    }

    for (i = 0; i < count - 1; ++i) {
        for (j = 0; j < count - 1 - i; ++j) {
            if (students[j]->Group > students[j + 1]->Group) {
                temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

double calc_group_average(Group *group) {
    int i;
    double sum = 0.0;

    if (group == NULL || group->n == 0) {
        return 0.0;
    }

    for (i = 0; i < group->n; ++i) {
        sum += group->students[i]->Rating;
    }

    return sum / group->n;
}

void build_new_groups(Student **students, int count, Group groups[], int size) {
    int i;
    int base_size;
    int extra;
    int current = 0;
    int group_size;

    if (students == NULL || groups == NULL || size <= 0) {
        return;
    }

    /*
      Инициализация групп
    */
    for (i = 0; i < size; ++i) {
        groups[i].students = NULL;
        groups[i].n = 0;
        groups[i].group_number = 301 + i;
        groups[i].average_rating = 0.0;
    }

    /*
      Делим студентов на 10 групп примерно поровну:
      base_size = минимальный размер каждой группы,
      extra = сколько первых групп получат по одному лишнему студенту.
    */
    base_size = count / size;
    extra = count % size;

    for (i = 0; i < size; ++i) {
        if (i < extra) {
            group_size = base_size + 1;
        } else {
            group_size = base_size;
        }

        groups[i].n = group_size;

        if (group_size > 0) {
            groups[i].students = (Student **)malloc(group_size * sizeof(Student *));
            if (groups[i].students == NULL) {
                /*
                  В учебном стиле здесь просто выходим,
                  так как вызывающий код потом всё освободит.
                */
                groups[i].n = 0;
                continue;
            }

            for (int j = 0; j < group_size; ++j) {
                groups[i].students[j] = students[current];
                groups[i].students[j]->Group = groups[i].group_number;
                current++;
            }
        }

        groups[i].average_rating = calc_group_average(&groups[i]);
    }
}

void sort_groups_by_rating(Group groups[], int size) {
    int i;
    int j;
    Group temp;

    if (groups == NULL || size <= 1) {
        return;
    }

    for (i = 0; i < size - 1; ++i) {
        for (j = 0; j < size - 1 - i; ++j) {
            /*
              Пустые группы должны быть в конце.
            */
            if (groups[j].n == 0 && groups[j + 1].n > 0) {
                temp = groups[j];
                groups[j] = groups[j + 1];
                groups[j + 1] = temp;
            }
            else if (groups[j].n > 0 && groups[j + 1].n > 0 &&
                     groups[j].average_rating < groups[j + 1].average_rating) {
                temp = groups[j];
                groups[j] = groups[j + 1];
                groups[j + 1] = temp;
            }
        }
    }
}

int main_function(void) {
    FILE *f_in;
    Student **students = NULL;
    Group groups[GROUP_COUNT];
    int count = 0;
    int status;
    int i;
    char filename[32];

    f_in = fopen("data.dat", "r");
    if (f_in == NULL) {
        return 1;
    }

    status = read_students(f_in, &students, &count);
    fclose(f_in);

    if (status != 0) {
        free_students(students, count);
        return 2;
    }

    sort_students_by_group(students, count);

    build_new_groups(students, count, groups, GROUP_COUNT);

    sort_groups_by_rating(groups, GROUP_COUNT);

    for (i = 0; i < GROUP_COUNT; ++i) {
        if (groups[i].n == 0) {
            continue;
        }

        sprintf(filename, "%d.txt", groups[i].group_number);
        write_group_file(filename, &groups[i]);
    }

    free_groups(groups, GROUP_COUNT);
    free_students(students, count);

    return 0;
}