#include <iostream>
#include <sstream>
#include <cstdio>
#include <valarray>
#include "simulator.h"
#include "util.h"
#include "math_vector.h"

using namespace std;
using math::sign;
using math::sigmoid;

simulator::simulator(size_t percepciones, size_t salidas, size_t row, size_t column, FILE *out):
	percepciones(percepciones),
	salidas(salidas),
	red(percepciones, row, column),
	out(out){}

void simulator::read(istream &in){
	size_t patrones;
	in >> patrones;
	//input.resize(patrones, vector(percepciones+1));
	input.resize(patrones, vector(percepciones));
	result.resize(patrones, vector(salidas+1) );

	for(size_t K=0; K<patrones; ++K){
		for(size_t L=0; L<percepciones; ++L){
			in >> input[K][L];
			in.ignore(); //csv o ssv funciona
		}
		
		for(size_t L=0; L<salidas; ++L){
			in >> result[K][L];
			in.ignore(); //csv o ssv funciona
		}

		//input[K][percepciones] = 1; //entrada extendida
		result[K][salidas] = 1; //entrada extendida
	}

	if(out){
		fprintf(out, "%lu\n", input.size());
		for(size_t K=0; K<input.size(); ++K)
			fprintf( out, "%f %f\n", input[K][0], input[K][1] );
		fflush(out);
	}
}


void simulator::train(){
	red.train(input, out);
}


