
void prepare(const char* input, char* output);
void grid(const char* codeword, char square[6][6]);
bool occurs_before(const char word[6][6], char ch, int length);
void bigram(const char square[6][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2);
void find_char(const char square[6][6], char ch, int& row, int& col);
void encode(const char square[6][6], const char* prepared, char* encoded);
void decode(const char square[6][6], const char* encoded, char* decoded);	
