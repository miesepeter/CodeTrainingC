#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */


bool valid_step(const char* current_word, const char* next_word) {
	//next word in dictionary
	if(!dictionary_search(next_word))
		return false;

	int diff = 0;
	int len_current = strlen(current_word);
	int len_next = strlen(next_word);

	//different lengths
	if(len_current != len_next) {
		return false;
	}

	//count of different letters
	while(*current_word) {
		if(*current_word != *next_word) {
			diff ++;
		}
		current_word ++;
		next_word ++;
	}

	if(diff == 1) {
		return true;
	}
	else {
		return false;
	}
}


bool display_chain(const char** chain, ostream& output_stream) {
	
	bool first_word = true;
	bool last_word = false;

	while(*chain) {
		if(first_word || last_word) {
			const char* ptr = *chain;
			char ch;
			while(*ptr) {
				ch = toupper(*ptr);
				output_stream << ch;
				ptr ++;
			}
		}
		else {
			const char* ptr = *chain;
			char ch;
			while(*ptr) {
				ch = tolower(*ptr);
				output_stream << ch;
				ptr ++;
			}
		}
		output_stream << endl;
		chain ++;
		
		if(first_word == true) {
			first_word = false;
		}
		if(*(chain + 1) == NULL) {
			last_word = true;
		}
	}
	return true;
}


bool valid_chain(const char** chain) {

	if(valid_link(chain)) {
		//check if any words occur twice (even if not after another)
		while(*chain) {
			const char** next = chain + 1;
			while(*next) {
				if(!strcmp(*chain, *next))
					return false;
				next ++;
			}
			chain ++;
		}
		return true;
		
	}
	return false;
}


bool valid_link(const char** chain, int depth) {
	//ending condition
	if(! *(chain + 1)) {
		if(depth < 2)
			return false;
		else {
			return true;
		}
	}

	//failure condition
	if(!valid_step(*chain, *(chain + 1))) {
		return false;
	}
	
	//recursive next check
	return valid_link(chain + 1, depth + 1);
}


bool find_chain(const char* start_word, const char* target_word, const char** answer_chain, int max_steps, int depth) {
	
	//finding end of answer_chain and adding to it
	answer_chain[depth] = start_word;
	
	if(strcmp(start_word, target_word) == 0) {
		answer_chain[depth + 1] = NULL;
		if(valid_chain(answer_chain))
			return true;
		else
			return false;
	}
	else if(max_steps == 0) {
		return false;
	}

	int index_char;
	int index_letter;
	int length = strlen(start_word);
	char* new_word = new char[length + 1];
	strcpy(new_word, start_word);
	char change_ch;
	char prev_ch;

	for(int i = 0; i < (length * 26); i ++) {
		//determine index and skip if char at index equal to target
		index_char = i % length;
		if(start_word[index_char] == target_word[index_char])
			continue;

		index_letter = (i / length) % 26;
		change_ch = 'A' + index_letter;
		prev_ch = start_word[index_char];
		
		//skip if new char is equal to old char
		if(prev_ch == change_ch) 
			continue;
	 
		//put in char
		new_word[index_char] = change_ch;
		
		//check if word exists and call to itself
		if(dictionary_search(new_word)) {
			if(find_chain(new_word, target_word, answer_chain, max_steps - 1, depth + 1))
				return true;
		}
		new_word[index_char] = prev_ch;
	}
	delete new_word;
	answer_chain[depth] = NULL;
	return false;
}
