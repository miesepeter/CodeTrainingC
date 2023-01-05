#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

//---------------------------------------------------------------------------------------------
bool find_marker(const char ch, char** maze, const int height, const int width, int& row, int& col) {

	row = -1;
	col = -1;

	for(int col_loop = 0; col_loop < width; col_loop ++) {
		for(int row_loop = 0; row_loop < height; row_loop ++) {
			if(maze[row_loop][col_loop] == ch) {
				row = row_loop;
				col = col_loop;
				return true;
			}
		}
	}
	return false;	
}

bool valid_solution(const char* seq, char** maze, const int height, const int width) {
	const char* step = seq;
	char location = '0';
	int cur_row, cur_col;

	find_marker('>', maze, height, width, cur_row, cur_col);

	while(*step != '\0') {
		if(*step == 'N') {
			cur_row --;
		}
		if(*step == 'S') {
			cur_row ++;
		}
		if(*step == 'W') {
			cur_col --;
		}
		if(*step == 'E') {
			cur_col ++;
		}

		if(cur_row >= height || cur_row < 0)
			return false;

		if(cur_col >= width || cur_col < 0)
			return false;

		location = maze[cur_row][cur_col];

		if(location == '+' || location == '|' || location == '-')
			return false;

		step ++;	
	}
	if(location == 'X')
		return true;
	else
		return false;
}


bool create_path(char* path, char* entry, char end, char** maze, const int height, const int width, int start_row, int start_col) {

	if(maze[start_row][start_col] == end) {
		maze[start_row][start_col] = '#';
		*entry = '\0';
		return true;
	}

	maze[start_row][start_col] = '#';
	char location;

	if(start_col < (width - 1)) {
		location = maze[start_row][start_col + 1];
		if(location != '+' && location != '|' && location != '-' && location != '#') {
			*entry = 'E';
			entry ++;
			//*entry = '\0';
			if(create_path(path, entry, end, maze, height, width, start_row, start_col + 1))
				return true;
			entry --;	
			//*entry = '\0';
		}
	}
	
	
	if(start_col > 0) {
		location = maze[start_row][start_col - 1];
		if(location != '+' && location != '|' && location != '-' && location != '#') {
			*entry = 'W';
			entry ++;
			//*entry = '\0';
			if(create_path(path, entry, end, maze, height, width, start_row, start_col - 1))
				return true;
			entry --;	
			//*entry = '\0';
		}
	}


	if(start_row < (height - 1)) {
		location = maze[start_row + 1][start_col];
		if(location != '+' && location != '|' && location != '-' && location != '#') {
			*entry = 'S';
			entry ++;
			//*entry = '\0';
			if(create_path(path, entry, end, maze, height, width, start_row + 1, start_col))
				return true;	
			entry --;	
			//*entry = '\0';
		}
	}


	if(start_row > 0) {
		location = maze[start_row - 1][start_col];
		if(location != '+' && location != '|' && location != '-' && location != '#') {
			*entry = 'N';
			entry ++;
			//*entry = '\0';
			if(create_path(path, entry, end, maze, height, width, start_row - 1, start_col))
				return true;	
			entry --;	
			//*entry = '\0';
		}
	}
	maze[start_row][start_col] = ' ';
	*entry = '\0';
	return false;
}


const char* find_path(char** maze, const int height, const int width, const char start, const char end) {
	int start_row, start_col;

	find_marker(start, maze, height, width, start_row, start_col);
	
	char* path = new char[256];
	char* entry = path;
	path[0] = '\0';

	if(create_path(path, entry, end, maze, height, width, start_row, start_col)) {
		return path;
	}

	return "no solution";
}

