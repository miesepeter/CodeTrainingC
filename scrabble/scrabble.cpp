#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */

int tile_score(const char tile) {
	char ch = toupper(tile);

	//return if space
	if(ch == ' ' || ch == '?') {
		return 0;
	} 
	
	//return if letter
	if(isalpha(ch)) {
		if(ch == 'D' || ch == 'G')
			return 2;
		if(ch == 'B' || ch == 'C' || ch == 'M' || ch == 'P')
			return 3;
		if(ch == 'F' || ch == 'H' || ch == 'V' || ch == 'W' || ch == 'Y')
			return 4;
		if(ch == 'K')
			return 5;
		if(ch == 'J' || ch == 'X')
			return 8;
		if(ch == 'Q' || ch == 'Z')
			return 10;

		//default if no special letter
		return 1;
	}
	
	//default if non-defined
	return -1;
}


bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles, int insert) {

	//place sentinel for char counting
	if(insert == 0) {
		*played_tiles = '\0';
	}

	//ending condition - played_tiles sentinel at same position as in word
	if(strlen(word) == strlen(played_tiles))
		return true;


	//count available tiles to place specific character
	char placement = word[insert];
	int free_chars = count_char(tiles, placement) - count_char(played_tiles, placement);
	int free_space = count_char(tiles, ' ') - count_char(played_tiles, ' ');
	int free_question = count_char(tiles, '?') - count_char(played_tiles, '?');
	int free_blank = free_space + free_question;

	//make move with char if available
	if(free_chars > 0) {
		played_tiles[insert] = placement;
		played_tiles[insert + 1] = '\0';
		return can_form_word_from_tiles(word, tiles, played_tiles, insert + 1);
	}

	//make move with blank if available
	else if(free_blank > 0) {
		played_tiles[insert] = '?';
		played_tiles[insert + 1] = '\0';
		return can_form_word_from_tiles(word, tiles, played_tiles, insert + 1);
	}
	else {
		return false;
	}
}


int count_char(const char* word, char ch) {
	int count = 0;
	while(*word) {
		if(*word == ch)
			count ++;
		word ++;
	}
	return count;
}

int compute_score(const char* played_tiles, const ScoreModifier* score_modifiers) {
	int total_score = 0, field_score, index = 0, double_word = 0, triple_word = 0;

	while(played_tiles[index]) {
		field_score = tile_score(played_tiles[index]);
		
		//increase tile score by letter multiplier
		if(score_modifiers[index] == DOUBLE_LETTER_SCORE) 
			field_score *= 2;
		else if(score_modifiers[index] == TRIPLE_LETTER_SCORE) 
			field_score *= 3;
		
		//count word multiplier
		else if(score_modifiers[index] == DOUBLE_WORD_SCORE) 
			double_word ++;
		else if(score_modifiers[index] == TRIPLE_WORD_SCORE) 
			triple_word ++;
		
		total_score += field_score;
		index ++;
	}
	//increase word score by multipliers
	if(double_word > 0)
		total_score *= double_word * 2;
	if(triple_word > 0)
		total_score *= triple_word * 3;

	//add bonus points
	if(strlen(played_tiles) == 7)
		total_score += 50;

	return total_score;
}


int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* score_modifiers, char* word) {
	ifstream in;
	in.open("words.txt");
	char input[64];

	char temp_word[8];
	char final_word[8];

	int temp_score = -1;
	int final_score = -1;

	while(!in.eof()) {
		in.getline(input, 64);

		//get next word if too large
		if(strlen(input) > 7)
			continue;
		
		if(can_form_word_from_tiles(input, tiles, temp_word)) {
			temp_score = compute_score(temp_word, score_modifiers); 

			if(temp_score > final_score) {
				final_score = temp_score;
				strcpy(final_word, temp_word);
			}
		}
	}
	in.close();
	strcpy(word, final_word);
	return final_score;
}
