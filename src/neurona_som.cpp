#include "neurona_som.h"
#include <iostream>

void neurona_som::set_weights (vector<float> & weights) {
    this->weights = weights;
}

float neurona_som::calcular_distancia (vector<float> & patron) {
    vector<float> dif;
    dif = restar_vectores(weights, patron);
    return norma2(dif);
}

void neurona_som::entrenar(vector<float> & patron, float eta) {
    for (int i = 0; i < weights.size(); i++) {
        weights[i] += eta * (patron[i] - weights[i]);
    }
}

void neurona_som::graph(FILE *out){
	if(not out) return;

	fprintf(out, "%f %f  ", weights[0], weights[1]);
}
