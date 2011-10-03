#include <fstream>
#include <iostream>

using namespace std;

simulator::simulator (int n_entradas, int neuronas_som_n, int neuronas_som_m, int neuronas_capa_salida, float eta) {
    capa_som(neuronas_som_m, neuronas_som_n,n_entradas); 
    capa_salida.resize(
            neuronas_capa_salida, 
            neurona(neurona_som_n*neurona_som_m, eta) // inicializacion de la neurona
    );

    ncs = neuronas_capa_salida;
    entradas = n_entradas;

    // Inicializar los pesos de las neuronas de salida
    for (int i = 0; i < neuronas_capa_salida; i++) {
        capa_salida[i].inicializar();
    }
}



vector<float> simulator::calcular_salida (vector<float> & entrada) {
    vector<float> salida;
    vector<float>salida_som;

	salida_som = capa_som.salida(entrada);

    salida = capa_salida.calcular(salida_som);
    return salida;
}

void simulator::read (const char *filename, FILE *out) {
	capa_som.read(filename, out);
}

void simulator::inicializar(){
	capa_som.inicializar();
}

void simulator::entrenar_som (FILE *out) {
	capa_som.entrenar(out);
}

int RBF::entrenar_capa_salida (int cant_epocas, float acierto_minimo, FILE *out) {
    vector<float> salidasom, salida_obtenida;
    int epoca, aciertos;

    for (epoca = 1; epoca <= cant_epocas; epoca++) {
        aciertos = 0;

        for (int i = 0; i < capa_som.sizeinput(); i++) { // por cada patron de entrenamiento
         	
            salida_obtenida = capasalida.salida(i);
            if (comparar_vectores(salida_obtenida, ) { // si acierta
                aciertos++;
            } 
            else { // si no son iguales, entreno las neuronas
                for (int j = 0; j < ncs; j++) {
				   salidasom = capa_som(i);
                   capa_salida[j].entrenar(salidasom,0.1)  }
            }
        }

		graph(out);

        // si el porcentaje de aciertos es el deseado, salgo
        if ( (float(aciertos)/input.size()) >= acierto_minimo) break;
    }

    if (epoca <= cant_epocas) { // si convergio
        return epoca;
    }
    else {
        return -1;
    }
}

void RBF::prueba () {
    vector<float> salida;
    for (int i = 0; i < input.size(); i++) { // por cada patron de entrenamiento
        salida = calcular_salida(input[i]);
        cout << "Entrada " << i << ":" << endl;
        imprimir_vector(input[i]);
        cout << "Salida esperada " << i << ":" << endl;
        imprimir_vector(result[i]);
        cout << "Salida " << i << ":" << endl;
        imprimir_vector(salida);
        cout << endl;
    }
}


void simulator::graph(FILE *out){
	if(not out) return;
	const size_t n=200;
	const float limit=2.f;
	fprintf(out, "-1\n");
	fprintf(out, "%lu\n", n*n);
	for(size_t K=0; K<n; ++K){
		for(size_t L=0; L<n; ++L){
			float x = K/float(n)*limit*2 - limit, y = L/float(n)*limit*2 - limit;
			vector<float> v(2);
			v[0] = x; v[1] = y;// v[2] = 1;
			int salida = signo(calcular_salida(v)[0]);
			fprintf( out, "%f %f %d\n", v[0], v[1], salida );
		}
	}
	fflush(out);
}


