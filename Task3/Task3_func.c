#include "prototypes3.h"

int search(const char *SInputFile, char *word) {
    FILE *file;
    char str[513];
    char current_word[513];
    int max_len = 0;

    if (word == NULL || SInputFile == NULL) {
        return -1;
    }

    word[0] = '\0';

    file = fopen(SInputFile, "r");
    if (file == NULL) {
        return -1;
    }

    while (fgets(str, 513, file) != NULL) {
        int i = 0;

        while (str[i] != '\0') {
            int j = 0;
            int has_upper = 0;
            int only_english = 1;

            while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
                i++;
            }

            if (str[i] == '\0') {
                break;
            }

            while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
                current_word[j] = str[i];

                if (str[i] >= 'A' && str[i] <= 'Z') {
                    has_upper = 1;
                }

                if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))) {
                    only_english = 0;
                }

                i++;
                j++;
            }

            current_word[j] = '\0';

            if (j > 0 && only_english && has_upper && j > max_len) {
                int t;

                max_len = j;
                for (t = 0; t <= j; ++t) {
                    word[t] = current_word[t];
                }
            }
        }
    }

    fclose(file);
    return 0;
}

int main_function(void) {
    char filename[256];
    char word[513];
    int status;

    printf("Input file name: ");
    scanf("%255s", filename);

    status = search(filename, word);
    if (status == -1) {
        printf("Could not open file!\n");
        return -1;
    }

    if (word[0] == '\0') {
        printf("No suitable word found.\n");
    } else {
        printf("Longest suitable word: %s\n", word);
    }

    return 0;
}
