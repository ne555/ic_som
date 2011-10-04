#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <cstdio>
#include "som.h"

class simulator{
public:
	typedef SOM::value_type value_type;
	typedef SOM::vector vector;

	typedef std::vector<vector> matrix;

	simulator(size_t percepciones, size_t salidas, size_t row, size_t column, FILE *out=NULL);
	void read(std::istream &in);

	value_type test();
	void train();

private:
	matrix input, result; //, prev_error, actual_error;
	SOM red;

	size_t percepciones, salidas;

	FILE *out;
};

#endif

