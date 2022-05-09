#include <iostream>
#include <fstream>
#include "scheme.h"
#include "elimination.h"
#include "brawl.h"

Scheme::~Scheme() {



}

Scheme* Scheme::create(const char* type) {

	if (!std::strcmp(type, "elimination"))
		return new Elimination;

	if (!std::strcmp(type, "brawl"))
		return new Brawl;

	return nullptr;

}
