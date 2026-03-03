#pragma once

#include "def.h"
#include <array>
#include <string_view>

/** A Wordle word optimized for fast checking. */
class Word {
private:
    /** The word. */
    char word[5];

    /** A mask of all contained letters. */
    u32 containedMask;

    /** Green letters in order by indices. */
    u32 greens;

    /** Throws an exception if the word is invalid (not 5 lowercase letters). */
    constexpr void validate_word(std::string_view s) {
        const auto isLower = [](char c) { return c >= 'a' && c <= 'z'; };
        if (s.size() != 5) {
            throw "Word must be five letters long";
        }
        if (!(isLower(s[0]) && isLower(s[1]) && isLower(s[2]) && isLower(s[3]) && isLower(s[4]))) {
            throw "Word must contain only lowercase letters";
        }
    }

public:
    consteval Word(std::string_view s) : word{'a'}, containedMask{0}, greens{0} {
        // Make sure the word is valid.
        validate_word(s);

        for (uZ i = 0; i < 5; i++) {
            const u8 c = s[i];
            const u32 letterI = u32{c} - 'a';

            // Put letter in word.
            word[i] = c;

            // Add to contained letters.
            containedMask |= 1u << letterI;

            // Add to green letters.
            greens |= letterI << (i * 5);
        }
    }

    /** Returns the word. */
    std::string_view get_word() const;

    /** Returns the word's contained letter mask. */
    u32 get_contained_mask() const;

    /** Returns the word's green letters. */
    u32 get_greens() const;
};
