#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include "multitap.h"

using namespace std;

int encode_character(const char ch, char* multitap) {

	if(isdigit(ch)) {
		multitap[0] = '*';
		multitap[1] = ch;
		multitap[2] = '\0';
	}

	else if(isalpha(ch)) {
		char upper = toupper(ch);
		int number, count;
		
		if(upper == 'Z') {
			number = 9;
			count = 4;
		}
		if(upper == 'S') {
			number = 7;
			count = 4;
		}
		else {
			number = ((upper - 'A') / 3) + 2;
			count = ((upper - 'A') % 3) + 1;
		}
		for(int i = 0; i < count; i++) {
			multitap[i] = number + '0';
			multitap[i + 1] = '\0';
		}
	}
	
	else {
		if(ch == '.') {
			char copy_1[] = "1";
			strcpy(multitap, copy_1);
		}
		if(ch == ',') {
			char copy_2[] = "11";
			strcpy(multitap, copy_2);
		}
		if(ch == '!') {
			char copy_3[] = "111";
			strcpy(multitap, copy_3);
		}
		if(ch == '?') {
			char copy_4[] = "1111";
			strcpy(multitap, copy_4);
		}
		if(ch == ' ') {
			char copy_5[] = "0";
			strcpy(multitap, copy_5);
		}
	}
	return strlen(multitap);
}


void encode(const char* plaintext, char* multitap) {
	
	multitap[0] = '\0';
	char temp[8];
	bool up = false;
	char previous = 'X';
	char copy[] = "#";

	while(*plaintext) {
		if(isupper(*plaintext)) {
			if(up == false) {
				up = true;
				strcat(multitap, copy);
			}

		}
		else if (up == true) {
			up = false;
			strcat(multitap, copy);
		}
		encode_character(*plaintext, temp);

		if(temp[0] == previous) {
			char copy[] = "|";
			strcat(multitap, copy);
		}

		strcat(multitap, temp);
	
		previous = temp[0];
		plaintext ++;
	}
}


void decode(istream& input, ostream& output) {
	char ch;
	bool up = false;
	bool skip = false;
	char temp[16];
	temp [0] = '\0';
	char next;
	char decoded = '\0';

	while(!input.eof()) {
		input.get(ch);
		if(skip) {
			skip = false;
			continue;
		}
		if(ch == '#') {
			up = (up) ? false : true;
			continue;
		}
		if(ch == '*') {
			decoded = input.peek();
			skip = true;
			output << decoded;
		}
		else if (isdigit(ch)) {
			next = input.peek();

				char copy[2];
			copy[0] = ch;
			copy[1] = '\0';
			strcat(temp, copy);

			if(ch != next) {	
				decoded = decode_part(temp);
				if(up == false)
					decoded = tolower(decoded);
				temp[0] = '\0';
				output << decoded;
			}
		}
	}
}


char decode_part(char* code) {
	int length = strlen(code);
	char return_ch;
	
	if(code[0] == '0')
		return_ch = ' ';

	else if(code[0] == '1') {
		if(length == 1)
			return_ch = '.';
		if(length == 2)
			return_ch = ',';
		if(length == 3)
			return_ch = '!';
		if(length == 4)
			return_ch = '?';
	}
	else {
		return_ch = (((code[0] - '0' - 2) * 3) + length) + 'A' - 1;

		if(code[0] == '8' || code[0] == '9')
			return_ch ++;
	}
	return return_ch;
}
