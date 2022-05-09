#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "scheme.h"
#include "elimination.h"
#include "help.h"

Elimination::Elimination() {

	matches = nullptr;

	teams = nullptr;

	exitType = nullptr;

}

Elimination::~Elimination() {

	delete[] matches;

	for (int i = 0; i < numTeams; ++i)
		delete[] teams[i];
	delete[] teams;

	delete[] exitType;

}

void Elimination::setMatches(std::ifstream& reader, const int& type) {

	int mult = 0;

	switch (type) {

	case 0:getDataTxt(reader); break;
	case 1:getDataCsv(reader); break;
	case 2:getDataJson(reader); break;

	}

	int numContenders = numTeams;
	char** contenders = new char*[numContenders];
	numMatch = 0;

	preRound(contenders, numContenders);

	while (numContenders > 1) {

		contenders = setRound(contenders, numMatch, numContenders, mult);

		if (courts > 1)
			++mult;

	}

	for (int i = 0; i < numContenders; ++i)
		delete[] contenders[i];
	delete[] contenders;
	
}

void Elimination::saveMatches() {

	char** toWrit = nullptr;
	int rows = 0;

	if (!std::strcmp(exitType, "txt"))
		toWrit = toWritTxt(rows);

	if (!std::strcmp(exitType, "csv"))
		toWrit = toWritCsv(rows);

	if (!std::strcmp(exitType, "json"))
		toWrit = toWritJson(rows);

	char output1[] = "output.txt";
	char output2[] = "output.csv";
	char output3[] = "output.json";

	std::ofstream write;

	if (!std::strcmp(exitType, "txt"))
		write.open(output1);

	if (!std::strcmp(exitType, "csv"))
		write.open(output2);

	if (!std::strcmp(exitType, "json"))
		write.open(output3);

	if (write.is_open() == false) {

		for (int i = 0; i < rows; ++i)
			delete[] toWrit[i];

		delete[] toWrit;

		return;

	}

	for (int i = 0; i < rows; ++i)
		write << toWrit[i] << std::endl;

	write.close();

	for (int i = 0; i < rows; ++i)
		delete[] toWrit[i];

	delete[] toWrit;

}

void Elimination::getDataTxt(std::ifstream& reader) {

	char buff[32];

	reader >> numTeams;

	teams = new char* [numTeams];
	for (int i = 0; i < numTeams; ++i) {

		reader >> buff;
		buff[std::strlen(buff)] = '\0';
		teams[i] = new char[safeLen(buff)];
		safeCpy(teams[i], buff);

	}

	reader >> lenght;
	reader >> courts;
	reader >> hour;
	reader >> day;
	reader >> month;
	reader >> year;

	reader >> buff;
	buff[std::strlen(buff)] = '\0';
	exitType = new char[safeLen(buff)];
	safeCpy(exitType, buff);

}

void Elimination::getDataCsv(std::ifstream& reader) {

	char buff[32];

	reader >> numTeams;

	teams = new char* [numTeams];
	for (int i = 0; i < numTeams; ++i) {

		reader >> buff;
		buff[std::strlen(buff)] = '\0';
		teams[i] = new char[safeLen(buff)];
		safeCpy(teams[i], buff);

	}

	reader >> lenght;
	reader >> courts;
	reader >> hour;
	reader >> day;
	reader >> month;
	reader >> year;

	reader >> buff;
	buff[std::strlen(buff)] = '\0';
	exitType = new char[safeLen(buff)];
	safeCpy(exitType, buff);

}

