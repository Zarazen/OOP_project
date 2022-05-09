#ifndef _elimination_h_
#define _elimination_h_

class Scheme;

class Elimination : public Scheme {

public:

	Elimination();
	~Elimination();

	void setMatches(std::ifstream& reader, const int& type);
	void saveMatches();

private:

	void getDataTxt(std::ifstream& reader);
	void getDataCsv(std::ifstream& reader);
	void getDataJson(std::ifstream& reader);
	void crop(char* input);
	char** setRound(char** contenders, int& pos, int& numContenders, int& mult);
	int decideWin();
	void addMatch(const int& numMatch);
	void preRound(char** contenders, int& numContenders);
	char* setDate(const int& mult);
	void fixH(int& Hour, int& Day, int& Month, int& Year);
	void fixD(int& Day, int& Month, int& Year);
	void fixM(int& Month, int& Year);
	bool isLeap(const int& input);
	void push(const char& input, char* buff);
	char** toWritTxt(int& rows);
	char** toWritCsv(int& rows);
	char** toWritJson(int& rows);
	char* extendTxt(const char* toExtend, const char* add);
	char* merge(const char* input1, const char* input2);
	char* extendCsv(const char* toExtend, const char* add);
	char* matchNameJson(const char* input1, const char* input2);
	char* matchDateJson(const char* input);
	char* matchCourtJson(const int& input);

	struct Match {

		Match() {

			t1 = nullptr;
			t2 = nullptr;
			dat = nullptr;

		}

		~Match() {

			delete[] t1;
			delete[] t2;
			delete[] dat;

		}

		char* t1;
		char* t2;
		char* dat;
		int cour;

	};

	Match* beg() {

		return matches;

	}

	Match* end() {

		return &matches[numMatch];

	}

	void putInTxt(char** ans, const Match* curr);
	void putInCsv(char** ans, const Match* curr);
	void putInJson(char** ans, const Match* curr, int& lastWrit);

	class Iterator {

	public:

		Iterator(Match* input) {

			curr = input;

		}

		Iterator& operator++() {

			++curr;

			return *this;

		}

		bool operator!=(const Match* input) {

			if (curr == input)
				return false;

			return true;

		}

		Match* curr;

	};

	Match* matches;
	int numTeams;
	char** teams;
	int lenght;
	int courts;
	int hour;
	int day;
	int month;
	int year;
	char* exitType;
	int numMatch;

};

#endif // !_elimination_h_

