//bird.cpp
#include "bird.hpp"
#include "main.hpp"
#include <cmath>
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
float Bird::getRotx() {return cos(rot * PI / 180); }
float Bird::getRoty() {return sin(rot * PI / 180); }
void Bird::setRot(float newrot) { 
    newrot = fmod(newrot, 360);
    rot = newrot;
}
void Bird::setRot(float x, float y){
    rot = atan2(y, x) * 180 / PI;
}
void Bird::addRot (float num) { setRot(rot + num); }
void Bird::draw(){
    glColor3f(1,getDotProductWithDirectionVector(0, 1),getDotProductWithDirectionVector(0,-1));
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

std::vector<Bird*> Bird::getNearbyBirds() {
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
    //if (recordDist < FLOCKING_RANGE) std::cout << "dist: " << recordDist << "   winner: " << nearbyBirds[0] << std::endl;
    return nearbyBirds;
}
int Bird::getId() {return id;}
void Bird:: steerAwayFromWall(){
    collisionRisk = x < wallx0 + OBSTACLE_RANGE || x > wallx1 - OBSTACLE_RANGE 
        || y < wally0 + OBSTACLE_RANGE || y > wally1 - OBSTACLE_RANGE;
    if(collisionRisk){
        float centerpointx = (wallx1 - wallx0)/2;
        float centerpointy = (wally1 - wally0)/2;
        turnTowardsVector(centerpointx - x, centerpointy - y, URGENT_TURNING_SPEED);
    }
    

}
/* void Bird::steerAwayFromWall() { //Checks distance to all four walls and decides how to turn based on position
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
} */
void Bird::teleportOnWall() {
    if (x < wallx0) { x = wallx1 - 2; } //2 is just so it doesnt immediately get teleported back next frame
    if (x > wallx1) { x = wallx0 + 2; }
    if (y < wally0) { y = wally1 - 2; }
    if (y > wally1) { y = wally0 + 2; }
}

float Bird::getDotProductWithDirectionVector(float vx, float vy) {
    float vmagnitude = sqrt(vx * vx + vy *vy);
    //std::cout << "%f  dot  %f  = %f"<< cos(getRot() * PI/180)<< sin(getRot() * PI/180)<< (cos(getRot() * PI/180) * vx + sin(getRot() * PI/180) * vy)/vmagnitude << std::endl;
    return (getRotx() * vx + getRoty() * vy)/vmagnitude;
}

int signOf(float n){
    if(n > 0) return 1;
    else if (n < 0) return -1;
    else return 0;
}

float angleBetweenVectors(float x0, float y0, float x1, float y1){ 
    //radians
    float dot = x0 * x1 + y0 * y1;
    float mag0 = sqrt(x0 * x0 + y0 * y0);
    float mag1 = sqrt(x1 * x1 + y1 * y1);
    return acos(dot/(mag0 * mag1));
}

float signedAngleBetweenVectors(float x0, float y0, float x1, float y1){
    //radians
    float angle = angleBetweenVectors(x0, y0, x1, y1);
    float crossproduct = x0 * y1 - y0 * x1;
    return crossproduct > 0 ? angle : -angle;
}

void Bird::turnTowardsVector(float vx, float vy, float speed){
    //turn until my rotation vector is parallel to the given vector
    if (angleBetweenVectors(getRotx(), getRoty(), vx, vy) * 180/PI < speed){
        setRot(vx, vy);
    }
    else{
        addRot(speed * signOf(getRotx() * vy - getRoty() * vx)) ; //signOf(cross product maginitude)
    }
}

void Bird::steerTowardsCenterOfFlock(){
    //adding up all x and y values of all nearby birds to find average x and y to steer towards
    float xsum = x;
    float ysum = y;

    for(int i = 0; i < size(nearbyBirds); i++){
        xsum += nearbyBirds[i]->getx();
        ysum += nearbyBirds[i]->gety();
    }
    float avex = xsum / (size(nearbyBirds) + 1);
    float avey = ysum / (size(nearbyBirds) + 1); //+1 because im including myself
    turnTowardsVector(avex, avey, TURNING_SPEED);
}

bool Bird::risksCollision(){
    return collisionRisk;
}

void Bird::avoidEachOther(){
    Bird* nearestBird = pGetNearestBird();
    if  (nearestBird == nullptr) return;
    //probably should implement collisionRisk here but I dont want to because I would
    // have to make sure steerAwayFromWall() isn't overriding this.
    if (distBetweenBirds(*this, *nearestBird) < PERSONAL_SPACE){
        turnTowardsVector(x - nearestBird->getx(), y - nearestBird->gety(), URGENT_TURNING_SPEED);
    }
}