#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <GL/glut.h>

#include <unistd.h>
using namespace std;

const float 
	white[] = {1, 1, 1, 1}, 
	red[]   = {1, 0, 0, 1}, 
	green[] = {0, 1, 0, 1}, 
	blue[]  = {0, 0, 1, 1}, 
	black[] = {0, 0, 0, 1}, 
	dark_red[]   = {.5, 0, 0, 1}, 
	dark_green[] = {0, .5, 0, 1}, 
	dark_blue[]  = {0, 0, .5, 1}, 
	light_red[]   = {1, 0.75, 0.75, 1}, 
	light_green[] = {0.75, 1, 0.75, 1}, 
	light_blue[]  = {0.75, 0.75, 1, 1};

enum estado { Patrones=-1, Centroide=-2 };

typedef void  (*cargar_dato)(size_t);

void leer_puntos(size_t row, size_t column);

struct point{
	float x[2];
	float& operator[](int index){
		return x[index];
	}
	float operator[](int index) const{
		return x[index];
	}
};

int width=480, height=480;

//vector< vector<float> > points;
vector<vector<point> > points;
vector<int> clase;

void display();
void draw_puntos();
void draw_conections();
void wait_for_input();

void fixed_background();

void init();

int main(int argc, char **argv){
	glutInit(&argc, argv);
	init();
	glutMainLoop();
	return 0;
}

void init(){
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ACCUM);
	glutInitWindowSize(width,height); glutInitWindowPosition(50,50);
	glutCreateWindow("Graficador de puntos de colores");
	glutDisplayFunc(display);
	glutIdleFunc(wait_for_input);

	glClearColor( white[0], white[1], white[2], 1);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	const float factor=0.5;
	glScaled(factor,factor,factor);
	
	glLineWidth(2);
	glPointSize(3);
	glColor3fv(black);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glAccum(GL_RETURN, 1);

	draw_puntos();
	draw_conections();

	glutSwapBuffers();
}

void wait_for_input(){
	static bool first_time = true;
	if(first_time){
		fixed_background();
		first_time = false;
		glutPostRedisplay();
		return;
	}

	size_t row,column;
	cin>>row>>column;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if(not cin){
		glutIdleFunc(NULL);
		cin.clear();
		string s;
		while(getline(cin,s)) //el programa finalizo. se hace un bypass a la entrada
			cerr << s << endl;
		cerr << "Fin";
		glutPostRedisplay();
		return;
	}
	leer_puntos(row,column);
	glutPostRedisplay();
}


void leer_puntos(size_t row, size_t column){
	points.resize(row, vector<point>(column));
	for(size_t K=0; K<row; ++K)
		for(size_t L=0; L<column; ++L)
			cin>>points[K][L][0]>>points[K][L][1];
}

void draw_puntos(){
	if(points.size()==0) return;
	size_t row = points.size(), column = points[0].size();
	glBegin(GL_POINTS);{
		for(size_t K=0; K<row; ++K)
			for(size_t L=0; L<column; ++L){
				glVertex2fv(points[K][L].x);
			}
	};glEnd();
}

void draw_conections(){ 
	if(points.size()==0) return;
	size_t row = points.size(), column = points[0].size();
	glBegin(GL_LINES);{
		for(size_t K=0; K<row-1; ++K)
			for(size_t L=0; L<column-1; ++L){
				glVertex2fv(points[K][L].x);
				glVertex2fv(points[K][L+1].x);
				glVertex2fv(points[K][L].x);
				glVertex2fv(points[K+1][L].x);
			}
		for(size_t K=0; K<row-1; ++K){
			glVertex2fv(points[K].back().x);
			glVertex2fv(points[K+1].back().x);
		}
		for(size_t L=0; L<column-1; ++L){
			glVertex2fv(points.back()[L].x);
			glVertex2fv(points.back()[L+1].x);
		}
	};glEnd();
}

void fixed_background(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3fv(red);
	int n;
	glBegin(GL_POINTS);{
		for(cin>>n; n>0; --n){
			point p;
			cin >> p[0] >> p[1];
			glVertex2fv(p.x);
		}
	;}glEnd();

	glAccum(GL_LOAD, 1);
	glColor3fv(black);
}
