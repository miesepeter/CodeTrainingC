int encode_character(const char ch, char* multitap);
void encode(const char* plaintext, char* multitap);
void decode(std::istream& input, std::ostream& output);
char decode_part(char* code);
