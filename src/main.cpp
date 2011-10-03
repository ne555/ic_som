#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include "som.h"
using namespace std;

void usage (int status)
{
	if (status != EXIT_SUCCESS)
		cerr << "Try \'-h\' for more information.\n";
	else{
		cerr << "Usage: program.bin [OPTIONS]\n";
		cerr << "Mapa autoorganizativo\n" << 
		"-e string \t Nombre del archivo de entrenamiento\n" << 
		"-p string \t Nombre del archivo de prueba\n" << 
		"-E float \t Velocidad de aprendizaje (eta)\n" << 
		"-g \t Grafica la evolucion del sistema\n" << 
		"-h \t Ayuda del programa\n";
	}

	exit (status);
}

int main (int argc, char **argv) {
	int entradas, row, column, salidas;
	int option;
    const char *train_file, *test_file;
    float eta = 0.05, success = 0.9;
	FILE *out=NULL;
	while( (option=getopt(argc, argv, "e:p:E:gh")) != -1 ){
		switch(option){
		case 'e': train_file=optarg; break;
		case 'p': test_file=optarg; break;
		case 'g': if(not out) out=popen("./bin/grapher", "w");break;
		case 'E': eta=strtof(optarg, NULL); break;
		case 'h': usage(EXIT_SUCCESS); break;
		default: usage(EXIT_FAILURE);
		}
	}

	//if(not out) cerr << "Broken pipe\n";
	// Leer estructura
	//cin >> entradas >> ncg >> ncs;
	cin>>entradas>>salidas>>row>>column;

	srand(time(NULL));

	SOM red(row, column, entradas, salidas, eta);
	red.read(train_file, out);
	red.inicializar();
	red.entrenar(out);

	red.read(test_file, NULL);
	red.test();
	if(out) pclose(out);
	return 0;
}


