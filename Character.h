#pragma once
#include "Actor.h"
class Character : public Actor
{
public:
	Character();
	void move(float leftRight, float upDown, float forward);
	float rotationalPosition(float rP, float move);
	void moveBack();
	glm::vec3 getPos();
	glm::vec3 getRot();
	bool getwaterLevel();

	~Character();

private:

	glm::vec3 forwardVec;
	bool waterLevel;
};

