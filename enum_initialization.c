/* Name: enum_initialization.c
 * Author: Kevin
 * - Link: https://github.com/Kenneth-Tel
 * Tags: [enum]
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    enum brands : unsigned char { nike, uniqlo, hm, under_armor, gucci, brands_num };

    char const* const brand_name[brands_num] = { 
        [nike] = "Nike",
        [uniqlo] = "Uniqlo",
        [hm] = "H&M",
        [under_armor] = "Under Armor",
        [gucci] = "Gucci",
    };

    // only x64. Should use uint64_t
    size_t const revenue[brands_num] = {
        [nike] = 51'500'000'000,
        [uniqlo] = 22'500'000'000,
        [hm] = 23'500'000'000,
        [under_armor] = 5'600'000'000,
        [gucci] = 10'000'000'000,
    };

    for (size_t i = 0; i < brands_num; ++i) {
        printf("%s's revenue in 2025 is %zu\n", brand_name[i], revenue[i]);
    }

    return EXIT_SUCCESS;
}

