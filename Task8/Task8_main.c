#include "prototypes8.h"

int main(void) {
    int status = main_function();

    if (status != 0) {
        printf("Program finished with error: %d\n", status);
    }

    return status;
}
