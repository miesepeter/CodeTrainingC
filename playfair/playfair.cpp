#include <cctype>
#include <cstring>
#include "playfair.h"

using namespace std;

void prepare(const char* input, char* output) {
	int out_loc = 0;
	while(*input) {
		if(isalnum(*input)) {
			output[out_loc] = toupper(*input);
			out_loc ++;
		}
		input ++;
	}
	output[out_loc] = '\0';

	if(strlen(output) % 2 != 0) {
		output[out_loc] = 'X';
		output[out_loc + 1] = '\0';
	}
}


void grid(const char* codeword, char square[6][6]) {
	int sq_loc = 0;
	char sq_loc_al = 'A';
	char sq_loc_di = '0';

	while(*codeword) {
		if(occurs_before(square, *codeword, sq_loc)) {
			codeword ++;
			continue;
		}
		square[sq_loc / 6][sq_loc % 6] = *codeword;
		sq_loc ++;
		codeword ++;
	}
	
	while(sq_loc < 37) {
		if(sq_loc_al < ('Z' + 1)) {
			if(occurs_before(square, sq_loc_al, sq_loc)) {
				sq_loc_al ++;
				continue;
			}
			square[sq_loc / 6][sq_loc % 6] = sq_loc_al;
			sq_loc_al ++;
			sq_loc ++;
		}
		else {
			if(occurs_before(square, sq_loc_di, sq_loc)) {
				sq_loc_di ++;
				continue;
			}
			square[sq_loc / 6][sq_loc % 6] = sq_loc_di;
			sq_loc_di ++;
			sq_loc ++;
		}
	}
}


bool occurs_before(const char square[6][6], char ch, int length) {
	for(int i = 0; i < length; i++) {
		if(square[i / 6][i % 6] == ch)
			return true;
	}
	return false;
}


void bigram(const char square[6][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2) {
	int char1_row, char1_col, char2_row, char2_col;

	find_char(square, inchar1, char1_row, char1_col);
	find_char(square, inchar2, char2_row, char2_col);

	outchar1 = square[char1_row][char2_col];
	outchar2 = square[char2_row][char1_col];
}


void find_char(const char square[6][6], char ch, int& row, int& col) {
	for(int i = 0; i < 37; i++) {
		if(square[i / 6][i % 6] == ch) {
			row = (i / 6);
			col = (i % 6);
			return;
		}
	}
	row = -1;
	col = -1;
}


void encode(const char square[6][6], const char* prepared, char* encoded) {
	bigram(square, *prepared, prepared[1], encoded[0], encoded[1]);

	if(!prepared[2]) {
		encoded[2] = '\0';
		return;
	}
	encode(square, prepared + 2, encoded + 2);
}


void decode(const char square[6][6], const char* encoded, char* decoded) {	
	encode(square, encoded, decoded);
}
