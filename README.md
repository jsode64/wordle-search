# Wordle Search

Wordle Search is a command line tool to get the possible answers to a Wordle based on your previous hints.

# Usage

Wordle search takes command line arguments for options/hints.

## Hints
- `-C`, contain letters: only words that contain the following letters
- `-X`, exclude letters: only words that don't contain any of the following letters
- `-Y`, yellow letters: only words with these letters in different spots
- `-G`, green letters: only words with these letters in these spots

## Options
- `:r`: give me a random word of the ones that match
- `:q`: quiet; don't print words (will still print the number of words and a random one if requested)

# Example

Given this Wordle game:

```
SAUTE ⬛🟨⬛⬛🟨
CLEAN 🟨⬛🟨🟩🟨
```

I could ask Wordle Search for what the remaining possible answers are with these arguments:

```-Xsutl```
The grays from "saute" and "clean", order doesn't matter.

```-Yc,a,e,,en```
The yellows. Starts at the left column and each comma moves it one to the right.

```-G---a-```
The greens. Each dash is a column with no green.

So, running:

```wordle-search -Xsutl -Yc,a,e,,en -G---a-```

will print:

```
eniac
 1 words found
```

So the only word that it can be is ```eniac```.
