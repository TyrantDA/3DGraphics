#pragma once
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "glm\gtx\quaternion.hpp"

#include "Camera.h"
class Actor
{
public:
	Actor();
	Actor(float x, float y, float z);
	void loadObj(char* obj, bool centrePoint);
	void draw();
	void updateTransform(float xinc, float yinc, float zinc);
	void shader(Shader* shader);
	void projection(glm::mat4 matrix);
	void newPos(glm::vec3 newPos);
	bool collisions(glm::vec3 front);
	glm::vec3 getForwardVelocity();
	glm::vec3 getPos();
	~Actor();

	glm::vec3 rot;
	glm::quat rotationQuat;
	

protected:
	ThreeDModel model;
	OBJLoader objLoader;
	Shader *myShader;
	//Material properties
	float Material_Ambient[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //change the last number
	float Material_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
	float Material_Shininess = 50;
	//Light Properties
	float Light_Ambient_And_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float LightPos[4] = { 0.0f, 1.0f, 0.0f, 0.0f };

	glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
	glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
	glm::mat4 ModelMatrix;
	glm::quat q;
	glm::mat4 objectRotation;
	glm::mat4 viewingMatrix;

	glm::vec3 pos;
	glm::vec3 forwardVelocity;
};

