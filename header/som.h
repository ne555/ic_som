#ifndef SOM_H
#define SOM_H

#include <vector>
#include <utility>
#include "neurona_som.h"

typedef pair<int, int> punto;

using namespace std;

class SOM {
public:
    SOM (int m, int n, int entradas, int salidas=0);
    void read (const char *filename, FILE *out=NULL);
    void inicializar ();
    punto calcular (vector<float> patron);
    void entrenar (FILE *out=NULL);

private:
    void entrenar_topo (FILE *out=NULL);
    void entrenar_trans (FILE *out=NULL);
    void entrenar_fino (FILE *out=NULL);
    void entrenar_area (punto & ganador, int lambda, float eta, vector<float> & patron);
	void graph(FILE *out);

    vector<vector<neurona_som> > mapa;
    int m, n; // dimensiones
    int entradas, salidas;
    vector<vector<float> > input, result;
};

#endif
