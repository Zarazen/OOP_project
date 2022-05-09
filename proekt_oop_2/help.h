#ifndef _help_h_
#define _help_h_

void safeCpy(char* to, const char* from);

int safeLen(const char* input);

int typeEntry(const char* input);

void getSchemeType(std::ifstream& reader, char* output, const int& fileType);

void getFromTxt(std::ifstream& reader, char* output);

void getFromCsv(std::ifstream& reader, char* output);

void getFromJson(std::ifstream& reader, char* output);

int power(const int& base, const int& pow);

bool safeCmp(const char* i1, const char* i2);

#endif // !_help_h_

