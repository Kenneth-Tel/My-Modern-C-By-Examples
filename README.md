# My-Modern-C-By-Examples
My own little artifact repository from reading Modern C, third Edition.

## Code Snippets
<details>
<summary>String Arrays and Concatenation</summary>

**Explanation**: The **C Standard** specifies that consecutive string literals are concatenated into a larger string.

> [!NOTE]
> While it's the compilers job to concatenate it, the C Standard demands this behaviour: Defined Behaviour.

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
Takeaway 5.3 #1 Consecutive string literals are concatenated.
</details>

