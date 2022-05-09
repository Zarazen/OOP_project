#ifndef _scheme_h_
#define _scheme_h_

class Scheme {

public:

	virtual ~Scheme();

	virtual void setMatches(std::ifstream& reader, const int& type) = 0;
	virtual void saveMatches() = 0;

	static Scheme* create(const char* type);

};

#endif // !_scheme_h_

