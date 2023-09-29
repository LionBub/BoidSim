#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <random>

//complie using $ g++ -o main main.cpp -lGL -lGLU -lglut -lm
using namespace std;
const float PI = 3.1415;
const float BIRDSCALE = 5;
void drawTriangle(float x, float y, float thetaRad);


class Bird {
private:
	float x;
	float y;
	float rot;
public:
	Bird(float _x, float _y, float _rot) 
		: x(_x), y(_y), rot(_rot) {
	}
	float getx() {return x;}
	void setx(float newx) {x = newx;}
	float gety() {return y;}
	void sety(float newy) {y = newy;}
	float getRot() {return rot;}
	void setRot(float newrot) { rot = newrot;}
	void addRot (float num) { rot += num;}
	void draw(){
		glColor3f((360-rot)/360, rot/360, 0);
		drawTriangle(x, y, rot);
	}
	void step(float distance){
		x += distance * cos(rot * PI/180);//cos is in rad, rot is in degrees
		y += distance * sin(rot * PI/180);
	}
};

vector<Bird> createRandomBirds(int n) { //thanks chatgpt
	vector<Bird> birds;
	random_device rd;//this creates a more uniformly distributed randomness over 2d space.
	mt19937 gen(rd()); //apparently also more suitable for multithreaded operations.
	uniform_real_distribution<float> xDist(50.f, 650.0f);
	uniform_real_distribution<float> yDist(50.f, 650.0f);
	uniform_real_distribution<float> rotDist(0.0f, 360.0f);
	
	for (int i = 0; i < n; ++i) {
		float randomX = xDist(gen);
		float randomY = yDist(gen);
		float randomRot = rotDist(gen);
		birds.emplace_back(randomX, randomY, randomRot);
	}
	return birds;
}
//MORE VARIABLES
float spin = 0;
vector<Bird> birds;


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	// Your OpenGL drawing code goes here
	//WHITE SQUARE
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(50, 50);
		glVertex2f(650, 50);
		glVertex2f(650, 650);
		glVertex2f(50, 650);
	glEnd();
	for (int i = 0; i < size(birds); i++){
		birds[i].step(1);
		birds[i].addRot(1);
		birds[i].draw();
	}
	glutSwapBuffers();
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
	    cerr << "OpenGL Error: " << gluErrorString(error) << endl;
	}
}

void update(){
   spin = spin + 0.2;
   if (spin > 360.0)
      spin = spin - 360.0;
     
   
   glutPostRedisplay();
}



void drawTriangle(float x, float y, float thetaDeg){
//isocoles, 20px long, 10px wide
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(thetaDeg, 0, 0, 1); //rotate theta degrees around the <0,0,1> vector
	glBegin(GL_POLYGON); //can I use vertex arrays?
		glVertex2f(4 * BIRDSCALE, 0);
		glVertex2f(0, BIRDSCALE);
		glVertex2f(0, -BIRDSCALE);
	glEnd();
	glPopMatrix();
}
void init(void) 
{
   glClearColor (0.9, 0.9, 0.9, 1);
   glShadeModel (GL_FLAT);
   
   birds = createRandomBirds(10);
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION); //setup viewing projection
   glLoadIdentity(); //start with identity matrix
   glOrtho(0, 700, 0, 700, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   std::cout<<"reshape"<<std::endl;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Boid Simulator");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutMainLoop();
	return 0;
}


	
