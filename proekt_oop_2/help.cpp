#include <iostream>
#include <fstream>
#include "help.h"

void safeCpy(char* to, const char* from) {

	for (int i = 0;; ++i) {

		to[i] = from[i];

		if (from[i] == '\0')
			return;

	}

}

int safeLen(const char* input) {

	if (input == nullptr)
		return 1;

	for (int i = 0;; ++i) {

		if (input[i] == '\0')
			return i + 1;

	}

}

int typeEntry(const char* input) {

	if (input[std::strlen(input) - 1] == 't')
		return 0;

	if (input[std::strlen(input) - 1] == 'v')
		return 1;

	if (input[std::strlen(input) - 1] == 'n')
		return 2;

}

void getSchemeType(std::ifstream& reader, char* output, const int& fileType) {

	switch (fileType) {

	case 0:getFromTxt(reader, output); break;
	case 1:getFromCsv(reader, output); break;
	case 2:getFromJson(reader, output); break;

	}

}

void getFromTxt(std::ifstream& reader, char* output) {

	reader >> output;

}

void getFromCsv(std::ifstream& reader, char* output) {

	reader >> output;

}

void getFromJson(std::ifstream& reader, char* output) {

	char buff[32];
	reader >> buff;
	reader >> buff;

	int lastWrit = 0;
	bool flag = false;

	for (int i = 0;; ++i) {

		if (flag && buff[i] == '"')
			break;

		if (buff[i] == ':') {

			flag = true;
			++i;
			continue;

		}

		if (flag) {

			output[lastWrit] = buff[i];
			++lastWrit;
			output[lastWrit] = '\0';

		}

	}

}

int power(const int& base, const int& pow) {

	int answer = 1;

	for (int i = 1; i < pow; ++i)
		answer *= 10;

	return answer;

}

bool safeCmp(const char* i1, const char* i2) {

	for (int i = 0;; ++i) {

		if (i1[i] != i2[i])
			return false;

		if (i1[i] == '\0' || i2[i] == '\0')
			break;

	}

	return true;

}
