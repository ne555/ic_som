#include <vector>
#include <utility>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "som.h"

using namespace std;

SOM::SOM (size_t entradas, size_t row, size_t column):
	entradas(entradas), row(row), column(column),
	mapa( row, fila(column, neurona_som(entradas)) )
{
	for(size_t K=0; K<row; ++K)
		for(size_t L=0; L<column; ++L)
			mapa[K][L].init(); //pesos aleatorios
}

SOM::coord SOM::mas_cercano(const vector &patron) {
	float dist_min;
	coord ganador;

	// Tomo el primero
	ganador.first = 0;
	ganador.second = 0;
	dist_min = mapa[0][0].distance(patron);

	for(size_t K=0; K<row; ++K)
		for(size_t L=0; L<column; ++L){
			float dist = mapa[K][L].distance(patron);
			if (dist < dist_min) {
				ganador.first = K;
				ganador.second = L;
				dist_min = dist;
			}
		}
	
	return ganador;
}

void SOM::train(const matrix &patrones, FILE *out){
	cerr << "topologico\n";
	topologico(patrones, out);
	cerr << "transicion\n";
	transicion(patrones, out);
	cerr << "ajuste_fino\n";
	ajuste_fino(patrones, out);
}

void SOM::topologico(const matrix &patrones, FILE *out){
	const float alfa = 0.9;
	const size_t epocas = 500, vecindad = max(row,column)/2;

	for (size_t K = 0; K < epocas; K++){
		for (size_t L = 0; L < patrones.size(); L++){
			size_t index = rand()%patrones.size();
			entrenar_area(vecindad, alfa, patrones[index]);
		}
		
		//random_shuffle(input.begin(), input.end());
		cerr << K << ' ';
		graph(out);
	}
}

void SOM::transicion(const matrix &patrones, FILE *out){
	const size_t epocas = 1000;
	const float 
		alfa_inic = 0.9, 
		alfa_final = 0.1,
		alfa_delta = (alfa_final-alfa_inic)/epocas,
		vecindad_inic = max(row, column)/2.0,
		vecindad_final = 1.0,
		vecindad_delta = (vecindad_final-vecindad_inic)/epocas;

	float alfa = alfa_inic, vecindad = vecindad_inic;

	for (size_t K = 0; K < epocas; K++){
		for (size_t L = 0; L < patrones.size(); L++){
			size_t index = rand()%patrones.size();
			entrenar_area(vecindad, alfa, patrones[index]);
		}
		// eta y lambda se reducen linealmente
		alfa += alfa_delta;
		vecindad += vecindad_delta;
		
		//random_shuffle(input.begin(), input.end());
		cerr << K << ' ';
		//cerr << alfa << '|' << vecindad << ' ';
		graph(out);
	}
}

void SOM::ajuste_fino(const matrix &patrones, FILE *out){
	const float alfa = 0.01;
	const size_t epocas = 400, vecindad = 0;

	for (size_t K = 0; K < epocas; K++){
		for (size_t L = 0; L < patrones.size(); L++){
			size_t index = rand()%patrones.size();
			entrenar_area(vecindad, alfa, patrones[index]);

		}
		
		//random_shuffle(input.begin(), input.end());
		cerr << K << ' ';
		graph(out);
	}
}

void SOM::entrenar_area (size_t vecindad, float alfa, const vector &patron){
	coord ganador = mas_cercano(patron);
	const int a = ganador.first, b = ganador.second;

	for(size_t K = max<int>(0,a-vecindad); K<min(row,a+vecindad+1); ++K){
		for(size_t L = max<int>(0,b-vecindad); L<min(column,b+vecindad+1); ++L){
			//afecta de forma uniforme
			mapa[K][L].train(patron, alfa);
		}
	}
}

void SOM::graph(FILE *out){
	if(not out) return;

	fprintf(out, "%lu %lu\n", row, column);
	for(size_t K=0; K<row; ++K){
		for(size_t L=0; L<column; ++L)
			mapa[K][L].graph(out);
		fprintf(out, "\n");
	}
	fflush(out);
}

