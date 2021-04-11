#include "Character.h"



Character::Character()
{
	pos.x = 0;
	pos.y = -10;
	pos.z = 0;
	rot.x = 0;
	rot.y = 0;
	rot.z = 0;
	forwardVec = glm::vec3(0.0f,0.0f, 0.0f);
	waterLevel = false;
}

void Character::move(float leftRight, float upDown, float forward)
{	
	forwardVec = glm::vec3(0.0f, 0.0f, 0.0f); //resets camera position
	glm::vec3 cameraPos = Camera::getPos(); // find camera position

	rot.x = rot.x + -upDown;
	if (rot.x < -45) // stops sub from rotation all the way round
	{
		rot.x = -45;
	}
	else if (rot.x > 45)
	{
		rot.x = 45;
	}

	rot.y = rot.y + leftRight;
	if (rot.y > 360) // sets left right turn to be 0 to 360 for easier understanding
	{
		rot.y = 0;
	}
	else if (rot.y < 0)
	{
		rot.y = 360;
	}

	if (pos.y >  -2.5) // surface of the sea stop the sub from flying
	{
		pos.y = -2.5;
		if (rot.x < 0)
		{
			rot.x = 0;
		}
		
	}

	if (cameraPos.y >= -2.8) // used to get the right shader when above an bellow the water
	{
		waterLevel = true;
	}
	else 
	{
		waterLevel = false;
	}

	glm::vec3 rad = glm::vec3((rot.x*3.1415f) / 180, (rot.y*3.1415f) / 180, 0); 
	rotationQuat = glm::quat(rad); // create rotational vector
	
	forwardVec = rotationQuat * glm::vec3(0, 0, forward); //create forward vector
	pos += forwardVec; 

}

float Character::rotationalPosition(float rP, float move)
{
	rP = rP + move;
	if (rP < 0)
	{
		rP = 360;
	}
	else if (rP > 360)
	{
		rP = 0;
	}

	return rP;
}

void Character::moveBack()
{
	pos -= forwardVec;
}

glm::vec3 Character::getPos()
{
	return pos;
}

glm::vec3 Character::getRot()
{
	return rot;
}

bool Character::getwaterLevel()
{
	return waterLevel;
}

Character::~Character()
{
}
