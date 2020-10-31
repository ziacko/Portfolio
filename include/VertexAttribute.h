#pragma once

struct vertexAttribute_t
{
	//have to put these first for offsetof to work
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec4 biNormal;
	glm::vec4 indices;
	glm::vec4 weights;
	glm::vec2 uv;
	glm::vec2 uv2;

	vertexAttribute_t(glm::vec4 position, glm::vec4 normal, glm::vec4 tangent,
		glm::vec4 biTangent, glm::vec2 uv)
	{
		this->position = position;
		this->normal = normal;
		this->tangent = tangent;
		this->biNormal = biTangent;
		this->uv = uv;
		this->indices = glm::vec4(0);
		this->weights = glm::vec4(0);
		this->uv2 = glm::vec4(0);
	}

	vertexAttribute_t()
	{
		position = glm::vec4(0);
		color = glm::vec4(0);
		normal = glm::vec4(0);
		tangent = glm::vec4(0);
		biNormal = glm::vec4(0);
		indices = glm::vec4(0);
		weights = glm::vec4(0);
		uv = glm::vec2(0);
		uv2 = glm::vec2(0);
	}
};

enum vertexOffset
{
	position = offsetof(vertexAttribute_t, position),
	color = offsetof(vertexAttribute_t, color),
	normal = offsetof(vertexAttribute_t, normal),
	tangent = offsetof(vertexAttribute_t, tangent),
	biNormal = offsetof(vertexAttribute_t, biNormal),
	indices = offsetof(vertexAttribute_t, indices),
	weights = offsetof(vertexAttribute_t, weights),
	uv = offsetof(vertexAttribute_t, uv),
	uv2 = offsetof(vertexAttribute_t, uv2)
};