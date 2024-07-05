# Markov Chain Text Generator

A C++ implementation of a Markov Chain Text Generator that works with both English and German texts.

## Features

- Generates pseudo-random text based on input samples
- Supports Unicode characters, including German umlauts and ß
- Works with Windows console for proper Unicode display

## Key Functions

- `MarkovChain`: Main class for the Markov Chain implementation
- `splitIntoWords`: Processes input text into individual words
- `generateText`: Produces new text based on the built Markov Chain

## Core Logic

1. Text Processing:
   - Split input text into words
   - Convert words to lowercase
   - Handle special characters (e.g., German umlauts)

2. Building the Markov Chain:
   - Create transitions between consecutive words
   - Store transitions in a map structure

3. Text Generation:
   - Start with a random or specified word
   - Repeatedly select next words based on transition probabilities
   - Handle dead ends by restarting from a random word

4. Unicode Handling:
   - Use wide strings (`std::wstring`) for Unicode support
   - Set Windows console to UTF-16 mode for proper display

## How to Compile and Run

Compile (using MinGW-w64 on Windows):
```
g++ markovChain.cpp -o textgen
```

Run:
```
./textgen.exe
```

## Current Limitations

- First-order Markov Chain (considers only the current word)
- Input text is currently hardcoded in the program
- Optimized for Windows console output

## Next Steps

- Implement file I/O for larger input texts
- Enhance to higher-order Markov Chains for more coherent output
- Add save/load functionality for Markov Chains
- Create a user interface for easier interaction