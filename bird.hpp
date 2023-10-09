//bird.hpp
#pragma once

#include <vector>

class Bird {
private:
	float x; // x position
	float y; // y position
	float rot; //rotation given in degrees [0, 360]
    float rotx; // x component of the rotation vector
    float roty; // y component of the rotation vectot
	int id;
	std::vector<Bird*> nearbyBirds; /* i think this should hold pointers to save memory because 
									I wont have multiple copies of birds in different places. All point back to the original birds vector 
									What if I use Bird& references instead?!?!*/
    bool collisionRisk;
public:
    Bird(float _x, float _y, float _rot);
    float getx();
    void setx(float newx);
    float gety();
    void sety(float newy);
    float getRot();
    float getRotx();
    float getRoty();
    void setRot(float newrot);
    void setRot(float x, float y);
    void addRot (float num);
    void turnTowardsVector(float x, float y, float speed);
    void draw();
    void step(float distance);
    Bird* pGetNearestBird();
    std::vector<Bird*> getNearbyBirds();
    int getId();
    void steerAwayFromWall();
    void teleportOnWall();
    float getDotProductWithDirectionVector(float vx, float vy);
    void steerTowardsCenterOfFlock();
    bool risksCollision();
    void avoidEachOther();
};

extern std::vector<Bird> birds; 

