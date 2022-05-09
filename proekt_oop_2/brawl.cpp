#include <iostream>
#include <fstream>
#include "scheme.h"
#include "brawl.h"
#include "help.h"

Brawl::Brawl() {

	teams = nullptr;

	exitType = nullptr;

	matches = nullptr;

}

Brawl::~Brawl() {

	for (int i = 0; i < numTeams; ++i)
		delete[] teams[i];

	delete[] teams;

	delete[] exitType;

	delete[] matches;

}

void Brawl::setMatches(std::ifstream& reader, const int& type) {

	switch (type) {

	case 0:getDataTxt(reader); break;
	case 1:getDataCsv(reader); break;
	case 2:getDataJson(reader); break;

	}

	createMatches();

}

void Brawl::saveMatches() {

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

void Brawl::getDataTxt(std::ifstream& reader) {

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

void Brawl::getDataCsv(std::ifstream& reader) {

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

void Brawl::getDataJson(std::ifstream& reader) {

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

void Brawl::crop(char* input) {

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

void Brawl::createMatches() {

	matches = new Match[(numTeams * (numTeams - 1)) / 2];

	int* r1;
	int* r2;
	int lastWrit = 0;

	if (numTeams % 2 == 1) {

		r1 = new int[(numTeams + 1) / 2];
		r2 = new int[(numTeams + 1) / 2];

		for (int i = 0; i < numTeams + 1; ++i) {

			if (i == (numTeams + 1) / 2)
				lastWrit = 0;

			if (i < (numTeams + 1) / 2) {

				r1[lastWrit] = i;
				++lastWrit;

			}
			else {

				r2[lastWrit] = i;
				++lastWrit;

			}

		}

	}
	else {

		r1 = new int[numTeams / 2];
		r2 = new int[numTeams / 2];

		for (int i = 0; i < numTeams; ++i) {

			if (i == numTeams / 2)
				lastWrit = 0;

			if (i < numTeams / 2) {

				r1[lastWrit] = i;
				++lastWrit;

			}
			else {

				r2[lastWrit] = i;
				++lastWrit;

			}

		}

	}

	lastWrit = 0;

	int mult = 0;

	if (numTeams % 2 == 1) {

		for (int i = 1; i < numTeams + 1; ++i) {

			setRound(lastWrit, r1, r2, mult);

			if (numTeams > 2)
				shuffle(r1, r2);

			if (courts > 1)
				++mult;

		}

	}
	else {

		for (int i = 1; i < numTeams; ++i) {

			setRound(lastWrit, r1, r2, mult);

			if (numTeams > 2)
				shuffle(r1, r2);

			if (courts > 1)
				++mult;

		}

	}

	delete[] r1;
	delete[] r2;

}

void Brawl::setRound(int& startPos, const int* r1, const int* r2, int& mult) {

	if (numTeams % 2 == 1)
		oddRound(startPos, r1, r2, mult);
	else
		evenRound(startPos, r1, r2, mult);

}

void Brawl::shuffle(int* r1, int* r2) {

	int buff;

	if (numTeams % 2 == 1) {

		buff = r1[(numTeams + 1) / 2 - 1];

		for (int i = (numTeams + 1) / 2 - 1; i > 1; --i)
			r1[i] = r1[i - 1];

		r1[1] = r2[0];

		for (int i = 0; i < (numTeams + 1) / 2 - 1; ++i)
			r2[i] = r2[i + 1];

		r2[(numTeams + 1) / 2 - 1] = buff;

	}
	else {

		buff = r1[numTeams / 2 - 1];

		for (int i = numTeams / 2 - 1; i > 1; --i)
			r1[i] = r1[i - 1];

		r1[1] = r2[0];

		for (int i = 0; i < numTeams / 2 - 1; ++i)
			r2[i] = r2[i + 1];

		r2[numTeams / 2 - 1] = buff;

	}

}

void Brawl::oddRound(int& pos, const int* r1, const int* r2, int& mult) {

	int len;
	int counter = 1;

	for (int i = 0; i < (numTeams + 1) / 2; ++i) {

		if (r1[i] == numTeams || r2[i] == numTeams)
			continue;

		len = safeLen(teams[r1[i]]);
		matches[pos].t1 = new char[len];
		safeCpy(matches[pos].t1, teams[r1[i]]);

		len = safeLen(teams[r2[i]]);
		matches[pos].t2 = new char[len];
		safeCpy(matches[pos].t2, teams[r2[i]]);

		matches[pos].dat = setDate(mult);

		matches[pos].cour = counter;

		++pos;
		++counter;

		if (counter > courts && (numTeams - i - 1 > 1) && courts != 1) {

			counter = 1;
			++mult;

		}

		if (courts == 1) {

			counter = 1;
			++mult;

		}

	}

}

void Brawl::evenRound(int& pos, const int* r1, const int* r2, int& mult) {

	int len;
	int counter = 1;

	for (int i = 0; i < numTeams / 2; ++i) {

		len = safeLen(teams[r1[i]]);
		matches[pos].t1 = new char[len];
		safeCpy(matches[pos].t1, teams[r1[i]]);

		len = safeLen(teams[r2[i]]);
		matches[pos].t2 = new char[len];
		safeCpy(matches[pos].t2, teams[r2[i]]);

		matches[pos].dat = setDate(mult);

		matches[pos].cour = counter;

		++pos;
		++counter;

		if (counter > courts && (numTeams - i - 1 > 1) && courts != 1) {

			++mult;
			counter = 1;

		}

		if (courts == 1) {

			++mult;
			counter = 1;

		}

	}

}

char* Brawl::setDate(const int& addMult) {

	int add = addMult * lenght;

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

void Brawl::fixH(int& Hour, int& Day, int& Month, int& Year) {

	if (Hour > 23) {

		Hour = 0;
		++Day;
		fixD(Day, Month, Year);

	}

}

void Brawl::fixD(int& Day, int& Month, int& Year) {

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

void Brawl::fixM(int& Month, int& Year) {

	if (Month > 12) {

		Month = 1;
		++Year;

	}

}

bool Brawl::isLeap(const int& input) {

	if (input % 100 == 0 && !(input % 400 == 0))
		return false;

	if (input % 4 == 0)
		return true;

	return false;

}

void Brawl::push(const char& input, char* buff) {

	int len = safeLen(buff);

	for (int i = len; i > 0; --i)
		buff[i] = buff[i - 1];

	buff[0] = input;

}

char** Brawl::toWritTxt(int& rows) {

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

char** Brawl::toWritCsv(int& rows) {

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

char** Brawl::toWritJson(int& rows) {

	rows = numTeams * (numTeams - 1) / 2 * 3 + 2;

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

char* Brawl::extendTxt(const char* toExtend, const char* add) {

	int len1 = safeLen(toExtend);
	int len2 = safeLen(add);

	char* output = new char[len1 + len2 + 1];

	safeCpy(output, toExtend);
	output[len1 - 1] = '|';
	int lastRead = 0;

	for(int i=len1;i<len1+len2+1;++i) {
		
		if (i == len1 + len2 - 1) {

			output[i] = '|';
			++i;

		}

		output[i] = add[lastRead];
		++lastRead;

	}

	return output;

}

char* Brawl::merge(const char* input1, const char* input2) {

	int len1 = safeLen(input1);
	int len2 = safeLen(input2);

	char* output = new char[len1 + len2 + 3];

	int lastRead1 = 0, lastRead2 = 0;
	for (int i = 0; i < len1 + len2 + 3; ++i) {

		if (i < len1 -1) {

			output[i] = input1[lastRead1];
			++lastRead1;

		}

		if (i > len1 + 2) {

			output[i] = input2[lastRead2];
			++lastRead2;

		}

	}

	output[len1-1] = ' ';
	output[len1] = 'v';
	output[len1 + 1] = 's';
	output[len1 + 2] = ' ';
	output[len1 + len2 + 2] = '\0';

	return output;

}

char* Brawl::extendCsv(const char* toExtend, const char* add) {

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

char* Brawl::matchNameJson(const char* input1, const char* input2) {

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

char* Brawl::matchDateJson(const char* input) {

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

char* Brawl::matchCourtJson(const int& input) {

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

void Brawl::putInTxt(char** ans, const Match* curr) {

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

void Brawl::putInCsv(char** ans, const Match* curr) {

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

void Brawl::putInJson(char** ans, const Match* curr, int& lastWrit) {

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

