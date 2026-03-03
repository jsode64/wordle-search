#include "hint.h"
#include "words.h"
#include <cstring>
#include <format>
#include <print>
#include <random>
#include <string>

Hint::Hint(uZ argc, cstr argv[]) : Hint{} {
    for (uZ i = 1; i < argc; i++) {
        const cstr arg = argv[i];

        if (arg[0] == ':') {
            parse_options(arg + 1);
            ;
        } else if (std::memcmp(arg, "-C", 2) == 0) {
            parse_contained(arg + 2);
        } else if (std::memcmp(arg, "-X", 2) == 0) {
            parse_grays(arg + 2);
        } else if (std::memcmp(arg, "-G", 2) == 0) {
            parse_greens(arg + 2);
        } else if (std::memcmp(arg, "-Y", 2) == 0) {
            parse_yellows(arg + 2);
        } else {
            throw std::format("Unknown input: `{}`", arg);
        }
    }

    // Prevent gray doubles from messing with selection.
    grayMask &= !containedMask;
}

bool Hint::matches_with(Word word) const {
    // Check if all contained letters are present.
    if ((containedMask & word.get_contained_mask()) != containedMask) {
        return false;
    }

    // Check if any green letters are missing.
    if ((word.get_greens() & greenMask) != greens) {
        return false;
    }

    // Check if any gray letters are present.
    if ((grayMask & word.get_contained_mask()) != 0) {
        return false;
    }

    // Check if any yellows are in their already disproven place.
    for (uZ i = 0; i < nYellows; i++) {
        const auto [yellow, yellowMask] = yellows[i];
        if ((word.get_greens() & yellowMask) == yellow) {
            return false;
        }
    }

    return true;
}

void Hint::parse_options(cstr s) {
    for (uZ i = 0; s[i] != '\0'; i++) {
        const char c = s[i];

        if (c == 'r' || c == 'R') {
            wantsRandom = true;
        } else if (c == 'q' || c == 'Q') {
            wantsQuiet = true;
        } else {
            throw std::format("Expected option, found `{}`", c);
        }
    }
}

void Hint::parse_contained(cstr s) {
    for (uZ i = 0; s[i] != '\0'; i++) {
        const u8 c = s[i];

        // Make sure the character is valid.
        if (c < 'a' || c > 'z') {
            throw std::format("Expected letter, found `{}`", static_cast<char>(c));
        }

        // Mark the letter as contained.
        const u32 letterI = u32{c} - 'a';
        containedMask |= 1u << letterI;
    }
}

void Hint::parse_grays(cstr s) {
    for (uZ i = 0; s[i] != '\0'; i++) {
        // Make sure input is valid.
        const u8 c = s[i];
        if (c < 'a' || c > 'z') {
            throw std::format("`-X` takes lowercase letters, found `{}`", static_cast<char>(c));
        }

        // Apply letter to mask.
        const u32 letterI = u32{c} - 'a';
        grayMask |= 1u << letterI;
    }
}

void Hint::parse_greens(cstr s) {
    for (uZ i = 0; s[i] != '\0'; i++) {
        // Make sure we don't go out of range.
        if (i >= 5) {
            throw std::string("`-G` only takes up to five inputs");
        }

        // Ignore underscores/dashes.
        const u8 c = s[i];
        if (c == '-' || c == '_') {
            continue;
        }

        // Put letter in place.
        const u32 letterI = u32{c} - 'a';
        const u32 shift = 5 * i;
        const u32 letterMask = 0x0000001Fu;
        greens |= letterI << shift;
        greenMask |= letterMask << shift;

        // Mark the letter as contained.
        containedMask |= 1u << letterI;
    }
}

void Hint::parse_yellows(cstr s) {
    u32 col = 0;
    for (uZ i = 0; s[i] != '\0'; i++) {
        const u8 c = s[i];

        if (c == ',') {
            // Increase column.
            col++;
            if (col >= 5) {
                throw std::string("Can only give 5 columns for yellows");
            }
        } else if (c >= 'a' && c <= 'z') {
            const u32 letterI = u32{c} - 'a';

            // Mark the letter as contained.
            containedMask |= 1u << letterI;

            // Push the yellow.
            const u32 shift = 5 * col;
            const u32 letterMask = 0x0000001Fu;
            yellows[nYellows] = std::tuple(letterI << shift, letterMask << shift);
            nYellows++;
        } else {
            throw std::format("Unknown `-Y` parameter: `{}`", static_cast<char>(c));
        }
    }
}

void Hint::print_matches() const {
    // Set up RNG.
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());

    std::string choice{"-----"};
    u32 n = 0;
    for (const auto& word : WORDS) {
        // Skip words that don't match.
        if (!matches_with(word)) {
            continue;
        } else {
            n++;
        }

        // Choose next random word.
        if (wantsRandom) {
            const bool choose = std::uniform_int_distribution<u32>(1, n)(rng) == 1;
            if (choose) {
                choice = std::string(word.get_word());
            }
        }

        // Print the word.
        if (!wantsQuiet) {
            std::println("{}", word.get_word());
        }
    }

    // Print number of words and random word (if wanted).
    std::println(" {} words found", n);
    if (wantsRandom) {
        std::println(" Random word: {}", choice);
    }
}
