#include "prototypes8.h"

#define MAX_SURNAMES 1000

void init_database(StudentData *database)
{
    if (database == NULL) {
        return;
    }

    database->students = NULL;
    database->count = 0;
}

void free_students(StudentData *database)
{
    if (database == NULL) {
        return;
    }

    free(database->students);
    database->students = NULL;
    database->count = 0;
}

int copy_database(StudentData *from, StudentData *to)
{
    int i;

    if (from == NULL || to == NULL) {
        return -1;
    }

    init_database(to);

    if (from->students == NULL || from->count <= 0) {
        return 0;
    }

    to->students = (Student *)malloc((size_t)from->count * sizeof(Student));

    if (to->students == NULL) {
        return -2;
    }

    to->count = from->count;

    for (i = 0; i < from->count; ++i) {
        to->students[i] = from->students[i];
    }

    return 0;
}

int generate_students(const char *surname_file_name, StudentData *database, int count,int min_group,int max_group, int min_school, int max_school){
    
    FILE *surname_file;
    char surnames[MAX_SURNAMES][NAME_LEN];
    int surname_count = 0;
    int i;

    if (surname_file_name == NULL || database == NULL) {
        return -1;
    }

    if (count <= 0) {
        return -2;
    }

    if (min_group > max_group || min_school > max_school) {
        return -3;
    }

    surname_file = fopen(surname_file_name, "r");

    if (surname_file == NULL) {
        return -4;
    }

    while (surname_count < MAX_SURNAMES &&
           fscanf(surname_file, "%127s", surnames[surname_count]) == 1) {
        surname_count++;
    }

    fclose(surname_file);

    if (surname_count == 0) {
        return -5;
    }

    free_students(database);

    database->students = (Student *)malloc((size_t)count * sizeof(Student));

    if (database->students == NULL) {
        database->count = 0;
        return -6;
    }

    database->count = count;
    srand((unsigned int)time(NULL));

    for (i = 0; i < database->count; ++i) {
        int surname_index;
        int group;
        int school;

        surname_index = rand() % surname_count;
        group = min_group + rand() % (max_group - min_group + 1);
        school = min_school + rand() % (max_school - min_school + 1);

        strcpy(database->students[i].Name, surnames[surname_index]);
        database->students[i].Group = group;
        database->students[i].School = school;
    }

    return 0;
}

int read_students(FILE *f, StudentData *database)
{
    Student temp;
    Student *new_students;
    int capacity = 10;

    if (f == NULL || database == NULL) {
        return -1;
    }

    free_students(database);

    database->students = (Student *)malloc((size_t)capacity * sizeof(Student));

    if (database->students == NULL) {
        database->count = 0;
        return -2;
    }

    database->count = 0;

    while (fscanf(f, "%127s %d %d", temp.Name, &temp.Group, &temp.School) == 3) {
        if (database->count >= capacity) {
            capacity *= 2;

            new_students = (Student *)realloc(database->students,
                                              (size_t)capacity * sizeof(Student));

            if (new_students == NULL) {
                free_students(database);
                return -2;
            }

            database->students = new_students;
        }

        database->students[database->count] = temp;
        database->count++;
    }

    if (database->count == 0) {
        free_students(database);
        return -3;
    }

    return 0;
}

int write_students(FILE *f, StudentData *database)
{
    int i;

    if (f == NULL || database == NULL || database->students == NULL) {
        return -1;
    }

    for (i = 0; i < database->count; ++i) {
        fprintf(f, "%s %d %d",
                database->students[i].Name,
                database->students[i].Group,
                database->students[i].School);

        if (i + 1 < database->count) {
            fprintf(f, "\n");
        }
    }

    return 0;
}

int compare_students(const void *a, const void *b)
{
    const Student *student_a;
    const Student *student_b;
    int name_compare;

    student_a = (const Student *)a;
    student_b = (const Student *)b;

    if (student_a->Group < student_b->Group) {
        return -1;
    }

    if (student_a->Group > student_b->Group) {
        return 1;
    }

    if (student_a->School < student_b->School) {
        return -1;
    }

    if (student_a->School > student_b->School) {
        return 1;
    }

    name_compare = strcmp(student_a->Name, student_b->Name);

    if (name_compare < 0) {
        return -1;
    }

    if (name_compare > 0) {
        return 1;
    }

    return 0;
}

void sort_students(StudentData *database)
{
    if (database == NULL || database->students == NULL || database->count <= 1) {
        return;
    }

    qsort(database->students,
          (size_t)database->count,
          sizeof(Student),
          compare_students);
}

