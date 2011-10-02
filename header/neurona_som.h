#ifndef NEURONA_SOM_H
#define NEURONA_SOM_H

#include <vector>
#include <cstdio>
#include "util.h"

using namespace std;

class neurona_som {
public:
    void set_weights (vector<float> & weights);
    float calcular_distancia (vector<float> & patron);
    void entrenar(vector<float> & patron, float eta);

	void graph(FILE *out);
private:
public:
    vector<float> weights;

};

#endif
