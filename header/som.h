#ifndef SOM_H
#define SOM_H

#include <vector>
#include <utility>
#include <cstdio>
#include "neurona_som.h"

class SOM {
public:
	typedef neurona_som::value_type value_type;
	typedef neurona_som::vector vector;
	typedef std::vector<vector> matrix;

	typedef std::vector<neurona_som> fila;
	typedef std::vector<fila> layer;

	typedef std::pair<size_t,size_t> coord;

    SOM (size_t entradas, size_t row, size_t column);

    void train(const matrix &patrones, FILE *out=NULL);
	void graph(FILE *out);
private:
    void topologico(const matrix &patrones, FILE *out=NULL);
    void transicion(const matrix &patrones, FILE *out=NULL);
    void ajuste_fino(const matrix &patrones, FILE *out=NULL);
    void entrenar_area (size_t vecindad, float alfa, const vector &patron);
	coord mas_cercano(const vector &patron);

	layer mapa;
    size_t entradas, row, column;
};

#endif
