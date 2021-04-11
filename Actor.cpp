#include "Actor.h"



Actor::Actor()
{
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	forwardVelocity = glm::vec3(0);
}

Actor::Actor(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	forwardVelocity = glm::vec3(0);
}


void Actor::loadObj(char* obj,bool centrePoint)
{
	std::cout << " loading model " << std::endl;
	if (objLoader.loadModel(obj, model))//returns true if the model is loaded, puts the model in the model parameter
	{
		std::cout << " model loaded " << std::endl;
		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		if (centrePoint)
		{
			model.calcCentrePoint();
			model.centreOnZero();
		}

		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


											 //turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.initDrawElements();
		model.initVBO(myShader);
		model.deleteVertexFaceData();

	}
	else
	{
		std::cout << " model failed to load " << std::endl;
	}
}

void Actor::draw()
{
	forwardVelocity = glm::vec3(0);
	glUseProgram(myShader->handle());
	
	//adds translation and rotation of sub to its model
	glm::mat4 trans = glm::translate(glm::mat4(1.0), pos);
	ModelMatrix = trans * glm::toMat4(rotationQuat);

	// used by sub to work out where is front is
	forwardVelocity = glm::vec3(ModelMatrix[2][0], ModelMatrix[2][1], ModelMatrix[2][2]);
	//v heading direction ModelMatrix[2][0], ModelMatrix[2][1], ModelMatrix[2][2]
	

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	viewingMatrix = Camera::getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);


	//DRAW THE MODEL
	ModelViewMatrix = viewingMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	model.drawElementsUsingVBO(myShader);
	//model.drawOctreeLeaves(myShader);
}

void Actor::updateTransform(float xinc, float yinc, float zinc)
{
	glm::mat4 matrixX, matrixXY;

	//rotation about the local x axis
	q = glm::angleAxis(xinc, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	matrixX = glm::mat4_cast(q) * objectRotation;

	//EXAMPLE FOR ACCESSING USING A 1D array
	const float *pSource = (const float*)glm::value_ptr(matrixX);
	//rotation about the local y axis
	q = glm::angleAxis(yinc, glm::vec3(pSource[4], pSource[5], pSource[6]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//EXAMPLE ACCESSING WITH 2D GLM structure.
	//rotation about the local z axis
	q = glm::angleAxis(zinc, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	objectRotation = glm::mat4_cast(q) * matrixXY;
}

void Actor::shader(Shader* shader)
{
	myShader = shader;
}

void Actor::projection(glm::mat4 matrix)
{

	ProjectionMatrix = matrix;

}

void Actor::newPos(glm::vec3 newPos)
{
	pos = newPos;
}

bool Actor::collisions(glm::vec3 front)
{
	//passes forward point verables into the model
	float x = 0, y = 0, z = 0;
	x = front.x;
	y = front.y;
	z = front.z;
	bool col = false;

	col = model.collide(x, y, z);

	return col;
}

glm::vec3 Actor::getForwardVelocity()
{
	return forwardVelocity;
}

glm::vec3 Actor::getPos()
{
	return pos;
}

Actor::~Actor()
{
}