int has_schoolmate_in_group(StudentData *database, int index, int group)
{
    int i;
    int school;

    if (database == NULL || database->students == NULL ||
        index < 0 || index >= database->count) {
        return 0;
    }

    school = database->students[index].School;

    for (i = 0; i < database->count; ++i) {
        if (i == index) {
            continue;
        }

        if (database->students[i].Group == group &&
            database->students[i].School == school) {
            return 1;
        }
    }

    return 0;
}

int group_has_school(StudentData *database, int group, int school, int skip_index)
{
    int i;

    if (database == NULL || database->students == NULL) {
        return 0;
    }

    for (i = 0; i < database->count; ++i) {
        if (i == skip_index) {
            continue;
        }

        if (database->students[i].Group == group &&
            database->students[i].School == school) {
            return 1;
        }
    }

    return 0;
}

int count_problem_students(StudentData *database)
{
    int i;
    int count = 0;

    if (database == NULL || database->students == NULL) {
        return 0;
    }

    for (i = 0; i < database->count; ++i) {
        if (has_schoolmate_in_group(database, i, database->students[i].Group)) {
            count++;
        }
    }

    return count;
}

void redistribute_students(StudentData *database)
{
    int i;
    int j;

    if (database == NULL || database->students == NULL || database->count <= 1) {
        return;
    }

    for (i = 0; i < database->count; ++i) {
        int group_a;
        int school_a;

        group_a = database->students[i].Group;
        school_a = database->students[i].School;

        if (!has_schoolmate_in_group(database, i, group_a)) {
            continue;
        }

        for (j = 0; j < database->count; ++j) {
            int group_b;
            int school_b;
            int temp_group;

            if (i == j) {
                continue;
            }

            group_b = database->students[j].Group;
            school_b = database->students[j].School;

            if (group_b == group_a) {
                continue;
            }

            if (group_has_school(database, group_a, school_b, i)) {
                continue;
            }

            if (group_has_school(database, group_b, school_a, j)) {
                continue;
            }

            temp_group = database->students[i].Group;
            database->students[i].Group = database->students[j].Group;
            database->students[j].Group = temp_group;

            break;
        }
    }

    sort_students(database);
}

int main_function(void)
{
    FILE *f_in = NULL;
    FILE *f_out = NULL;
    StudentData database;
    StudentData original_database;
    int status;
    int choice;
    int problems_before = 0;
    int problems_after = 0;

    init_database(&database);
    init_database(&original_database);

    printf("Choose input mode:\n");
    printf("1 - Read students from data.dat\n");
    printf("2 - Generate students automatically\n");
    printf("Your choice: ");

    if (scanf("%d", &choice) != 1) {
        return -1;
    }

    if (choice == 1) {
        f_in = fopen("data.dat", "r");

        if (f_in == NULL) {
            return -2;
        }

        status = read_students(f_in, &database);
        fclose(f_in);
        f_in = NULL;

        if (status != 0) {
            free_students(&database);
            return -3;
        }
    } else if (choice == 2) {
        int count;
        int min_group;
        int max_group;
        int min_school;
        int max_school;

        printf("Enter number of students: ");
        if (scanf("%d", &count) != 1) {
            return -4;
        }

        printf("Enter minimum group number: ");
        if (scanf("%d", &min_group) != 1) {
            return -5;
        }

        printf("Enter maximum group number: ");
        if (scanf("%d", &max_group) != 1) {
            return -6;
        }

        printf("Enter minimum school number: ");
        if (scanf("%d", &min_school) != 1) {
            return -7;
        }

        printf("Enter maximum school number: ");
        if (scanf("%d", &max_school) != 1) {
            return -8;
        }

        status = generate_students("surnames.txt",
                                   &database,
                                   count,
                                   min_group,
                                   max_group,
                                   min_school,
                                   max_school);

        if (status != 0) {
            free_students(&database);
            return -9;
        }
    } else {
        return -10;
    }

    sort_students(&database);
    problems_before = count_problem_students(&database);

    status = copy_database(&database, &original_database);

    if (status != 0) {
        free_students(&database);
        return -11;
    }

    redistribute_students(&database);
    problems_after = count_problem_students(&database);

    f_out = fopen("data.res", "w");

    if (f_out == NULL) {
        free_students(&database);
        free_students(&original_database);
        return -12;
    }

    fprintf(f_out, "Sorted list before redistribute:\n");
    write_students(f_out, &original_database);

    fprintf(f_out, "\n\nProblem students before redistribute: %d\n\n",
            problems_before);

    fprintf(f_out, "Sorted list after redistribute:\n");
    write_students(f_out, &database);

    fprintf(f_out, "\n\nProblem students after redistribute: %d\n",
            problems_after);

    fclose(f_out);
    f_out = NULL;

    free_students(&database);
    free_students(&original_database);

    return 0;
}
