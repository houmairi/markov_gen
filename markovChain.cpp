#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <random>

class MarkovChain {
private:
    // This map stores the state transitions
    // The key is a word, and the value is a vector of possible next words
    std::map<std::string, std::vector<std::string>> transitions;

    // Random number generator for selecting next words
    std::random_device rd;
    std::mt19937 gen;

public:
    MarkovChain() : gen(rd()) {}

    // Function to add a transition to the Markov chain
    void addTransition(const std::string& current, const std::string& next) {
        transitions[current].push_back(next);
    }

    // Function to get a random next word given the current word
    std::string getNextWord(const std::string& current) {
        if (transitions.find(current) == transitions.end()) {
            return "";  // Return empty string if no transitions found
        }
        
        const auto& possibleNextWords = transitions[current];
        std::uniform_int_distribution<> dis(0, possibleNextWords.size() - 1);
        return possibleNextWords[dis(gen)];
    }

    // Function to build the Markov chain from input text
    void buildChain(const std::vector<std::string>& words) {
        for (size_t i = 0; i < words.size() - 1; ++i) {
            addTransition(words[i], words[i + 1]);
        }
    }
};

// Utility function to split a string into words
std::vector<std::string> splitIntoWords(const std::string& text) {
    std::vector<std::string> words;
    std::string word;
    for (char c : text) {
        if (std::isalnum(c)) {
            word += c;
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

int main() {
    MarkovChain chain;

    // Example usage
    std::string inputText = "Die Technik ist ein Hund."                                                             //
                            "Das letzte Auto ist immer ein Kombi."                                                  //
                            "Zuwider ist er nicht. Aber ein Ziegenbock ist auch nicht zuwider... Stinken tut er.";   // austrian quotes
                            
    std::vector<std::string> words = splitIntoWords(inputText);
    chain.buildChain(words);

    // Generate some text
    std::string currentWord = words[0];  // Start with the first word
    for (int i = 0; i < 10; ++i) {
        std::cout << currentWord << " ";
        currentWord = chain.getNextWord(currentWord);
        if (currentWord.empty()) break;
    }
    std::cout << std::endl;

    return 0;
}