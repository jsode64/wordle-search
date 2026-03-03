#include "def.h"
#include "hint.h"
#include "word.h"
#include <print>

i32 main(i32 argc, cstr argv[]) {
    try {
        const Hint hint{static_cast<uZ>(argc), argv};
        hint.print_matches();
        return 0;
    } catch (std::string s) {
        std::println("Error: {}", s);
        return 1;
    }
}
