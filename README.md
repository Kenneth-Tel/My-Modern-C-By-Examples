# My Modern C23 By Examples
While reading Modern C, Third Edition, some thoughts came up about the implementation of what was being told.
This is suppose to be a scratchpad like repository, to collect my thought.

Some snippets are short and concise. Some are longer and more convoluted.

## 1. String Arrays and Concatenation
The C Standard specifies that multi-string literals are concatenated into a single literal during compilation. 
This happens before the code is run.

Mutable vs Immutable String Literals Initialization
  
<details>
<summary>📝 Code Snippet</summary>

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
This sections explains why, with C's abstract state model, values are compared only with semantic value, rather than bit representaion.
Meaning (0.0f == -0.0f) yields true, even though they have different bit patterns.

> [!WARNING]  
> It started off as a C abstract machine note, but then it diverges into something else.

<details>
<summary>📝 Code Snippet</summary>

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
</details>

## 3. Implicit Type Inference on Literals
In C, all values must have a type, including literals.
Without suffixes to indicate the literal type, the type must be inferred, in accordance with the C23 standard and notably __STDC_IEC_60559_BFP__.

<details>
<summary>📝 Code Snippet</summary>

```C
#include <stdio.h>

#include <stdlib.h> // for EXIT_SUCCESS
#include <limits.h> // for [TYPE]_MAX & [TYPE]_MIN

// Assume 64bit x86
int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    int a = -1; //  Negative scalars are not valid numerical literals. We are performing 2 operations, a unary minus, then an assignment operator.
    float b = 12E-1; // Negative scalars for the exponent are valid floating point numerical literals, only 1 operation is happening.
    printf("%d, %f\n", a, b);

    // The C standard has a hierarchy for type assignment for literals. Eg, ints: int -> long -> long long

    // For Hex/Octal/Binary, is can get a bit funky: int -> unsigned int -> long -> ...
    printf("INT  : [%d;%d]\n", INT_MIN, INT_MAX);
    printf("LONG : [%ld;%ld]\n", LONG_MIN, LONG_MAX);

    int intMax = 0x7FFF'FFFF; 
    int hexInt = -0x8000'0000; 
    long hexLong = -0x8000'0000;
    size_t hexSize_t = -0x8000'0000;

    printf("intMax     = %d\nhexInt    = %d\nhexLong    = %ld\nhexSize_t  = %zu\n", intMax, hexInt, hexLong, hexSize_t);

    // int hexD = -0x80000000 * 2; 0
    // int hexD = -0x80000000 * 1.5; This actually gets nuanced.
    // In general: FE_INVALID, then, Unspecified Behaviour: See https://open-std.org/JTC1/SC22/WG14/www/docs/n3550.pdf#subsection.0.15.4.2. Under __STDC_IEC_60559_BFP__. However, using --ffast-math, it defaults to C23 standard, and becomes Undefined Behaviour
    // int hexD = -0x80000000L * 2; Platform dependent, OS dependent => Implementation Defined

    return EXIT_SUCCESS;
}
```
Produces: 
```bash
-1, 1.200000
INT  : [-2147483648;2147483647]
LONG : [-9223372036854775808;9223372036854775807]
intMax     = 2147483647
hexInt    = -2147483648
hexLong    = 2147483648
hexSize_t  = 2147483648
```
</details>

<details>
<summary>Hex Inference Explanation</summary>


I feel like the point of the code above is a bit opaque, so lets trim the fat.

```C
int hexInt = -0x8000'0000;      // yields -2147483648
long hexLong = -0x8000'0000;    // yields 2147483648
```

What type is 0x8000'0000?
1. That hex pattern has the value: 2,147,483,648.
2. That value does not fit within a standard 32-bit signed int (max 2,147,483,647).
3. However, it fits perfectly within an unsigned int (max 4,294,967,295).
4. Per C/C++ rules, the literal 0x8000'0000 is assigned the type unsigned int.
5. The unary minus operator is applied. Since the operand is unsigned, the calculation follows unsigned arithmetic rules (wrapping around).
6. The result of -0x8000'0000 is 2,147,483,648, but the type is still unsigned int.

Now, look at the assignments:

Case A: int hexInt = -0x8000'0000;
- We try to store 2,147,483,648 into a signed int.
- This exceeds the maximum limit of a signed int.
- On two's complement systems, the bits (0x8000'0000) are reinterpreted as a negative number.
- Result: -2147483648.

Case B: long hexLong = -0x8000'0000;
- We try to store 2,147,483,648 into a long.
- Assuming a 64-bit long, this value fits comfortably.
- Result: 2147483648.
</details>

> [!NOTE]
> 1. Takeaway 5.1.2 #1 All values have a type that is statically determined.
> 2. Takeaway 5.3 #6 Don’t use binary, octal, or hexadecimal literals for negative values
> 3. If you want to manually control the types for a literal, you use suffixes do indicate it.


## 4. Enum Initialization.
Enums is used to connect data through our meaning of the data.
In other words, we can imbue our own semantics into the data organization.
By convention, we add aName_num, at the end of the enum, to indicate how big an object should be to contain each connected data point.


<details>
<summary>📝 Code Snippet</summary>

```C
#include <stdio.h>

#include <stdlib.h> // for EXIT_SUCCESS
#include <limits.h> // for [TYPE]_MAX & [TYPE]_MIN

// Assume 64bit x86
int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    int a = -1; //  Negative scalars are not valid numerical literals. We are performing 2 operations, a unary minus, then an assignment operator.
    float b = 12E-1; // Negative scalars for the exponent are valid floating point numerical literals, only 1 operation is happening.
    printf("%d, %f\n", a, b);

    // The C standard has a hierarchy for type assignment for literals. Eg, ints: int -> long -> long long

    // For Hex/Octal/Binary, is can get a bit funky: int -> unsigned int -> long -> ...
    printf("INT  : [%d;%d]\n", INT_MIN, INT_MAX);
    printf("LONG : [%ld;%ld]\n", LONG_MIN, LONG_MAX);

    int intMax = 0x7FFF'FFFF; 
    int hexInt = -0x8000'0000; 
    long hexLong = -0x8000'0000;
    size_t hexSize_t = -0x8000'0000;

    printf("intMax     = %d\nhexInt    = %d\nhexLong    = %ld\nhexSize_t  = %zu\n", intMax, hexInt, hexLong, hexSize_t);

    // int hexD = -0x80000000 * 2; 0
    // int hexD = -0x80000000 * 1.5; This actually gets nuanced.
    // In general: FE_INVALID, then, Unspecified Behaviour: See https://open-std.org/JTC1/SC22/WG14/www/docs/n3550.pdf#subsection.0.15.4.2. Under __STDC_IEC_60559_BFP__. However, using --ffast-math, it defaults to C23 standard, and becomes Undefined Behaviour
    // int hexD = -0x80000000L * 2; Platform dependent, OS dependent => Implementation Defined

    return EXIT_SUCCESS;
}
```
Produces: 
```bash
Nike's revenue in 2025 is 51500000000
Uniqlo's revenue in 2025 is 22500000000
H&M's revenue in 2025 is 23500000000
Under Armor's revenue in 2025 is 5600000000
Gucci's revenue in 2025 is 10000000000
```
</details>
