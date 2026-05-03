
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_LEN 128
#define GROUP_COUNT 10

typedef struct {
    char Name[NAME_LEN];
    int Group;
    double Rating;
} Student;

typedef struct {
    Student **students;
    int n;
    int group_number;
    double average_rating;
} Group;

void free_students(Student **students, int count);
void free_groups(Group groups[], int size);
int generate_data_file(const char *surname_file_name, const char *data_file_name, int count, int min_group, int max_group, double min_rating, double max_rating);
int read_students(FILE *f, Student ***students, int *count);
int write_group_file(const char *filename, Group *group);
void sort_students_by_group(Student **students, int count);
void build_new_groups(Student **students, int count, Group groups[], int size);
void sort_groups_by_rating(Group groups[], int size);
double calc_group_average(Group *group);
int main_function(void);
