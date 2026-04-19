#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 128

typedef struct {
    char Name[NAME_LEN];
    int Group;
    int School;
} Student;

void free_students(Student *students);
int read_students(FILE *f, Student **students, int *count);
int write_students(FILE *f, Student *students, int count);
int has_schoolmate_in_group(Student *students, int count, int index, int group);
int group_has_school(Student *students, int count, int group, int school, int skip_index);
void redistribute_students(Student *students, int count);
int main_function(void);