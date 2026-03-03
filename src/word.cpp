#include "word.h"

std::string_view Word::get_word() const { return std::string_view(word, 5); }

u32 Word::get_contained_mask() const { return containedMask; }

u32 Word::get_greens() const { return greens; }
