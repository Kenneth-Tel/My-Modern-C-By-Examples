/* Name: string_pointer_vs_array.c
 * Author: Kevin
 * - Link: https://github.com/Kenneth-Tel
 * Tags: [unnamed string literals, array, pointer]
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    char *ptrName = "Kevin" " Telling"; // There exists an unnamed string literal "Kevin Telling" which lives in the read-only data segment of the program, which ptrName points to.
    char arrName[] = "Kevin" " Telling"; // The unnamed string literal gets copied into arrName's place into the stack. 2 Objects in memory. 

    printf("arrName: %s\n", arrName);
    arrName[0] = 'L';
    printf("arrName: %s\n", arrName);

    ptrName[0] = 'L'; // Crashes. (ASAN error if -fsanitize=address)

    return EXIT_SUCCESS;
}