void Elimination::getDataJson(std::ifstream& reader) {

	char buff[64];
	int len;

	reader >> buff;
	crop(buff);
	if (buff[1] == '\0')
		numTeams = buff[0] - 48;
	else
		numTeams = (buff[1] - '0') + (buff[0] - '0') * 10;

	teams = new char* [numTeams];

	for (int i = 0; i < numTeams; ++i) {

		reader >> buff;
		crop(buff);
		teams[i] = new char[safeLen(buff)];
		safeCpy(teams[i], buff);

	}

	reader >> buff;
	crop(buff);
	len = safeLen(buff) - 1;
	lenght = 0;
	for (int i = 0; i < len; ++i)
		lenght += (buff[i] - '0') * power(10, len - i);

	reader >> buff;
	crop(buff);
	len = safeLen(buff) - 1;
	courts = 0;
	for (int i = 0; i < len; ++i)
		courts += (buff[i] - '0') * power(10, len - i);

	reader >> buff;
	crop(buff);
	len = safeLen(buff) - 1;
	hour = 0;
	for (int i = 0; i < len; ++i)
		hour += (buff[i] - '0') * power(10, len - i);

	reader >> buff;
	crop(buff);
	len = safeLen(buff) - 1;
	day = 0;
	for (int i = 0; i < len; ++i)
		day += (buff[i] - '0') * power(10, len - i);

	reader >> buff;
	crop(buff);
	len = safeLen(buff) - 1;
	month = 0;
	for (int i = 0; i < len; ++i)
		month += (buff[i] - '0') * power(10, len - i);

	reader >> buff;
	crop(buff);
	len = safeLen(buff) - 1;
	year = 0;
	for (int i = 0; i < len; ++i)
		year += (buff[i] - '0') * power(10, len - i);

	reader >> buff;
	crop(buff);
	exitType = new char[safeLen(buff)];
	safeCpy(exitType, buff);

}

void Elimination::crop(char* input) {

	char output[64];
	int lastWrit = 0;
	bool flag = false;

	for (int i = 0;; ++i) {

		if (flag && (input[i] == '"' || input[i] == ','))
			break;

		if (input[i] == ':') {

			flag = true;

			if (input[i + 1] == '"')
				++i;

			continue;

		}

		if (flag) {

			output[lastWrit] = input[i];
			++lastWrit;

		}

	}

	output[lastWrit] = '\0';
	safeCpy(input, output);

}

char** Elimination::setRound(char** contenders, int& pos, int& numContenders, int& mult) {

	char** newContenders;
	int lastWrit = 0;
	bool flag = false;
	int counter = 1;

	if (numContenders % 2 == 1) {

		newContenders = new char* [(numContenders + 1) / 2];

		newContenders[lastWrit] = new char[safeLen(contenders[numContenders - 1])];
		safeCpy(newContenders[lastWrit], contenders[numContenders - 1]);
		++lastWrit;
		--numContenders;
		flag = true;

	}
	else
		newContenders = new char* [numContenders / 2];

	for (int i = 0; i < numContenders; i = i + 2) {

		addMatch(pos);

		matches[pos].t1 = new char[safeLen(contenders[i])];
		safeCpy(matches[pos].t1, contenders[i]);

		matches[pos].t2 = new char[safeLen(contenders[i + 1])];
		safeCpy(matches[pos].t2, contenders[i + 1]);

		matches[pos].dat = setDate(mult);

		matches[pos].cour = counter;

		++pos;
		++counter;

		if (counter > courts && (numContenders - i - 1 > 1) && courts != 1) {

			counter = 1;
			++mult;

		}

		if (courts == 1) {

			counter = 1;
			++mult;

		}

		if (decideWin() == 0) {

			newContenders[lastWrit] = new char[safeLen(contenders[i])];
			safeCpy(newContenders[lastWrit], contenders[i]);
			++lastWrit;

		}
		else {

			newContenders[lastWrit] = new char[safeLen(contenders[i + 1])];
			safeCpy(newContenders[lastWrit], contenders[i + 1]);
			++lastWrit;

		}

	}

	if (flag) {

		for (int i = 0; i < numContenders + 1; ++i)
			delete[] contenders[i];
		delete[] contenders;

	}
	else {

		for (int i = 0; i < numContenders; ++i)
			delete[] contenders[i];
		delete[] contenders;

	}

	numContenders = lastWrit;

	return newContenders;

}

int Elimination::decideWin() {

	srand(time(NULL));

	return rand() % 2;

}

void Elimination::addMatch(const int& numMatch) {

	Match* newMatches = new Match[numMatch + 1];

	for (int i = 0; i < numMatch; ++i) {

		newMatches[i].t1 = new char[safeLen(matches[i].t1)];
		safeCpy(newMatches[i].t1, matches[i].t1);

		newMatches[i].t2 = new char[safeLen(matches[i].t2)];
		safeCpy(newMatches[i].t2, matches[i].t2);

		newMatches[i].dat = new char[safeLen(matches[i].dat)];
		safeCpy(newMatches[i].dat, matches[i].dat);

		newMatches[i].cour = matches[i].cour;

	}

	delete[] matches;
	matches = newMatches;

}

