//main.hpp
#ifndef MAIN_HPP
#define MAIN_HPP

#include <bits/types/struct_timespec.h>
#include <bits/types/struct_timeval.h>
#include <sys/time.h>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <string>
#include <vector>
#include <random>

static const int FPS_LIMIT = 50;
static const int BIRDCOUNT = 100;
static const float BIRDSCALE = 5.0;
static const float TURNING_SPEED = 2.0;
static const float URGENT_TURNING_SPEED = 4.0;
static const float FLOCKING_RANGE = 80;
static const float OBSTACLE_RANGE = 50.0;
static const float PERSONAL_SPACE = 20.0;
static const float wallx0 = 50.0;
static const float wallx1 = 650.0;
static const float wally0 = 50.0;
static const float wally1 = 650.0;

class Bird;

void drawTriangle(float x, float y, float thetaRad);
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius);
float distBetweenPoints(float ax, float ay, float bx, float by);
float distBetweenBirds(Bird& a, Bird& b);
void drawLineToNearestBird(Bird& bird);
void debugShowFlockingRange(Bird& bird);
std::vector<Bird> createRandomBirds(int n);
int indexOfMinimumFloatArray(float *arr);

#endif //MAIN_HPP