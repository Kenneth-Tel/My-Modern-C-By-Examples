# My-Modern-C-By-Examples
My own little artifact repository from reading Modern C, third Edition.

## 1. String Arrays and Concatenation
**The Concept**

The C Standard specifies that adjacent string literals are concatenated into a single literal during compilation. 
This happens before the code is run.

Mutable vs Immutable String Literals Initialization
  
<details>
<summary>Code Snipped</summary>

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
</details>

> [!NOTE]
> 1. Takeaway 5.3 #1 Consecutive string literals are concatenated.
> 2. While it's the compilers job to concatenate it, the C Standard demands this behaviour: Defined Behaviour.

## 2. Values and Representation.
> [!WARNING]  
> It started off as a C abstract machine note, but then it diverges into something else.

```C
#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS

#include <stdint.h> // for SIZE_MAX
#include <string.h> // for memcpy

// Assumes: sizeof(input) < sizeof(size_t), x86 LE
void printBits(char msg[], void* startAddress, size_t size) {
    size_t bits = 0;
    memcpy(&bits, startAddress, size);
    // Prints in BE
    printf("%15s0x%0*zX\n", msg, (int)size * 2, bits);
}

void printIsSame(char msg[], bool isSame) {
    printf("%15s%s\n", msg, isSame ? "true" : "false");
}

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    bool isSame;

    // Defined Behaviour
    // Both have same bit pattern.
    size_t a = -1;
    size_t max = SIZE_MAX;
    isSame = ( a == max );

    printBits("a: ", &a, sizeof(a));
    printBits("max: ", &max, sizeof(max));
    printIsSame("a == max: ", isSame);

    printf("\n");

    // Twos complement: +0 and -0 have same bit patterns.
    int int1 = 0;
    int int2 = -0;

    isSame = (int1 == int2); // Using one's complement: 0000 = 0, 1111 = -0: 0 == -0, would also define true, due to C's abstract state machine model. However, the C23 standard now demands two's complement.

    printBits("int1: ", &int1, sizeof(int1));
    printBits("int2: ", &int2, sizeof(int2));
    printIsSame("int1 == int2: ", isSame);

    printf("\n");

    // IEEE 754 Behaviour: 0.0 and -0.0 are distinct values
    // 0.0f vs -0.0f, have to different bit patterns.
    float float1 = 0.0f;
    float float2 = -0.0f;

    isSame = (float1 == float2); // This is true, even though 0.0 and -0.0 are distinct values. Due to IEEE 754, defining it as such, and therefor also specified by the C standard.

    printBits("0.0f: ", &float1, sizeof(float1));
    printBits("-0.0f: ", &float2, sizeof(float2));
    printIsSame("0.0 == -0.0: ", isSame);

    printf("\n");

    // Float edge cases. 
    float r1 = 1.0f / float1;
    float r2 = 1.0f / float2;

    isSame = ( r1 == r2 );
    // See https://open-std.org/JTC1/SC22/WG14/www/docs/n3550.pdf#chapter.0.13
    // However, it's bad practice do divide by zero, since the C standard (technically) does not define this behaviour, only IEE 754: See __STDC_IEC_559__ (old). __STDC_IEC_60559_BFP__ (new) (Note: --fast-math silently breaks IEEE 754)
    // Usually when you compile without flags, the compiler checks your hardware and chooses accordingly.
    // TLDR: floating points are weird. Make sure you understand them, before you make something complex.
    
    printBits("r1: ", &r1, sizeof(r1));
    printBits("r2: ", &r2, sizeof(r2));
    printIsSame("r1 == r2: ", isSame); // This will yield false, because. While divide 0.0f and -0.0f is defined, their abstract values are different

    return EXIT_SUCCESS;
}
```
Produces:
```bash
            a: 0xFFFFFFFFFFFFFFFF
          max: 0xFFFFFFFFFFFFFFFF
     a == max: true

         int1: 0x00000000
         int2: 0x00000000
 int1 == int2: true

         0.0f: 0x00000000
        -0.0f: 0x80000000
  0.0 == -0.0: true

           r1: 0x7F800000
           r2: 0xFF800000
     r1 == r2: false
```
