#include "GameObject.h"

GameObject::GameObject(GLuint _program, Model _model, LightNode* lightNodes):modelObj(_model), lightNodes(lightNodes)
{
	programID = _program;
}

void GameObject::SetupTransform(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
	Position = _position;
	Scale = _scale;
	EulerRotation = _rotation;
	QRotation = glm::quat(EulerRotation);
}

GameObject* GameObject::CreateGameObject(std::string path, LightNode* lightNodes)
{
	Model goModel = Model(path);

	GLuint goShader = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
	GameObject* gameObj = new GameObject(goShader, goModel, lightNodes);

	gameObj->SetupTransform(glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0, 0, 0));

	return gameObj;
}

void GameObject::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPosWorld)
{
	glUseProgram(programID);
	
	glm::quat Rot = QRotation;

	if (ModelNeedsRotating) {
		Rot = Rot * modelStartRot;
	}

	//mvp calc. model will be at origin 
	glm::mat4 model = glm::translate(glm::mat4(1.0f), Position) * glm::toMat4(Rot) * glm::scale(glm::mat4(1.0f), Scale);
	glm::mat4 mvp = projection * view * model;


	//used to transform normal to world space and and keep correct orientation
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

	GLuint mvpID = glGetUniformLocation(programID, "MVP");
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &model[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(programID, "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	glUniform3fv(glGetUniformLocation(programID, "viewPos"), 1, &viewPosWorld[0]);
	
	glUniform3fv(glGetUniformLocation(programID, "lightColour"), 1, &lightNodes[0].lightColour[0]);

	glUniform1f(glGetUniformLocation(programID, "lightPower"), lightNodes[0].lightPower);

	glUniform3fv(glGetUniformLocation(programID, "LightPosition_worldspace"), 1, &lightNodes[0].lightPosition[0]);

	modelObj.Draw(programID);
}

void GameObject::Translate(glm::vec3 translation)
{
	Position += translation;
}

void GameObject::Rotate(glm::vec3 axis, float rotation)
{
	glm::quat rotQuaternion = glm::quat(axis * glm::radians(rotation));
	QRotation = rotQuaternion * QRotation;
}

void GameObject::Rotation(glm::vec3 eulerRotation)
{
	EulerRotation = eulerRotation;
	QRotation = glm::quat(EulerRotation);
}

glm::vec3 GameObject::Forward()
{
	return glm::normalize(glm::rotate(QRotation, glm::vec3(0, 0, 1)));
}

glm::vec3 GameObject::Right()
{
	return glm::normalize(glm::rotate(QRotation, glm::vec3(-1, 0, 0)));
}

glm::vec3 GameObject::Up()
{
	return glm::normalize(glm::rotate(QRotation, glm::vec3(0, 1, 0)));
}

void GameObject::SetModelStartRotation(glm::vec3 eulerRotation)
{
	ModelNeedsRotating = true;
	modelStartRot = glm::quat(eulerRotation);

}


