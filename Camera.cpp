#include "Camera.h"


int Camera::camPos = 0;
glm::mat4 Camera::viewingMatrix = glm::mat4(1.0);
glm::vec3 Camera::cameraPos(0, 0, 0);

Camera::Camera()
{
}

glm::mat4 Camera::cameraView(glm::vec3 pos, glm::vec3 rot, float angle)
{ 
	// pos is the position of the sub 
	// rot is the rotational vector of the sub
	// angle is the movement of the mouse in the x axis
	cameraPos = glm::vec3(0, 0, 0); //resets the camera pos for each run
	glm::vec3 cameraSet(0, 0, 0);	//sets the camera possition
	glm::vec3 temp(0, 0, 0);		// hows the camera possition while being changed
	glm::vec3 rad = glm::vec3((rot.x*3.1415f) / 180, (rot.y*3.1415f) / 180, 0); //the rotation vector of the sub turn into radiuns
	glm::quat rotationQuat = glm::quat(rad); //the rotation vector of the sub turned into quats
	glm::vec3 mousePointer(0,0,0); //vector that hold the mouse Possition
	glm::vec3 perOffset(0, 0, 1); 

	//periscope
	if (camPos == 3)
	{
		temp.y = pos.y;
		temp.x = pos.x;
		temp.z = pos.z;

		glm::mat4 rotmat(1.0); //resets 
		rotmat = glm::rotate(rotmat, -angle, glm::vec3(0, 1, 0)); 

		perOffset = glm::vec3(rotmat*glm::vec4(perOffset, 1.0));

		// sents position of camera
		cameraSet = temp + rotationQuat * glm::vec3(0, 4, -2);
		// sents what the camera is looking at
		mousePointer = temp + rotationQuat * (perOffset + glm::vec3(0, 4, -2));
		cameraPos = cameraSet;

		if (rot.x > -1 && rot.x < 90)
		{
			return glm::lookAt(cameraSet, mousePointer, glm::vec3(0, 1, 0));
		}
		else if (rot.x > 89 && rot.x < 270)
		{
			return glm::lookAt(cameraSet, mousePointer, glm::vec3(0, -1, 0));
		}
		else if (rot.x > 269 && rot.x < 361)
		{
			return glm::lookAt(cameraSet, mousePointer, glm::vec3(0, 1, 0));
		}
		
	}
	//top down
	else if(camPos == 2)
	{
		
		cameraSet.y = pos.y + 100;
		cameraSet.x = pos.x;
		cameraSet.z = pos.z;
		cameraPos = cameraSet;

		return glm::lookAt(cameraSet, pos, glm::vec3(0, 0, 1));
	}
	// over the sholder
	else if (camPos == 1)
	{
		
		temp.y = pos.y;
		temp.x = pos.x;
		temp.z = pos.z;

		// sents position of camera
		cameraSet = temp + rotationQuat * glm::vec3(0,0,-50);
		cameraPos = cameraSet;
		/*if (rot.x > -1 && rot.x < 90)
		{*/
			return glm::lookAt(cameraSet, pos, glm::vec3(0,1,0));
		/*}
		else if (rot.x > 89 && rot.x < 270)
		{
			return glm::lookAt(cameraSet, pos, glm::vec3(0,-1,0));
		}
		else if (rot.x > 269 && rot.x < 361)
		{
			return glm::lookAt(cameraSet, pos, glm::vec3(0, 1, 0));
		}*/
	}
	//facing sub
	else 
	{
		temp.y = pos.y;
		temp.x = pos.x;
		temp.z = pos.z;

		// sents position of camera
		cameraSet = temp + rotationQuat * glm::vec3(0, 0, 50);
		cameraPos = cameraSet;
		/*if (rot.x > -1 && rot.x < 90)
		{*/
			return glm::lookAt(cameraSet, pos, glm::vec3(0, 1, 0));
		/*}
		else if (rot.x > 89 && rot.x < 270)
		{
			return glm::lookAt(cameraSet, pos, glm::vec3(0, -1, 0));
		}
		else if (rot.x > 269 && rot.x < 361)
		{
			return glm::lookAt(cameraSet, pos, glm::vec3(0, 1, 0));
		}*/
	}
}

// sets which can mode The sub is in
void Camera::changeCamera()
{
	if (camPos < 3)
	{
		camPos++;
	}
	else { camPos = 0; }
}

// sets the viewMatrix for all objects
void Camera::view(glm::vec3 pos, glm::vec3 rot,float angle)
{
	viewingMatrix = Camera::cameraView(pos,rot,angle);
}

glm::mat4 Camera::getViewMatrix()
{
	return viewingMatrix;
}

glm::vec3 Camera::getPos()
{
	return cameraPos;
}



Camera::~Camera()
{
}
