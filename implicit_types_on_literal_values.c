/* Name: implicit_types_on_literal_values.c
 * Author: Kevin
 * - Link: https://github.com/Kenneth-Tel
 * Tags: [Literal Types, Literals, Numerical Literals]
 */

#include <stdio.h>

#include <stdlib.h> // for EXIT_SUCCESS
#include <limits.h> // for [TYPE]_MAX & [TYPE]_MIN

// Literals Static Type Analysis
// Assume 64bit x86
int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    int a = -1; //  Negative scalars are not valid numerical literals. We are performing 2 operations, a unary minus, then an assignment operator.
    float b = 12E-1; // Negative scalars for the exponent are valid floating point numerical literals, only 1 operation is happening.
    printf("%d, %f\n", a, b);

    // Takeaway 5.1.2 #1 All values have a type that is statically determined.
    // If you want to manually control the types for a literal, you use suffixes do indicate it.
    // Otherwise, the C standard has a hierarchy for type assignment for literals. Eg, ints: int -> long -> long long

    // For Hex/Octal/Binary, is can get a bit funky: int -> unsigned int -> long -> ...
    printf("INT  : [%d;%d]\n", INT_MIN, INT_MAX);
    printf("LONG : [%ld;%ld]\n", LONG_MIN, LONG_MAX);

    int intMax = 0x7FFF'FFFF; // 2'147'483'647
    int hexInt = -0x8000'0000; // -2'147'483'648 ? int: [-2147483648; 2147483647]
    long hexLong = -0x8000'0000; // -2'147'483'648 ? long: [-9,223,372,036,854,775,808; 9,223,372,036,854,775,807]
    size_t hexSize_t = -0x8000'0000; // -2'147'483'648 ?

    printf("intMax     = %d\nhexInt    = %d\nhexLong    = %ld\nhexSize_t  = %zu\n", intMax, hexInt, hexLong, hexSize_t);

    // Takeaway 5.3 #6 Don’t use binary, octal, or hexadecimal literals for negative values

    // int hexD = -0x80000000 * 2; 0
    // int hexD = -0x80000000 * 1.5; This actually gets nuanced.
    // In general: FE_INVALID, then, Unspecified Behaviour: See https://open-std.org/JTC1/SC22/WG14/www/docs/n3550.pdf#subsection.0.15.4.2. Under __STDC_IEC_60559_BFP__. However, using --ffast-math, it defaults to C23 standard, and becomes Undefined Behaviour
    // int hexD = -0x80000000L * 2; Platform dependent, OS dependent => Implementation Defined

    return EXIT_SUCCESS;
}


