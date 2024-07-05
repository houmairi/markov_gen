#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <fcntl.h>
#include <io.h>

class MarkovChain {
private:
    std::map<std::wstring, std::vector<std::wstring>> transitions;
    std::random_device rd;
    std::mt19937 gen;

public:
    MarkovChain() : gen(rd()) {}

    void addTransition(const std::wstring& current, const std::wstring& next) {
        transitions[current].push_back(next);
    }

    std::wstring getNextWord(const std::wstring& current) {
        if (transitions.find(current) == transitions.end()) {
            return L"";
        }
        
        const auto& possibleNextWords = transitions[current];
        std::uniform_int_distribution<> dis(0, possibleNextWords.size() - 1);
        return possibleNextWords[dis(gen)];
    }

    void buildChain(const std::vector<std::wstring>& words) {
        for (size_t i = 0; i < words.size() - 1; ++i) {
            addTransition(words[i], words[i + 1]);
        }
    }

    size_t getTransitionCount() const {
        return transitions.size();
    }

    size_t getNextWordCount(const std::wstring& word) const {
        auto it = transitions.find(word);
        return (it != transitions.end()) ? it->second.size() : 0;
    }

    std::wstring generateText(const std::wstring& startWord, int numWords) {
        std::vector<std::wstring> result;
        std::wstring currentWord = startWord;

        for (int i = 0; i < numWords; ++i) {
            result.push_back(currentWord);
            std::wstring nextWord = getNextWord(currentWord);
            if (nextWord.empty()) {
                auto it = transitions.begin();
                std::advance(it, std::uniform_int_distribution<>(0, transitions.size() - 1)(gen));
                nextWord = it->first;
            }
            currentWord = nextWord;
        }

        std::wostringstream oss;
        std::copy(result.begin(), result.end(), std::ostream_iterator<std::wstring, wchar_t>(oss, L" "));
        return oss.str();
    }
};

std::vector<std::wstring> splitIntoWords(const std::wstring& text) {
    std::vector<std::wstring> words;
    std::wstring word;
    for (wchar_t c : text) {
        if (std::iswalnum(c) || c == L'ä' || c == L'ö' || c == L'ü' || c == L'ß') {
            word += std::towlower(c);
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

void runTests(MarkovChain& chain) {
    // Test 1: Adding transitions
    chain.addTransition(L"hallo", L"welt");
    chain.addTransition(L"hallo", L"dort");
    assert(chain.getTransitionCount() == 1);
    assert(chain.getNextWordCount(L"hallo") == 2);

    // Test 2: Getting next word
    std::wstring nextWord = chain.getNextWord(L"hallo");
    assert(nextWord == L"welt" || nextWord == L"dort");

    // Test 3: Building chain from text
    std::vector<std::wstring> words = {L"der", L"schnelle", L"braune", L"fuchs", L"springt", L"über", L"den", L"faulen", L"hund"};
    chain.buildChain(words);
    assert(chain.getTransitionCount() > 1);
    assert(chain.getNextWordCount(L"der") == 1);

    // Test 4: Generating text
    std::wstring generatedText = chain.generateText(L"der", 5);
    std::wistringstream iss(generatedText);
    std::vector<std::wstring> generatedWords;
    std::copy(std::istream_iterator<std::wstring, wchar_t>(iss), std::istream_iterator<std::wstring, wchar_t>(),
              std::back_inserter(generatedWords));
    assert(generatedWords.size() == 5);

    std::wcout << L"All tests passed!" << std::endl;
}

int main() {
    // Set console output to UTF-16
    _setmode(_fileno(stdout), _O_U16TEXT);

    MarkovChain chain;

    // Run tests
    runTests(chain);

    // Example usage with Austrian quotes
    std::wstring inputText = L"Die Technik ist ein Hund. "
                             L"Das letzte Auto ist immer ein Kombi. "
                             L"Zuwider ist er nicht. Aber ein Ziegenbock ist auch nicht zuwider... Stinken tut er.";

    std::vector<std::wstring> words = splitIntoWords(inputText);
    chain.buildChain(words);

    // Generate some text
    std::wstring generatedText = chain.generateText(words[0], 20);
    std::wcout << L"Generated text: " << generatedText << std::endl;

    return 0;
}