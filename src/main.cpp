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
		"-f string \t Nombre del archivo\n" << 
		"-g \t Grafica la evolucion del sistema\n" << 
		"-h \t Ayuda del programa\n";
	}

	exit (status);
}

int main (int argc, char **argv) {
	int entradas, row, column;
	int option;
	const char *file;
	FILE *out=NULL;
	while( (option=getopt(argc, argv, "f:gh")) != -1 ){
		switch(option){
		case 'f': file=optarg; break;
		case 'h': usage(EXIT_SUCCESS); break;
		case 'g': if(not out) out=popen("./bin/grapher", "w");break;
		default: usage(EXIT_FAILURE);
		}
	}

	if(not out) cerr << "Broken pipe\n";
	// Leer estructura
	//cin >> entradas >> ncg >> ncs;
	cin>>entradas>>row>>column;

	srand(time(NULL));

	SOM red(row, column, entradas, 0);
	red.read(file);
	red.inicializar();
	red.entrenar(out);

	if(out) pclose(out);
	return 0;
}


