//bird.cpp
#include "bird.hpp"
#include "main.hpp"
#include <math.h>

static const float PI = 3.14159265359;
int highestBirdId = 0;
std::vector<Bird> birds; 

Bird::Bird(float _x, float _y, float _rot) 
    : x(_x), y(_y), rot(_rot) {
        id = highestBirdId++;
}
float Bird::getx() {return x;}
void Bird::setx(float newx) {x = newx;}
float Bird::gety() {return y;}
void Bird::sety(float newy) {y = newy;}
float Bird::getRot() {return rot;}
void Bird::setRot(float newrot) { 
    newrot = fmod(newrot, 360);
    rot = newrot;
}
void Bird::setRot(float x, float y){
    rot = atan2(y, x) * 180 / PI;
}
void Bird::addRot (float num) { setRot(rot + num); }
void Bird::draw(){
    glColor3f(abs(180-rot)/360, rot/360, 0);
    drawTriangle(x, y, rot);
}
void Bird::step(float distance){
    x += distance * cos(rot * PI/180);//cos is in rad, rot is in degrees
    y += distance * sin(rot * PI/180);
}

Bird* Bird::pGetNearestBird() { 
    if(size(nearbyBirds) > 0) return nearbyBirds[0];
        return nullptr;
}
void Bird::findNearbyBirds() {
    nearbyBirds.clear();
    float recordDist = MAXFLOAT;
    float dist = MAXFLOAT;

    for(int i = 0; i < size(birds); i++){
        if (this == &birds[i]) continue;
        dist = distBetweenBirds(*this, birds[i]);
        
        if (dist < FLOCKING_RANGE) {
            if(dist < recordDist){
                recordDist = dist;
                nearbyBirds.insert(nearbyBirds.begin(), &birds[i]);
            }
            else {
                nearbyBirds.emplace_back(&birds[i]);
            }
        }
    }
    if (recordDist < FLOCKING_RANGE) std::cout << "dist: " << recordDist << "   winner: " << nearbyBirds[0] << std::endl;
}
int Bird::getId() {return id;}
void Bird::steerAwayFromWall() {
    float distanceToWalls[] = {x - wallx0, wally1 - y, wallx1 - x, y - wally0}; // left, top, right, bottom
    int shortestDistanceIndex = indexOfMinimumFloatArray(distanceToWalls);
    if (distanceToWalls[shortestDistanceIndex] > OBSTACLE_RANGE) return;
    switch (shortestDistanceIndex) { //find which wall is closest
    //If my point is closer to the left vertex of this edge, turn right. Else turn left
        case 0:
            if (distBetweenPoints(x, y, wallx0, wally0) > distBetweenPoints(x, y, wallx0, wally1)){
                addRot(TURNING_SPEED);
            }
            else {addRot(-TURNING_SPEED); }
            break;
        case 1:
            if (distBetweenPoints(x, y, wallx0, wally1) > distBetweenPoints(x, y, wallx1, wally1)){
                addRot(TURNING_SPEED);
            }
            else {addRot(-TURNING_SPEED); }
            break;
        case 2:
            if (distBetweenPoints(x, y, wallx1, wally1) > distBetweenPoints(x, y, wallx1, wally0)){
                addRot(TURNING_SPEED);
            }
            else {addRot(-TURNING_SPEED); }
            break;
        case 3:
            if (distBetweenPoints(x, y, wallx1, wally0) > distBetweenPoints(x, y, wallx0, wally0)){
                addRot(TURNING_SPEED);
            }
            else {addRot(-TURNING_SPEED); }
            break;
    }
}
void Bird::teleportOnWall() {
    if (x < wallx0) { x = wallx1 - 2; } //2 is just so it doesnt immediately get teleported back next frame
    if (x > wallx1) { x = wallx0 + 2; }
    if (y < wally0) { y = wally1 - 2; }
    if (y > wally1) { y = wally0 + 2; }
}