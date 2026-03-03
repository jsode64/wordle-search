words: list[str] = []

with open("words.csv", "r") as src:
    for line in src.readlines():
        words.append(line)

words.sort()
array_len: str = f"{len(words):,}".replace(",", "'")
file_head: str = f"""\
#pragma once

#include <array>
#include "word.h"

/** An array of all valid Wordle guesses. */
static constexpr std::array<Word, {array_len}> WORDS = {{
"""

with open("words.h", "w") as dst:
    dst.write(file_head)

    for word in words:
        dst.write(f"    Word(\"{word.strip()}\"),\n")

    dst.write("};\n")
