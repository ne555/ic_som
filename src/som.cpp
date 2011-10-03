#include <vector>
#include <fstream>
#include <iostream>
#include "som.h"
#include <cmath>


#include <algorithm>

typedef pair<int, int> punto;

SOM::SOM (int m, int n, int entradas,int salidas, float eta) {
    this->m = m;
    this->n = n;
    this->entradas = entradas;
    this->salidas = salidas;
    mapa.resize(m, vector<neurona_som>(n));
	capa_salida.resize(salidas,neurona(n*m,eta));
}

void SOM::read (const char *filename, FILE *out) {
	int ent;

    ifstream file (filename);
    file >> ent;
    input.resize(ent, vector<float>(entradas));
    result.resize(ent, vector<float>(salidas));

	for(int K=0; K<ent; ++K){
		for(int L=0; L<entradas; ++L){
			file >> input[K][L];
			file.ignore(); //csv o ssv funciona
		}
		
		
		for(int L=0; L<salidas; ++L){
			file >> result[K][L];
			file.ignore(); //csv o ssv funciona
		}
	}

	if(out){
		fprintf(out, "%d\n", ent);
		for(int K=0; K<ent; ++K)
			fprintf(out, "%f %f\n", input[K][0], input[K][1]);
		fflush(out);
	}
}

void SOM::inicializar () {
    int ind;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ind = randomize(0, input.size() - 1);
            mapa[i][j].set_weights(input[ind]);
        }
    }
	for(int i=0;i<capa_salida.size();i++)
		capa_salida[i].inicializar();
}

punto SOM::calcular (vector<float> patron) {
    int i_min, j_min;
    float dist_min, dist;

    // Tomo el primero
    i_min = 0;
    j_min = 0;
    dist_min = mapa[0][0].calcular_distancia(patron);

    for (int i = 0; i < m; i++) { 
        for (int j = 0; j < n; j++) {
            dist = mapa[i][j].calcular_distancia(patron);
            if (dist < dist_min) {
                i_min = i;
                j_min = j;
                dist_min = dist;
            }
        }
    }

	//cerr << patron[0] << ' ' << patron[1] << " -> " << mapa[i_min][j_min].weights[0] << ' ' << mapa[i_min][j_min].weights[1] << endl;

    return punto(i_min, j_min);
}

void SOM::entrenar (FILE *out) {
	cerr << "topologico\n";
    entrenar_topo(out);
	cerr << "transicion\n";
    entrenar_trans(out);
	cerr << "ajuste_fino\n";
    entrenar_fino(out);
	entrenar_capa_salida(0.9);
}

void SOM::entrenar_topo (FILE *out) {
    punto ganador;
    float eta = 0.9;
    int n_epocas = 500, lambda = max(m, n)/2;

    for (int K = 0; K < n_epocas; K++) {
        for (int i = 0; i < input.size(); i++) {
            ganador = calcular(input[i]);
            entrenar_area(ganador, lambda, eta, input[i]);
        }
		//random_shuffle(input.begin(), input.end());
		graph(out);
    }
}

void SOM::entrenar_trans (FILE *out) {
    punto ganador;
    float eta = 0.9, lambda = max(m, n)/2.0;
    float eta_i = 0.9, eta_f = 0.1, eta_dif = eta_f - eta_i;
    float lambda_i = max(m, n)/2.0, lambda_f = 1.0, lambda_dif = lambda_f - lambda_i;
    int n_epocas = 1000;

	float alfa = log(eta_i/eta_f)/n_epocas;
	eta = eta_i;

    for (int epoca = 0; epoca < n_epocas; epoca++) {
        for (int i = 0; i < input.size(); i++) {
            ganador = this->calcular(input[i]);
            entrenar_area(ganador, lambda, eta, input[i]);
        }
        // eta y lambda se reducen linealmente
        //eta = eta_i + epoca * eta_dif / n_epocas;
        //lambda = lambda_i + epoca * lambda_dif / n_epocas;
		//eta += eta_dif/n_epocas;
		lambda += lambda_dif/n_epocas;
		
		eta /= exp(alfa);

		//random_shuffle(input.begin(), input.end());
		graph(out);
    }
}

void SOM::entrenar_fino (FILE *out) {
    punto ganador;
    float eta = 0.05;
    int n_epocas = 3000, lambda = 0;
	
	n_epocas = 100;

    for (int epoca = 0; epoca < n_epocas; epoca++) {
        for (int i = 0; i < input.size(); i++) {
            ganador = this->calcular(input[i]);
            entrenar_area(ganador, lambda, eta, input[i]);
        }
		//random_shuffle(input.begin(), input.end());
		graph(out);
    }
}

