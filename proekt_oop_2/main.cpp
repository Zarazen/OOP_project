#include <iostream>
#include <fstream>
#include "help.h"
#include "scheme.h"

int main() {

	/// INPUT FILE IS EITHER ONE OF
	/// test.txt
	/// text.csv
	/// test.json
	/// files can be modified in proekt_oop_2 folder

	char path[64];
	std::cin >> path;
	int type = typeEntry(path);

	std::ifstream getData;

	getData.open(path);

	if (getData.is_open() == false)
		return 0;

	char schemeType[16];
	getSchemeType(getData, schemeType, type);

	Scheme* theScheme = Scheme::create(schemeType);
	theScheme->setMatches(getData, type);

	getData.close();

	theScheme->saveMatches();
	delete theScheme;

	return 0;

}