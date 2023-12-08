#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "graphics_headers.h"

class Light {

public:

	glm::vec4 m_globalAmbient;
	glm::vec4 m_lightAmbient;
	glm::vec4 m_lightDiffuse;
	glm::vec3 m_lightPosition;
	glm::vec3 m_lightPositionViewSpace;
	glm::vec4 m_lightSpecular;

	Light(glm::mat4 viewMatrix);
	Light(glm::mat4 viewMatrix, glm::vec4 lightAmb, glm::vec4 lightDiff, glm::vec4 lightSpec, glm::vec4 gAmb);
	void SetViewSpacePosition(glm::mat4 viewMatrix);
};
#endif