void Elimination::preRound(char** contenders, int& numContenders) {

	for (int i = 0; i < numContenders; ++i) {

		contenders[i] = new char[safeLen(teams[i])];
		safeCpy(contenders[i], teams[i]);

	}

}

char* Elimination::setDate(const int& mult) {

	int add = mult * lenght;

	int ansH = hour;
	int ansD = day;
	int ansM = month;
	int ansY = year;

	while (add > 0) {

		++ansH;
		fixH(ansH, ansD, ansM, ansY);
		--add;

	}

	char buff[128];
	buff[0] = '\0';
	char buf;

	for (int i = 2;; ++i) {

		if (ansH == 0)
			break;

		buf = '0' + (ansH % 10);
		push(buf, buff);
		ansH -= ansH % 10;
		ansH /= 10;

	}

	push('-', buff);

	for (int i = 2;; ++i) {

		if (ansD == 0)
			break;

		buf = '0' + (ansD % 10);
		push(buf, buff);
		ansD -= ansD % 10;
		ansD /= 10;

	}

	push('-', buff);

	for (int i = 2;; ++i) {

		if (ansM == 0)
			break;

		buf = '0' + (ansM % 10);
		push(buf, buff);
		ansM -= ansM % 10;
		ansM /= 10;

	}

	push('-', buff);

	for (int i = 2;; ++i) {

		if (ansY == 0)
			break;

		buf = '0' + (ansY % 10);
		push(buf, buff);
		ansY -= ansY % 10;
		ansY /= 10;

	}

	char* answer = new char[safeLen(buff)];
	safeCpy(answer, buff);

	return answer;

}

void Elimination::fixH(int& Hour, int& Day, int& Month, int& Year) {

	if (Hour > 23) {

		Hour = 0;
		++Day;
		fixD(Day, Month, Year);

	}

}

void Elimination::fixD(int& Day, int& Month, int& Year) {

	if (Month == 2) {

		if (isLeap(Year) && Day > 29) {

			Day = 1;
			++Month;
			return;

		}

		if (!isLeap(Year) && Day > 28) {

			Day = 1;
			++Month;
			return;

		}

	}

	if (((Month % 2 == 1 && Month < 8) || (Month % 2 == 0 && Month >= 8)) && Day > 31) {

		Day = 1;
		++Month;
		fixM(Month, Year);
		return;

	}

	if ((Month % 2 == 0 && Month < 8) || (Month % 2 == 1 && Month > 8) && Day > 30) {

		Day = 1;
		++Month;
		return;

	}

}

void Elimination::fixM(int& Month, int& Year) {

	if (Month > 12) {

		Month = 1;
		++Year;

	}

}

bool Elimination::isLeap(const int& input) {

	if (input % 100 == 0 && !(input % 400 == 0))
		return false;

	if (input % 4 == 0)
		return true;

	return false;

}

void Elimination::push(const char& input, char* buff) {

	int len = safeLen(buff);

	for (int i = len; i > 0; --i)
		buff[i] = buff[i - 1];

	buff[0] = input;

}

char** Elimination::toWritTxt(int& rows) {

	const char* courstStr = "court";
	char* input;
	char buff[16];
	int copy;

	rows = courts + 1;

	char** ans = new char* [rows];
	for (int i = 0; i < rows; ++i) {

		if (i == 0) {

			ans[i] = new char[1];
			ans[i][0] = '\0';
			continue;

		}

		copy = i;
		for (int j = 0;; ++j) {

			buff[j] = (copy % 10) + '0';
			copy /= 10;

			if (copy == 0) {

				buff[j + 1] = '\0';
				break;

			}

		}

		int len1 = std::strlen(courstStr);
		int len2 = safeLen(buff);
		input = new char[len1 + len2];
		safeCpy(input, courstStr);
		for (int j = len1, lastRead = len2 - 2; lastRead >= 0; --lastRead, ++j)
			input[j] = buff[lastRead];
		input[len1 + len2 - 1] = '\0';

		ans[i] = new char[safeLen(input)];
		safeCpy(ans[i], input);

	}

	for (Iterator it = beg(); it != end(); ++it)
		putInTxt(ans, it.curr);

	return ans;

}

