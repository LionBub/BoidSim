//main.cpp
#include "main.hpp"
#include "bird.hpp"
#include <GL/gl.h>

//complie using g++ -o myprogram main.cpp bird.cpp -lGL -lGLU -lglut -lm

using namespace std;
static const float PI = 3.14159265359;


int indexOfMinimumFloatArray(float *arr) {
	int recordIndex = 0;
	int recordMin = arr[0];
	for (int i = 1; i < sizeof(*arr); i++) {
		if (arr[i] < recordMin) {
			recordMin = arr[i];
			recordIndex = i;
		}
	}
	return recordIndex;
}

float distBetweenPoints(float ax, float ay, float bx, float by) {
	return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

float distBetweenBirds(Bird& a, Bird& b){ //I want this to be a pass by reference since im not modifing the birds at all, so I do not want to make copies of them
	//distance between two points
	return sqrt(pow(b.getx() - a.getx(), 2) + pow(b.gety() - a.gety(),2));
}

void drawLineToNearestBird(Bird& bird){ //Passing by reference. SHOULD BRD BE A POINTER??!?!
	Bird* nearBrd = bird.pGetNearestBird();
	if(nearBrd == nullptr) return;
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex2f(bird.getx(), bird.gety());
	glVertex2f(nearBrd -> getx(), nearBrd -> gety());
	glEnd();
}

void debugShowFlockingRange(Bird& bird){
	Bird* nearestBrd = bird.pGetNearestBird();
	for (int i = 0; i < size(birds); i++){ //birds is the list of all birds, found in bird.cpp
		if (&bird == &birds[i]) continue;
		if(&birds[i] == nearestBrd){
			glColor3f(1,0,1);
		}
		else if (distBetweenBirds(bird, birds[i]) < FLOCKING_RANGE){
			glColor3f(0.6,0.6,1);
		}
		else{
			glColor3f(0.95,0.95,1);
		}

		glBegin(GL_LINES);
		glVertex2f(bird.getx(), bird.gety());
		glVertex2f(birds[i].getx(), birds[i].gety());
		glEnd();

		glColor3f(0.5,1,0.5);
		drawHollowCircle(birds[0].getx(), birds[0].gety(), FLOCKING_RANGE);
	}
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

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius){
	// https://gist.github.com/linusthe3rd/803118
	int i;
	int lineAmount = 16; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	
	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) { 
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)), 
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}

void WaitForEndOfFrame(){ //thanks chat gpt
	static struct timespec last_time;
	struct timespec this_time;
	clock_gettime(CLOCK_MONOTONIC, &this_time);
	if(last_time.tv_sec == 0 && last_time.tv_nsec == 0) {
		last_time = this_time; //Initialize last_time on the first call
	} 
	else{
		// Calculate the time to sleep to achieve the desired frame rate
		const long NANOSECONDS_PER_SECOND = 1000000000;
		const int timePerFrame = NANOSECONDS_PER_SECOND / FPS_LIMIT;

		// Calculate the time difference between this_time and last_time
		long elapsed_ns = (this_time.tv_sec - last_time.tv_sec) * NANOSECONDS_PER_SECOND +
							this_time.tv_nsec - last_time.tv_nsec;

		long timeToSleep = timePerFrame - elapsed_ns;
		if (timeToSleep > 0) {
			struct timespec sleep_duration;
			sleep_duration.tv_sec = timeToSleep / NANOSECONDS_PER_SECOND;
			sleep_duration.tv_nsec = timeToSleep % NANOSECONDS_PER_SECOND;

			//Sleep for the required duration
			nanosleep(&sleep_duration, NULL);
		}
	}
	last_time = this_time;
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
	// SECTION LINES
	glColor3f(0.2,0.2,0.2);
	glBegin(GL_LINES);
		glVertex2f(wallx0, wally0);
		glVertex2f(wallx1, wally1);
		glVertex2f(wallx0, wally1);
		glVertex2f(wallx1, wally0);
	glEnd();
	// DRAW ALL BIRDS
	for (int i = 0; i < size(birds); i++){
		Bird thisBird = birds[i];
		birds[i].step(1);
		birds[i].teleportOnWall();
		birds[i].getNearbyBirds();
		//birds[i].turnTowardsVector(-1, 0);
		birds[i].steerAwayFromWall();
		if (!birds[i].risksCollision()){
			birds[i].steerTowardsCenterOfFlock();
			birds[i].avoidEachOther();

		}
		birds[i].draw();
		//drawLineToNearestBird(birds[i]);
	}
	debugShowFlockingRange(birds[0]);
	glutSwapBuffers();
	WaitForEndOfFrame();
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


	
