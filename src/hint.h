#pragma once

#include "def.h"
#include "word.h"
#include <array>
#include <tuple>

/** Hints from previous Wordle guesses. */
class Hint {
private:
    /** Did the user want a random word? */
    bool wantsRandom;

    /** Does the user want a quiet run? */
    bool wantsQuiet;

    /** A mask of letters that are not in the word. */
    u32 grayMask;

    /** Green letters in order by indices. */
    u32 greens;

    /** A mask of the bits present in `greens`. */
    u32 greenMask;

    /** A mask of contained letters. */
    u32 containedMask;

    /** Yellows. First `u32` is the value in position, second is the mask. */
    std::array<std::tuple<u32, u32>, 25> yellows;

    /** The number of yellows. */
    uZ nYellows;

    /** Returns `true` if the word works with the hints, `false` it not. */
    bool matches_with(Word word) const;

    /** Parses options from the given string. */
    void parse_options(cstr s);

    /** Reads the caracters marking the letters as contained in the word. */
    void parse_contained(cstr s);

    /** Reads until a null terminator, tracking the characters into `grayMask`. */
    void parse_grays(cstr s);

    /** Reads until a null terminator, putting the characters in place into the green mask. */
    void parse_greens(cstr s);

    /** Reads until a null terminator, tracking the yellow letter info. */
    void parse_yellows(cstr s);

public:
    constexpr Hint()
        : wantsRandom{false}, wantsQuiet{false}, grayMask{0}, greens{0}, greenMask{0},
          containedMask{0}, yellows{std::tuple(0, 0)}, nYellows{0} {}

    /** Reads the command line arguments to generate the hints. */
    Hint(uZ argc, cstr argv[]);

    /** Prints all words that work with the hint. */
    void print_matches() const;
};