char** Elimination::toWritCsv(int& rows) {

	const char* courstStr = "court";
	char* input;
	char buff[16];
	int copy;

	rows = courts + 1;

	char** ans = new char* [rows];
	for (int i = 0; i < rows; ++i) {

		if (i == 0) {

			ans[i] = new char[1];
			ans[i][0] = '\0';
			continue;

		}

		copy = i;
		for (int j = 0;; ++j) {

			buff[j] = (copy % 10) + '0';
			copy /= 10;

			if (copy == 0) {

				buff[j + 1] = '\0';
				break;

			}

		}

		int len1 = std::strlen(courstStr);
		int len2 = safeLen(buff);
		input = new char[len1 + len2];
		safeCpy(input, courstStr);
		for (int j = len1, lastRead = len2 - 2; lastRead >= 0; --lastRead, ++j)
			input[j] = buff[lastRead];
		input[len1 + len2 - 1] = '\0';

		ans[i] = new char[safeLen(input)];
		safeCpy(ans[i], input);

	}

	for (Iterator it = beg(); it != end(); ++it)
		putInCsv(ans, it.curr);

	return ans;

}

char** Elimination::toWritJson(int& rows) {

	rows = numMatch * 3 + 2;

	char** ans = new char* [rows];
	ans[0] = new char[2];
	ans[0][0] = '{';
	ans[0][1] = '\0';
	ans[rows - 1] = new char[2];
	ans[rows - 1][0] = '}';
	ans[rows - 1][1] = '\0';

	int position = 1;
	int numMatch = 1;

	for (Iterator it = beg(); it != end(); ++it, ++numMatch)
		putInJson(ans, it.curr, position);

	return ans;

}

char* Elimination::extendTxt(const char* toExtend, const char* add) {

	int len1 = safeLen(toExtend);
	int len2 = safeLen(add);

	char* output = new char[len1 + len2 + 1];

	safeCpy(output, toExtend);
	output[len1 - 1] = '|';
	int lastRead = 0;

	for (int i = len1; i < len1 + len2 + 1; ++i) {

		if (i == len1 + len2 - 1) {

			output[i] = '|';
			++i;

		}

		output[i] = add[lastRead];
		++lastRead;

	}

	return output;

}

char* Elimination::merge(const char* input1, const char* input2) {

	int len1 = safeLen(input1);
	int len2 = safeLen(input2);

	char* output = new char[len1 + len2 + 3];

	int lastRead1 = 0, lastRead2 = 0;
	for (int i = 0; i < len1 + len2 + 3; ++i) {

		if (i < len1 - 1) {

			output[i] = input1[lastRead1];
			++lastRead1;

		}

		if (i > len1 + 2) {

			output[i] = input2[lastRead2];
			++lastRead2;

		}

	}

	output[len1 - 1] = ' ';
	output[len1] = 'v';
	output[len1 + 1] = 's';
	output[len1 + 2] = ' ';
	output[len1 + len2 + 2] = '\0';

	return output;

}

char* Elimination::extendCsv(const char* toExtend, const char* add) {

	int len1 = safeLen(toExtend);
	int len2 = safeLen(add);

	char* output = new char[len1 + len2];

	safeCpy(output, toExtend);
	output[len1 - 1] = ',';

	int lastRead = 0;
	for (int i = len1; i < len1 + len2; ++i) {

		output[i] = add[lastRead];
		++lastRead;

	}

	return output;

}

char* Elimination::matchNameJson(const char* input1, const char* input2) {

	const char* matchStr = "match";

	char* matchName = merge(input1, input2);
	int len1 = std::strlen(matchStr);
	int len2 = safeLen(matchName);
	char* output = new char[len1 + 6 + len2];
	int lastRead = 0;

	output[0] = '"';
	output[len1 + 1] = '"';
	output[len1 + 2] = ':';
	output[len1 + 3] = '"';
	output[len1 + len2 + 3] = '"';
	output[len1 + len2 + 4] = ',';
	output[len1 + len2 + 5] = '\0';

	for (int i = 1;; ++i) {

		if (i < 1 + len1) {

			output[i] = matchStr[lastRead];
			++lastRead;

		}

		if (i > len1 + 3) {

			output[i] = matchName[lastRead];
			++lastRead;

			if (matchName[lastRead] == '\0')
				break;

		}

		if (i == 1 + len1)
			lastRead = 0;

	}

	delete[] matchName;

	return output;

}

