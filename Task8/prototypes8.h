#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_LEN 128

typedef struct {
 char Name[NAME_LEN];
 int Group;
 int School;
} Student;

typedef struct {
 Student *students;
 int count;
} StudentData;

void init_database(StudentData *database);
void free_students(StudentData *database);
int copy_database(StudentData *from, StudentData *to);
int generate_students(const char *surname_file_name, StudentData *database, int count, int min_group, int max_group, int min_school, int max_school);
int read_students(FILE *f, StudentData *database);
int write_students(FILE *f, StudentData *database);
int compare_students(const void *a, const void *b);
void sort_students(StudentData *database);
int has_schoolmate_in_group(StudentData *database, int index, int group);
int group_has_school(StudentData *database, int group, int school, int skip_index);
int count_problem_students(StudentData *database);
void redistribute_students(StudentData *database);
int main_function(void);