void SOM::entrenar_area (punto & ganador, int lambda, float eta, vector<float> & patron) {
    int a = ganador.first, b = ganador.second;

    for (int i = (a-lambda>0)?-lambda:-a; i <= lambda; i++) {
        a = ganador.first + i;
		if( a>=m ) break;
		b = ganador.second;
        for (int j = (b-lambda>0)?-lambda:-b; j <= lambda; j++) {
            b = ganador.second + j;
			if( b>=n ) break;
			double alfa=1;
			if(lambda!=0)
				alfa = (lambda - 0.9*max(abs(i),abs(j)) ) / float(lambda);

            mapa[a][b].entrenar(patron, alfa*eta);
        }
    }
}

void SOM::graph(FILE *out){
	if(not out) return;

	fprintf(out, "%lu %lu\n", mapa.size(), mapa[0].size());
	for(size_t K=0; K<mapa.size(); ++K){
		for(size_t L=0; L<mapa[K].size(); ++L)
			mapa[K][L].graph(out);
		fprintf(out, "\n");
	}
	fflush(out);
}

vector<float> SOM::salida(vector<float> &e){
	vector<float> r(n*m, 0);
	punto ganador = calcular(e);
	r.at( ganador.first*n + ganador.second ) = 1;
	return  r;
}

vector<float> SOM::salida_perceptron(vector<float> &e){
	vector<float> salida;
	e.push_back(1);
	for(int i=0;i<salidas;i<i++){
		salida.push_back(capa_salida[i].calcular(e));
	}
	return salida;
}

int SOM::entrenar_capa_salida(float acierto_minimo, FILE *out){
	
    vector<float> salidasom, salida_obtenida;
    int epoca, aciertos;
	int cant_epocas = 300;

	ofstream graficar("grafico"), salsa("soma"), out_file("somalia");

    for (epoca = 1; epoca <= cant_epocas; epoca++) {
        aciertos = 0;
	//	graficar << input.size() << endl;

		for(size_t K=0; K<input.size(); ++K){
			for(size_t L=0; L<2; ++L){
				out_file << input[K][L] << ' ';
			}
			out_file << result[K][0] << endl;
		}
		out_file.close();

        for (int i = 0; i < input.size(); i++) { // por cada patron de entrenamiento

            salidasom = salida(input[i]);
					punto ganador = calcular(input[i]);
					vector<float> vg = mapa[ganador.first][ganador.second].weights;

			if( signo(vg[0]) == signo(vg[1]) ){
				if( signo(result[i][0]) == 1 ){
					cerr << "No vale ";
			cerr << vg[0] << ' ' << vg[1] << " -> " << input[i][0] << ' ' << input[i][1] << ' ' << signo(result[i][0]) << endl;
			throw "malo";
				}
			
			}else{
				if( signo(result[i][0]) == -1 ){
					cerr << "No vale ";
			cerr << vg[0] << ' ' << vg[1] << " -> " << input[i][0] << ' ' << input[i][1] << ' ' << signo(result[i][0]) <<  endl;
			throw "malo";
				}
			}


            salida_obtenida = salida_perceptron(salidasom);
            //if (comparar_vectores(salida_obtenida, result[i])) { // si acierta
			if( signo(salida_obtenida[0]) == signo(result[i][0]) ){
                aciertos++;
            } 
            //else { // si no son iguales, entreno las neuronas
                for (int j = 0; j < salidas; j++) {
                    capa_salida[j].entrenar(salidasom, result[i][j] - salida_obtenida[j]);
                }
            //}
			//graficar <<input[i][0] <<' '<< input[i][1]<<' '<<signo(salida_obtenida[0]) << endl;;

			for(size_t K=0; K<salidasom.size(); ++K)
				salsa << salidasom[K] <<  ' ';
			
			salsa << result[i][0] << endl;
        }

		graph(out);

        // si el porcentaje de aciertos es el deseado, salgo
        if ( (float(aciertos)/input.size()) >= acierto_minimo) break;
    }

	cout<<"cantidad de epoca: "<< epoca << " cantidad de aciertos: "<< float(aciertos)/input.size()<<endl;;
	cout << "Los pesos ";
	capa_salida[0].print();
    if (epoca <= cant_epocas) { // si convergio
    return epoca;
	}
    else {
        return -1;
    }
}

vector<float> SOM::test(vector<float> &e){
	vector<float> result;
	vector<float> intermedio;
	intermedio = salida(e);
	result = salida_perceptron(intermedio);
	return result;
}