char* Elimination::matchDateJson(const char* input) {

	const char* dateStr = "date";

	int len1 = std::strlen(dateStr);
	int len2 = safeLen(input);
	char* output = new char[len1 + 6 + len2];

	output[0] = '"';
	output[len1 + 1] = '"';
	output[len1 + 2] = ':';
	output[len1 + 3] = '"';
	output[len1 + len2 + 3] = '"';
	output[len1 + len2 + 4] = ',';
	output[len1 + len2 + 5] = '\0';

	int lastRead = 0;
	for (int i = 1;; ++i) {

		if (i < 1 + len1) {

			output[i] = dateStr[lastRead];
			++lastRead;

		}

		if (i > len1 + 3) {

			output[i] = input[lastRead];
			++lastRead;

			if (input[lastRead] == '\0')
				break;

		}

		if (i == 1 + len1)
			lastRead = 0;

	}

	return output;

}

char* Elimination::matchCourtJson(const int& input) {

	const char* courtStr = "court";

	char buff[16];
	int copy = input;
	int len1 = std::strlen(courtStr);
	for (int i = 0;; ++i) {

		buff[i] = (copy % 10) + '0';
		copy /= 10;

		if (copy == 0) {

			buff[i + 1] = '\0';
			break;

		}

	}
	int len2 = std::strlen(buff);

	char* output = new char[len1 + len2 + 5];

	output[0] = '"';
	output[len1 + 1] = '"';
	output[len1 + 2] = ':';
	output[len1 + len2 + 3] = ',';
	output[len1 + len2 + 4] = '\0';

	int lastRead = 0;
	for (int i = 1; i < 1 + len1; ++i, ++lastRead)
		output[i] = courtStr[lastRead];

	lastRead = len2 - 1;
	for (int i = len1 + 3; i < len1 + len2 + 3; ++i, --lastRead)
		output[i] = lastRead[buff];

	return output;

}

void Elimination::putInTxt(char** ans, const Match* curr) {

	char* buff = nullptr;

	if (curr->cour == 1) {

		buff = extendTxt(ans[0], curr->dat);
		delete[] ans[0];
		ans[0] = new char[safeLen(buff)];
		safeCpy(ans[0], buff);
		delete[] buff;

	}

	char* matchName = merge(curr->t1, curr->t2);

	buff = extendTxt(ans[curr->cour], matchName);
	delete[] ans[curr->cour];
	ans[curr->cour] = new char[safeLen(buff)];
	safeCpy(ans[curr->cour], buff);
	delete[] buff;

	delete[] matchName;

}

void Elimination::putInCsv(char** ans, const Match* curr) {

	char* buff;

	if (curr->cour == 1) {

		buff = extendCsv(ans[0], curr->dat);
		delete[] ans[0];
		ans[0] = new char[safeLen(buff)];
		safeCpy(ans[0], buff);
		delete[] buff;

	}

	char* matchName = merge(curr->t1, curr->t2);

	buff = extendCsv(ans[curr->cour], matchName);
	delete[] ans[curr->cour];
	ans[curr->cour] = new char[safeLen(buff)];
	safeCpy(ans[curr->cour], buff);
	delete[] buff;

	delete[] matchName;

}

void Elimination::putInJson(char** ans, const Match* curr, int& lastWrit) {

	char* matchName = matchNameJson(curr->t1, curr->t2);
	ans[lastWrit] = new char[safeLen(matchName)];
	safeCpy(ans[lastWrit], matchName);
	++lastWrit;
	delete[] matchName;

	char* matchDate = matchDateJson(curr->dat);
	ans[lastWrit] = new char[safeLen(matchDate)];
	safeCpy(ans[lastWrit], matchDate);
	++lastWrit;
	delete[] matchDate;

	char* matchCourt = matchCourtJson(curr->cour);
	ans[lastWrit] = new char[safeLen(matchCourt)];
	safeCpy(ans[lastWrit], matchCourt);
	++lastWrit;
	delete[] matchCourt;

}


