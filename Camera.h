#pragma once
#include <iostream>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
static class Camera
{
public:
	
	Camera();
	static glm::mat4 cameraView(glm::vec3 pos, glm::vec3 rot, float angle);
	static void changeCamera();
	static void view(glm::vec3 pos, glm::vec3 rot, float angle);
	static glm::mat4 getViewMatrix();
	static glm::vec3 getPos();
	~Camera();
private:
	static int camPos;
	static glm::mat4 viewingMatrix;
	static glm::vec3 cameraPos;
};

