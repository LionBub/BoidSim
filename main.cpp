#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <string>
#include <vector>
#include <random>

//complie using $ g++ -o main main.cpp -lGL -lGLU -lglut -lm
using namespace std;
class Bird;
const float PI = 3.1415;
const float BIRDSCALE = 5.0;
const int BIRDCOUNT = 10;
const float FLOCKING_RANGE = 100.0;
const float wallx0 = 50.0;
const float wallx1 = 650.0;
const float wally0 = 50.0;
const float wally1 = 650.0;
int highestBirdId = 0;
std::vector<Bird> birds; 
void drawTriangle(float x, float y, float thetaRad);
float distBetweenBirds(Bird& a, Bird& b);
void drawLineToNearestBird(Bird& bird);
std::vector<Bird> createRandomBirds(int n);

class Bird {
private:
	float x;
	float y;
	float rot;
	int id;
	std::vector<Bird*> nearbyBirds; /* i think this should hold pointers to save memory because 
									I wont have multiple copies of birds in different places. All point back to the original birds vector 
									What if I use Bird& references instead?!?!*/
public:
	Bird(float _x, float _y, float _rot) 
		: x(_x), y(_y), rot(_rot) {
			id = highestBirdId++;
	}
	float getx() {return x;}
	void setx(float newx) {x = newx;}
	float gety() {return y;}
	void sety(float newy) {y = newy;}
	float getRot() {return rot;}
	void setRot(float newrot) { 
		newrot = fmod(newrot, 360);
		rot = newrot;
	}
	void addRot (float num) { setRot(rot + num); }
	void draw(){
		glColor3f((360-rot)/360, rot/360, 0);
		drawTriangle(x, y, rot);
	}
	void step(float distance){
		x += distance * cos(rot * PI/180);//cos is in rad, rot is in degrees
		y += distance * sin(rot * PI/180);
	}
	// Bird* pFindNearestBird(){ //NOT USED
	// 	if (size(birds) > 1){
	// 		int recordIndex;
	// 		float recordDist;
	// 		if (this != &birds[0]){
	// 			recordIndex = 0;
	// 			recordDist = distBetweenBirds(*this, birds[0]); //bird zero to bird 1
	// 		}
	// 		else { //If this is birds[0]
	// 			std::cout << std::endl;
	// 			recordIndex = 1;
	// 			recordDist = distBetweenBirds(*this, birds[1]); //bird zero to bird 1
	// 		}
	// 		for(int i = 1; i < size(birds); i++){
	// 			if (this == &birds[i]) continue;
	// 			float thisDist = distBetweenBirds(*this, birds[i]);
	// 			if (thisDist < recordDist) { 
	// 				recordIndex = i;
	// 				recordDist = thisDist; 
	// 			}
	// 		}
	// 		return &birds[recordIndex];
	// 		/* std::cout << "Bird " + std::to_string(getId()) + " to " + std::to_string(nearestBird -> getId())
	// 			+"    at a distance of " + std::to_string(recordDist)<< std::endl; 
	// 		*/
	// 	}
	// 	std::cout << "can't find nearby birds. Is there only one bird? findNearestBird() returned null" << std::endl;
	// }
	Bird* pGetNearestBird() { return nearbyBirds[0];}
	void findNearbyBirds() {
		for(int i = 0; i < size(birds); i++){
			if(distBetweenBirds(*this, birds[i]) < FLOCKING_RANGE){
				nearbyBirds.insert(nearbyBirds.begin(), &birds[i]);
			}
			else {
				nearbyBirds.emplace_back(&birds[i]);
			}
		}
	}
	int getId() {return id;}
	void steerAwayFromWall() {
		
	}
};

float distBetweenBirds(Bird& a, Bird& b){ //I want this to be a pass by reference since im not modifing the birds at all, so I do not want to make copies of them
	//distance between two points
	return sqrt(pow(b.getx() - a.getx(), 2) + pow(b.getx() - a.getx(),2));
}

void drawLineToNearestBird(Bird& bird){ //Passing by reference. SHOULD BRD BE A POINTER??!?!
	glColor3f(0,0,1);
	Bird* nearBrd = bird.pGetNearestBird();
	glBegin(GL_LINES);
	glVertex2f(bird.getx(), bird.gety());
	glVertex2f(nearBrd -> getx(), nearBrd -> gety());
	glEnd();
}

std::vector<Bird> createRandomBirds(int n) { //thanks chatgpt
	vector<Bird> birds;
	random_device rd;//this creates a more uniformly distributed randomness over 2d space.
	mt19937 gen(rd()); //apparently also more suitable for multithreaded operations.
	uniform_real_distribution<float> xDist(wallx0, wallx1);
	uniform_real_distribution<float> yDist(wally0, wally1);
	uniform_real_distribution<float> rotDist(0.0f, 360.0f);
	
	for (int i = 0; i < n; ++i) {
		float randomX = xDist(gen);
		float randomY = yDist(gen);
		float randomRot = rotDist(gen);
		birds.emplace_back(randomX, randomY, randomRot);
	}
	return birds;
}




void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	// Your OpenGL drawing code goes here
	//WHITE SQUARE
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(wallx0, wally0);
		glVertex2f(wallx1, wally0);
		glVertex2f(wallx1, wally1);
		glVertex2f(wallx0, wally1);
	glEnd();
	// DRAW ALL BIRDS
	for (int i = 0; i < size(birds); i++){
		birds[i].step(1);
		birds[i].findNearbyBirds();
		birds[i].addRot(1);
		birds[i].draw();
		drawLineToNearestBird(birds[i]);
	}
	glutSwapBuffers();
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
	    cerr << "OpenGL Error: " << gluErrorString(error) << endl;
	}
}

void update(){
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
   
   birds = createRandomBirds(BIRDCOUNT);
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION); //setup viewing projection
   glLoadIdentity(); //start with identity matrix
   glOrtho(0, 700, 0, 700, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
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


	
