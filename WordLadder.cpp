/**
 * File: WordLadder.cpp
 * --------------
 * Implements WordLadder.
 */
	
#include <fstream>		//For filestreams
#include <iostream>		//For cout
#include <string>		//For getline
#include "lexicon.h"	//For lexicon
#include "console.h"	//For getLine
#include "queue.h"		//For queue
#include "vector.h"		//For Vector
using namespace std;

//Prototypes
static void welcome();
static bool getEnglishWord(const Lexicon& dictionary, string& word, string prompt);
static bool checkWordEndpoints(const Lexicon& dictionary, const string& sourceWord, string& destinationWord);
static Vector<string> getWordLadder(const Lexicon& dictionary, const string& sourceWord, const string& destinationWord);
static Lexicon oneHopAway(const Lexicon& dictionary, Lexicon& usedWords, const string& topWord);
static void printWordLadder(const Vector<string>& wordLadder, const string& sourceWord, const string& destinationWord);

/**
 * Function: main
 * The main welcomes a user and continues to offer to find a word ladder
 * until the user hits return.  It leverages helper functions getEnglishWord and
 * checkWordEndpoints to validate both user input of a string and a 
 * whether the words can form a ladder, respectively.  Once successful
 * entries have been received, it calculates the shortest word ladder
 * using getWordLadder and prints it out using printWordLadder.
 */
int main() {
	Lexicon dictionary("dictionary.txt");

    welcome();
    while(true) {
    	string sourceWord;
		string destinationWord;
		Vector<string> wordLadder;

    	if(getEnglishWord(dictionary, sourceWord, "Please enter the source word [return to quit]: ")) break;
		if(getEnglishWord(dictionary, destinationWord, "Please enter the destination word [return to quit]: ")) break;
		if(checkWordEndpoints(dictionary, sourceWord, destinationWord)) break;

		wordLadder = getWordLadder(dictionary, sourceWord, destinationWord);
		printWordLadder(wordLadder, sourceWord, destinationWord);
    }
    cout << "\nThanks for playing!" << endl;
    return 0;
}

/**
 * Procedure: welcome()
 * Prints out a greeting on two lines.
 */
static void welcome() {
	cout << "Welcome to the CS106 word ladder application!" << endl;
	cout << "Please give me two English words, and I will change the first into the second by changing one letter at a time." << endl << endl;
}

/**
 * Function: getEnglishWord
 * getEnglishWord is similar to the Stanford getLine function but also adds
 * the program-specific requirements of checking if the user's input is an
 * English word or blank. Rather than calling a separate helper function
 * to determine if the user's input is an English word and housing all
 * condition checks in the main, this function can be called to check any word.
 * It returns a boolean as the check.
 * Parameters: dictionary declared in main as constant reference, a user-inputted word
 * that needs to be validated as a constant reference, and the prompt as a literal
 */
static bool getEnglishWord(const Lexicon& dictionary, string& word, string prompt) {
	while(true) {
		cout << prompt;
		getline(cin, word);
		if(word.empty()) {
			return true;
		} else if(dictionary.contains(word)) {
			return false;
		}
		cout << "Your response needs to be an English word, so please try again." << endl;
	}
}

/**
 * Function: checkWordEndpoints
 * Simple helper function called from the main() that verifies if the user's source and
 * destination words are compatible for a word ladder, and if not, prompts the user to 
 * change their destination word to conform. It returns a boolean in the event that the 
 * user cancels out of the application upon being prompted for a conforming destination word.
 * Parameters: dictionary cleared in main as constant reference, user-inputted source word as a constant reference,
 * and a reference to the destination word in case it needs to be altered
 */
static bool checkWordEndpoints(const Lexicon& dictionary, const string& sourceWord, string& destinationWord) {
	if(sourceWord.length() != destinationWord.length()) {
		cout << "The two endpoints must contain the same number of characters, or else no word ladder can exist." << endl;
		return getEnglishWord(dictionary, destinationWord, "Please enter the destination word [return to quit]: ");
	}
	return false;
}

/**
 * Function: getWordLadder
 * getWordLadder does most of the heavy lifting in terms of finding the appropriate ladder,
 * enlisting oneHopAway at one point to return all English words with one character differing.
 * It follows the structure outlined in the spec, which at high-level puts in a queue 
 * the possible ladders in ascending length, where ladders are a Vector of strings, and
 * checks the top of the queue (the last word in the shortest ladder) against the destination word passed in as
 * a constant reference. If that check fails, the function adds those words one hop away
 * to the queue via a vector. If the check succeeds, the comporting Vector of strings is returned.
 * Parameters: dictionary cleared in main as constant reference, user-inputted source and destination 
 * words as constant references
 */
static Vector<string> getWordLadder(const Lexicon& dictionary, const string& sourceWord, const string& destinationWord) {
	Queue< Vector<string> > paths;
	Vector<string> wordLadder;
	Lexicon usedWords;

	wordLadder.add(sourceWord);
	usedWords.add(sourceWord);
	paths.enqueue(wordLadder);
	while(!paths.isEmpty()) {
		wordLadder = paths.dequeue();
		string topWord = wordLadder[wordLadder.size()-1];
		if(topWord == destinationWord) {
			break;
		} else {
			for(string word: oneHopAway(dictionary, usedWords, topWord)) {
				Vector<string> wordLadderClone = wordLadder;
				wordLadderClone.add(word);
				paths.enqueue(wordLadderClone);
			}
		}
	}
	return wordLadder;
}

/**
 * Function: oneHopAway
 * This helper function loops through each letter of the alphabet
 * at each position for a word passed through, checking whether
 * the newly formed word is both English and hasn't been used 
 * previously.  The latter check is maintained by a Lexicon
 * passed through by reference.
 * Parameters: dictionary cleared in main as constant reference, Lexicon of used words
 * passed by reference since it can be altered, constant reference to the word 
 * looking to build off of
 */
static Lexicon oneHopAway(const Lexicon& dictionary, Lexicon& usedWords, const string& topWord) {
	Lexicon oneHopAway;
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	for(int i = 0; i < topWord.length(); i++) {
		for(int j = 0; j < alphabet.length(); j++) {
			string testWord = topWord;
			testWord[i] = alphabet[j];
			if(dictionary.contains(testWord) && !usedWords.contains(testWord)) {
				oneHopAway.add(testWord);
				usedWords.add(testWord);
			}
		}
	}
	return oneHopAway;
}

/*
 * Procedure: printWordLadder
 * Final helper function that takes in the comporting word ladder, which may not exist,
 * and prints out the corresponding result for the source and destination words given.
 */
static void printWordLadder(const Vector<string>& wordLadder, const string& sourceWord, const string& destinationWord) {
	if(sourceWord == destinationWord) {
		cout << "Found ladder: " << wordLadder[0];
	} else if(wordLadder.size() == 1) {
		cout << "No word ladder between \"" << sourceWord << "\" and \"" << destinationWord << "\" could be found.";
	} else {
		cout << "Found ladder:";
		for(string word: wordLadder) {
			cout << ' ' << word;
		}
	}
	cout << endl << endl;
}