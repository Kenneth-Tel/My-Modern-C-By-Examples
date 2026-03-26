# My-Modern-C-By-Examples
My own little artifact repository from reading Modern C, third Edition.

## Code Snippets
[[string_array_concatenation.c]]
```C
#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    char message[] = "Hello, " "World! "
        "Welcome to my program! " "Its a cool program right?";

    printf("%s\n", message);

    message[0] = 'W'; // The code defines message as an array of characters, not a pointer to a string literal.
    printf("%s\n", message);

    return EXIT_SUCCESS;
}
```
Produces:
```bash
Hello, World! Welcome to my program! Its a cool program right?
Wello, World! Welcome to my program! Its a cool program right?
```
