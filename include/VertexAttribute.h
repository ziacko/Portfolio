#pragma once

struct vertexAttribute_t
{
	//have to put these first for offsetof to work
	glm::vec4	position;
	glm::vec4	color;
	glm::vec4	normal;
	glm::vec4	tangent;
	glm::vec4	biNormal;
	glm::ivec4	boneIndex;
	glm::vec4	weight;
	glm::vec2	uv;
	glm::vec2	uv2;


	vertexAttribute_t(glm::vec4 position, glm::vec4 normal, glm::vec4 tangent,
		glm::vec4 biTangent, glm::vec2 uv)
	{
		this->position = position;
		this->normal = normal;
		this->tangent = tangent;
		this->biNormal = biTangent;
		this->uv = uv;
		this->boneIndex = glm::ivec4(0);
		this->weight = glm::vec4(0.0f);
		this->uv2 = glm::vec2(0);
	}

	vertexAttribute_t()
	{
		position = glm::vec4(0);
		color = glm::vec4(0);
		normal = glm::vec4(0);
		tangent = glm::vec4(0);
		biNormal = glm::vec4(0);
		boneIndex = glm::ivec4(0);
		weight = glm::vec4(0.0f);
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
	boneIndex = offsetof(vertexAttribute_t, boneIndex),
	weight = offsetof(vertexAttribute_t, weight),
	uv = offsetof(vertexAttribute_t, uv),
	uv2 = offsetof(vertexAttribute_t, uv2),

};