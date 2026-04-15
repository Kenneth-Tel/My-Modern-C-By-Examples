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

    // To simulate the char* with VLA, we can create a const-qualified object, to the constant "Kevin Telling"
    char const arrConstName[] = "Kevin " "Telling";
    arrConstName[0] = 'L'; // Will not crash, because the program wont compile. So try to use this.

    return EXIT_SUCCESS;
}

