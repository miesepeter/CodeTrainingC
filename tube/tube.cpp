#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}


bool get_symbol_position(char** map, int height, int width, const char target, int& r, int& c) {
	
	for(int row = 0; row < height; row ++) {
		for(int col = 0; col < width; col ++) {
			if(map[row][col] == target) {
				r = row;
				c = col;
				return true;
			}
		}
	}
	r = -1;
	c = -1;
	return false;
}


char get_symbol_in_file(const char* name, const char* file) {
	ifstream ins;
	ins.open(file);

	char text[64];

	while(!ins.eof()) {
		ins.getline(text, 64);

		if(!strcmp(&text[2], name)) {
			ins.close();
			return text[0];	
		}
	}
	ins.close();
	return ' ';
}


char get_symbol_for_station_or_line(const char* name) {
	char return_ch;
	return_ch = get_symbol_in_file(name, "lines.txt");

	if(return_ch == ' ') {
		return_ch = get_symbol_in_file(name, "stations.txt");
	}
	return return_ch;
}


int validate_route(char** map, int height, int width, const char* start_station, char route[], char destination[]) {

	//check start valid
	char symbol_station = get_symbol_in_file(start_station, "stations.txt");
	if(symbol_station == ' ')
		return -1;
	
	//extract route
	char* route_ptr = route;
	char dir_ch[2];
	Direction dir;

	//line
	char previous_line = '?';
	char current_line = '?';
	int line_changes = 0;
	bool station;

	//current position
	int cur_row = 0;
	int cur_col = 0;
	int prev_row = 0;
	int prev_col = 0;
	int prev_prev_row =0;
	int prev_prev_col = 0;
	get_symbol_position(map, height, width, symbol_station, cur_row, cur_col);

	while(*route_ptr != '\0') {

		//extract direction
		if(*(route_ptr + 1) == ',') {
			dir_ch[0] = *route_ptr;
			dir_ch[1] = '\0';
			dir = string_to_direction(dir_ch);
			
			if(*(route_ptr + 1) == '\0') {
				route_ptr ++;
			}
			else {
				route_ptr += 2;
			}
		}
		else {
			dir_ch[0] = *route_ptr;
			dir_ch[1] = *(route_ptr + 1);
			dir_ch[2] = '\0';
			dir = string_to_direction(dir_ch);

			if(*(route_ptr + 1) == '\0') {
				route_ptr ++;
			}
			else {
				route_ptr += 3;
			}
		}
		if(dir == INVALID_DIRECTION)
			return -5;
	
		//update to new position
		prev_prev_row = prev_row;
		prev_prev_col = prev_col;
		prev_row = cur_row;
		prev_col = cur_col;
		new_position(dir, cur_row, cur_col);

		//cout << endl << prev_row << " " << prev_col << " " << cur_row << " " << cur_col << endl;
		
		//check for backtracking
		if(cur_row == prev_prev_row && cur_col == prev_prev_col && station == false)
			return -4;

		//check out of bounce of off track
		if(cur_row < 0 || cur_row > height || cur_col < 0 || cur_col > width)
			return 	-7;

		if(map[cur_row][cur_col] == ' ')
			return -6;
	

		//check position and update line	
		if(!isalnum(map[cur_row][cur_col])) {
			previous_line = current_line;
			current_line = map[cur_row][cur_col];

			//set previous line to current line at start
			if(previous_line == '?')
				previous_line = current_line;
			
			//update line count
			if(current_line != previous_line) {
				if(station == true)
					line_changes ++;
				else
					return -3;
			}
			station = false;	
		}
		else {
			station = true;
		}
	}
	char dest = map[cur_row][cur_col];

	if(isalnum(dest)) {
		ifstream ins;
		ins.open("stations.txt");
		char text[64];

		while(!ins.eof()) {
			ins.getline(text, 64);
			if(text[0] == dest)
				strcpy(destination, &text[2]);
		}
		ins.close();
		return line_changes;
	}
	else {
		return -2;
	}
}


void new_position(Direction dir, int& row, int& col) {
	if(dir == N) {
		row --;
	}
	else if(dir == S) {
		row ++;
	}
	else if(dir == W) {
		col --;
	}
	else if(dir == E) {
		col ++;
	}
	else if(dir == NE) {
		row --;
		col ++;
	}
	else if(dir == NW) {
		row --;
		col --;
	}
	else if(dir == SE) {
		row ++;
		col ++;
	}
	else if(dir == SW) {
		row ++;
		col --;
	}
}
