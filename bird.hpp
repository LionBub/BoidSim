//bird.hpp
#pragma once

#include <vector>

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
    Bird(float _x, float _y, float _rot);
    float getx();
    void setx(float newx);
    float gety();
    void sety(float newy);
    float getRot();
    void setRot(float newrot);
    void setRot(float x, float y);
    void addRot (float num);
    void draw();
    void step(float distance);
    Bird* pGetNearestBird();
    void findNearbyBirds();
    int getId();
    void steerAwayFromWall();
    void teleportOnWall();
};

extern std::vector<Bird> birds; 

