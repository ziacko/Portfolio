#pragma once
#include "VertexAttribute.h"
#define AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES false
#define NUM_BONES_PER_VEREX 4

struct boneTransforms_t
{
	std::vector<glm::mat4> finalTransforms;
};

struct mesh_t
{
	std::string								name;

	std::vector<vertexAttribute_t>			vertices;
	std::vector<unsigned int>				indices;
	std::vector<texture>					textures;

	glm::vec4								diffuse;
	glm::vec4								specular;
	glm::vec4								ambient;
	glm::vec4								emissive;
	glm::vec4								reflective;

	unsigned int							vertexArrayHandle;
	unsigned int							vertexBufferHandle;
	unsigned int							indexBufferHandle;

	unsigned int							numBones;

	bool									isCollision;

	unsigned int							vertexOffset;

	mesh_t()
	{
		vertices = std::vector<vertexAttribute_t>();
		indices = std::vector<unsigned int>();
		textures = std::vector<texture>();

		diffuse = glm::vec4(0);
		specular = glm::vec4(0);
		ambient = glm::vec4(0);
		emissive = glm::vec4(0);
		reflective = glm::vec4(0);

		vertexArrayHandle = 0;
		vertexBufferHandle = 0;
		indexBufferHandle = 0;
		isCollision = false;
		numBones = 0;
	}

	mesh_t(std::vector<vertexAttribute_t> inVertices, std::vector<unsigned int> inIndices, std::vector<texture> inTextures) : 
		vertices(inVertices), indices(inIndices), textures(inTextures)
	{
		diffuse = glm::vec4(0);
		specular = glm::vec4(0);
		ambient = glm::vec4(0);
		emissive = glm::vec4(0);
		reflective = glm::vec4(0);

		vertexArrayHandle = 0;
		vertexBufferHandle = 0;
		indexBufferHandle = 0;
		isCollision = false;
		numBones = 0;
	}
};

class model_t
{
public:

	model_t(const char* resourcePath = "../../resources/models/SoulSpear.fbx", bool ignoreCollision = false, bool keepData = false)
	{
		this->resourcePath = resourcePath;
		position = glm::vec3(0.0f, -2.0f, -3.0f);
		scale = glm::vec3(1.0f);
		rotation = glm::vec3(0.0f);
		this->ignoreCollision = ignoreCollision;
		isPicked = false;
		this->keepData = keepData;
		skeletonFound = false;
		skeletonID = 0;
		skeletonNode = nullptr;
		boneIndex = 0;
	}

	glm::mat4 makeTransform()
	{
		//make a rotation matrix
		glm::mat4 euler = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
		euler[3] = glm::vec4(position.x, position.y, position.z, 1.0f);
		euler = glm::scale(euler, scale);
			
		return euler;
	}

	void loadModel()
	{
		assimpScene = aiImportFile(resourcePath.c_str(), aiProcessPreset_TargetRealtime_Fast); //change this to max quality later
		assert(assimpScene != nullptr);

		directory = resourcePath.substr(0, resourcePath.find_last_of('/'));

		//load the model into OpenGL
		ExtractAnimations(assimpScene);
		ExtractNode(assimpScene->mRootNode);

		glm::mat4 rootTransform = glm::mat4(0);
		globalInverse = glm::mat4(0);
		rootTransform[0].x = assimpScene->mRootNode->mTransformation.a1;
		rootTransform[0].y = assimpScene->mRootNode->mTransformation.b1;
		rootTransform[0].z = assimpScene->mRootNode->mTransformation.c1;
		rootTransform[0].w = assimpScene->mRootNode->mTransformation.d1;

		rootTransform[1].x = assimpScene->mRootNode->mTransformation.a2;
		rootTransform[1].y = assimpScene->mRootNode->mTransformation.b2;
		rootTransform[1].z = assimpScene->mRootNode->mTransformation.c2;
		rootTransform[1].w = assimpScene->mRootNode->mTransformation.d2;

		rootTransform[2].x = assimpScene->mRootNode->mTransformation.a3;
		rootTransform[2].y = assimpScene->mRootNode->mTransformation.b3; //why is some of this garbled?
		rootTransform[2].z = assimpScene->mRootNode->mTransformation.c3;
		rootTransform[2].w = assimpScene->mRootNode->mTransformation.d3;

		rootTransform[3].x = assimpScene->mRootNode->mTransformation.a4;
		rootTransform[3].y = assimpScene->mRootNode->mTransformation.b4;
		rootTransform[3].z = assimpScene->mRootNode->mTransformation.c4;
		rootTransform[3].w = assimpScene->mRootNode->mTransformation.d4;
			
		globalInverse = glm::inverse(rootTransform);

		Evaluate("Character|Character|Take 001|BaseLayer", 1.f, false, 24, 0);
	}

	void BeginExtraction(aiNode* node, const aiScene* scene)
	{
		//so assuming the root node has children

	}

	void Evaluate(std::string animationName, float seconds, bool loop, float framesPerSec, unsigned int animIndex = 0)
	{
		//get anim ticks per second
		float ticksPS = assimpScene->mAnimations[animIndex]->mTicksPerSecond;
		if(ticksPS == 0.f)
		{
			//set standard ticks per second to 24 for now
			ticksPS = 24.0f;
		}

		float animDuration = assimpScene->mAnimations[animIndex]->mDuration / framesPerSec;

		float tickTime = seconds * ticksPS;
		float animTimeInTicks = fmod(tickTime, assimpScene->mAnimations[animIndex]->mDuration); //duration is animation frames

		float frameTime = 0.0f;

		if(loop)
		{
			frameTime = glm::max<float>(fmod(seconds, animDuration), 0);
		}

		else
		{
			frameTime = glm::min<float>(glm::max<float>(seconds, 0), animDuration);
		}

		frameTime *= ticksPS;

		//this hack should allow us to skip ahead a bit
		if(skeletonFound)
		{
			SearchNodeHeirarchy(frameTime, skeletonNode, glm::identity<glm::mat4>());
		}
		else
		{
			SearchNodeHeirarchy(frameTime, assimpScene->mRootNode, glm::identity<glm::mat4>());
		}

		//for(auto iter : meshes)
		{
			//ok by now all of the transforms should have been processed into finalTransforms
			boneBuffer.Update(gl_shader_storage_buffer, gl_dynamic_draw,
				sizeof(glm::mat4) * boneBuffer.data.finalTransforms.size(),
				boneBuffer.data.finalTransforms.data());
		}
	}

	void SearchNodeHeirarchy(float animationTime, aiNode* node, glm::mat4& parentTransform, unsigned int animIndex = 0)
	{
		std::string nodeName = node->mName.C_Str();
		aiAnimation* anim = assimpScene->mAnimations[animIndex];

		glm::mat4 trans = parentTransform;

		//need the aiNodeAnim from the current animation being accessed
		const aiNodeAnim* nodeAnimation = GetNodeAnimByName(nodeName, anim);
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		if(nodeAnimation != nullptr)
		{
			glm::vec3 euler = glm::eulerAngles(rotation);
			CalcInterpolatePRS(position, rotation, scale, animationTime, nodeAnimation);

			glm::mat4 P = glm::translate(glm::identity<glm::mat4>(), position);
			glm::mat4 R = glm::toMat4(rotation);
			glm::mat4 S = glm::scale(glm::identity<glm::mat4>(), scale);

			//assimp matrix transformations go scale, rotation then translation
			trans = S * R * P;

			//looks like we found the root animation node. tag it to save performance later
			if(!skeletonFound)
			{
				skeletonNode = node;
				skeletonFound = true;
			}
		}

		glm::mat4 globalTransform = parentTransform * trans;

		//for each bone, look for all the meshes that use this bone and update the final transform of that bone
		for (size_t meshIter = 0; meshIter < meshes.size(); meshIter++)
		{
			if (boneLookup.count(std::string(node->mName.C_Str())) > 0)
			{
				unsigned int boneIndex = boneLookup[node->mName.C_Str()];
				boneBuffer.data.finalTransforms[boneIndex] = globalTransform * rawTransforms[boneIndex];
			}
		}


		for(size_t nodeIter = 0; nodeIter < node->mNumChildren; nodeIter++)
		{
			SearchNodeHeirarchy(animationTime, node->mChildren[nodeIter], globalTransform);
		}
	}

	void CalcInterpolatePRS(glm::vec3& outPosition, glm::quat& outRotation, glm::vec3& outScale, float animationTime, const aiNodeAnim* nodeAnim)
	{

		bool shouldReturn = false;
		if(nodeAnim->mNumPositionKeys == 1)
		{
			outPosition = glm::vec3(nodeAnim->mPositionKeys[0].mValue.x, nodeAnim->mPositionKeys[0].mValue.y, nodeAnim->mPositionKeys[0].mValue.z);
			shouldReturn = true;
		}

		if (nodeAnim->mNumRotationKeys == 1)
		{
			outRotation = glm::quat(nodeAnim->mRotationKeys[0].mValue.x, nodeAnim->mRotationKeys[0].mValue.y, nodeAnim->mRotationKeys[0].mValue.z, nodeAnim->mRotationKeys[0].mValue.w);
			shouldReturn = true;
		}

		if (nodeAnim->mNumScalingKeys == 1)
		{
			outScale = glm::vec3(nodeAnim->mScalingKeys[0].mValue.x, nodeAnim->mScalingKeys[0].mValue.y, nodeAnim->mScalingKeys[0].mValue.z);
			shouldReturn = true;
		}

		if(shouldReturn)
		{
			return;
		}

		unsigned int positionIndex = GetPositionIndex(animationTime, nodeAnim);
		unsigned int rotationIndex = GetRotationIndex(animationTime, nodeAnim);
		unsigned int scaleIndex = GetScaleIndex(animationTime, nodeAnim);

		if (positionIndex < nodeAnim->mNumPositionKeys)
		{
			float deltaPositionTime = nodeAnim->mPositionKeys[positionIndex + 1].mTime - nodeAnim->mPositionKeys[positionIndex].mTime;
			float positionFactor = (animationTime - nodeAnim->mPositionKeys[positionIndex].mTime) / deltaPositionTime;
			assert(positionFactor >= 0.0f && positionFactor <= 1.0f);
			glm::vec3 startPosition = glm::vec3(nodeAnim->mPositionKeys[positionIndex].mValue.x, nodeAnim->mPositionKeys[positionIndex].mValue.y, nodeAnim->mPositionKeys[positionIndex].mValue.z);
			glm::vec3 endPosition = glm::vec3(nodeAnim->mPositionKeys[positionIndex + 1].mValue.x, nodeAnim->mPositionKeys[positionIndex + 1].mValue.y, nodeAnim->mPositionKeys[positionIndex + 1].mValue.z);

			//does this need to be normalized?
			outPosition = glm::lerp(startPosition, endPosition, positionFactor);
		}

		if (rotationIndex < nodeAnim->mNumRotationKeys + 1)
		{
			float deltaRotationTime = nodeAnim->mRotationKeys[rotationIndex + 1].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime;
			float rotationFactor = (animationTime - nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaRotationTime;
			assert(rotationFactor >= 0.0f && rotationFactor <= 1.0f);
			glm::quat startRotation = glm::quat(nodeAnim->mRotationKeys[rotationIndex].mValue.x, nodeAnim->mRotationKeys[rotationIndex].mValue.y, nodeAnim->mRotationKeys[rotationIndex].mValue.z, nodeAnim->mRotationKeys[rotationIndex].mValue.w);
			glm::quat endRotation = glm::quat(nodeAnim->mRotationKeys[rotationIndex + 1].mValue.x, nodeAnim->mRotationKeys[rotationIndex + 1].mValue.y, nodeAnim->mRotationKeys[rotationIndex + 1].mValue.z, nodeAnim->mRotationKeys[rotationIndex + 1].mValue.w);

			//does this need to be normalized?
			outRotation = glm::lerp(startRotation, endRotation, rotationFactor);
		}

		if (scaleIndex < nodeAnim->mNumScalingKeys + 1)
		{
			float deltaRotationTime = nodeAnim->mScalingKeys[scaleIndex + 1].mTime - nodeAnim->mScalingKeys[scaleIndex].mTime;
			float scaleFactor = (animationTime - nodeAnim->mScalingKeys[scaleIndex].mTime) / deltaRotationTime;
			assert(scaleFactor >= 0.0f && scaleFactor <= 1.0f);
			glm::vec3 startScale = glm::vec3(nodeAnim->mScalingKeys[scaleIndex].mValue.x, nodeAnim->mScalingKeys[scaleIndex].mValue.y, nodeAnim->mScalingKeys[scaleIndex].mValue.z);
			glm::vec3 endScale = glm::vec3(nodeAnim->mScalingKeys[scaleIndex + 1].mValue.x, nodeAnim->mScalingKeys[scaleIndex + 1].mValue.y, nodeAnim->mScalingKeys[scaleIndex + 1].mValue.z);

			//does this need to be normalized?
			outScale = glm::lerp(startScale, endScale, scaleFactor);
		}

	}

	int GetPositionIndex(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (size_t iter = 0; iter < nodeAnim->mNumRotationKeys - 1; iter++)
		{
			//if the time of the current key is the same as the passed in key, 
			if (animationTime < (float)nodeAnim->mPositionKeys[iter + 1].mTime)
			{
				return iter;
			}
		}
		return -1;
	}

	int GetRotationIndex(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (size_t iter = 0; iter < nodeAnim->mNumRotationKeys - 1; iter++)
		{
			//if the time of the current key is the same as the passed in key, 
			if (animationTime < (float)nodeAnim->mRotationKeys[iter + 1].mTime)
			{
				return iter;
			}
		}
		return -1;
	}

	int GetScaleIndex(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for(size_t iter = 0; iter < nodeAnim->mNumRotationKeys - 1; iter++)
		{
			//if the time of the current key is the same as the passed in key, 
			if(animationTime < (float)nodeAnim->mScalingKeys[iter + 1].mTime)
			{
				return iter;
			}
		}
		return -1;
	}

	const aiNodeAnim* GetNodeAnimByName(const std::string name, const aiAnimation* anim)
	{
		for(size_t iter = 0; iter < anim->mNumChannels; iter++)
		{
			//printf("%s \t %s\n", anim->mChannels[iter]->mNodeName.C_Str(), name.c_str());
			if(name.compare(anim->mChannels[iter]->mNodeName.C_Str()) == 0)
			{
				return anim->mChannels[iter];
			}
		}
		return nullptr;
	}

	void ExtractNode(aiNode* node)
	{
		//extract each mesh
		for (size_t iter = 0; iter < node->mNumMeshes; iter++)
		{
			ExtractMesh(assimpScene->mMeshes[node->mMeshes[iter]]);
		}

		//if the mesh has children, use recursion
		for (size_t iter = 0; iter < node->mNumChildren; iter++)
		{
			ExtractNode(node->mChildren[iter]);
		}
	}

	void ExtractAnimations(const aiScene* scene)
	{
		if (scene->HasAnimations())
		{
			for (size_t iter = 0; iter < scene->mNumAnimations; iter++)
			{
				/*printf("name %s | \t", scene->mAnimations[iter]->mName.C_Str());
				printf("duration %f \t", scene->mAnimations[iter]->mDuration);
				printf("ticks per sec %f", scene->mAnimations[iter]->mTicksPerSecond);
				printf("\n");*/
			}
		}
	}

	void ExtractMesh(aiMesh* mesh)
	{
		mesh_t newMesh;
		newMesh.name = mesh->mName.C_Str();
		std::vector<vertexAttribute_t> verts;
		std::vector<texture> textures;

		//if ignore collision is on, skip the node with the prefix UCX_
		std::string ue4String = "UCX_";
		std::string nodeName = mesh->mName.C_Str();
		newMesh.isCollision = (nodeName.substr(0, 4).compare(ue4String) == 0);
		std::vector<glm::vec4> positions;
		for (unsigned int vertexIter = 0; vertexIter < mesh->mNumVertices; vertexIter++)
		{
			glm::vec4 normal = glm::vec4(0, 0, 0, 0);
			glm::vec4 tangent = glm::vec4(0, 0, 0, 0);
			glm::vec4 biTangent = glm::vec4(0, 0, 0, 0);
			glm::vec4 color = glm::vec4(0);
			GLuint boneIndex = 0;
			GLfloat weight = 0.0f;
			glm::vec2 uv = glm::vec2(0, 0);

			if (mesh->mVertices != nullptr)
			{
				positions.push_back(glm::vec4(mesh->mVertices[vertexIter].x,
					mesh->mVertices[vertexIter].y, mesh->mVertices[vertexIter].z, 1.0f));
			}

			if (mesh->HasNormals())
			{
				normal = glm::vec4(mesh->mNormals[vertexIter].x,
					mesh->mNormals[vertexIter].y, mesh->mNormals[vertexIter].z, 1.0f);
			}

			if (mesh->HasTangentsAndBitangents())
			{
				tangent = glm::vec4(mesh->mTangents[vertexIter].x,
					mesh->mTangents[vertexIter].y, mesh->mTangents[vertexIter].z, 1.0f);

				biTangent = glm::vec4(mesh->mBitangents[vertexIter].x,
					mesh->mBitangents[vertexIter].y, mesh->mBitangents[vertexIter].z, 1.0f);
			}

			if(mesh->HasVertexColors(vertexIter))
			{
				//color = glm::vec4(mesh->mColors[vertexIter].x,
					//mesh->mBitangents[vertexIter].y, mesh->mBitangents[vertexIter].z, 1.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				uv = glm::vec2(mesh->mTextureCoords[0][vertexIter].x,
					mesh->mTextureCoords[0][vertexIter].y);
			}
			aiMaterial* mat = nullptr;
			//grab materials
			if (mesh->mMaterialIndex >= 0)
			{
				mat = assimpScene->mMaterials[mesh->mMaterialIndex];
			}

			vertexAttribute_t vertAttrib;
			vertAttrib.position = positions[vertexIter];
			vertAttrib.normal = normal;
			vertAttrib.tangent = tangent;
			vertAttrib.biNormal = biTangent;
			vertAttrib.uv = uv;

			verts.push_back(vertAttrib);
		}

		newMesh.vertices = verts;

		//go through bones, assign weights and vertex indices to vertex attribute
		if (mesh->HasBones())
		{
			ExtractBoneOffsets(mesh, verts, newMesh);
		}

		if(keepData)
		{
			posData.push_back(positions);
		}

		for (unsigned int faceIter = 0; faceIter < mesh->mNumFaces; faceIter++)
		{
			//only interested in triangle polygons for now
			if (mesh->mFaces[faceIter].mNumIndices == 3)
			{
				newMesh.indices.push_back(mesh->mFaces[faceIter].mIndices[0]);
				newMesh.indices.push_back(mesh->mFaces[faceIter].mIndices[1]);
				newMesh.indices.push_back(mesh->mFaces[faceIter].mIndices[2]);
			}
		}

		//extract the material textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = assimpScene->mMaterials[mesh->mMaterialIndex];

			aiColor3D diffuse;
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			newMesh.diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.0f);

			aiColor3D specular;
			mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			newMesh.specular = glm::vec4(specular.r, specular.g, specular.b, 1.0f);

			aiColor3D ambient;
			mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			newMesh.ambient = glm::vec4(ambient.r, ambient.g, ambient.b, 1.0f);

			aiColor3D emissive;
			mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
			newMesh.emissive = glm::vec4(emissive.r, emissive.g, emissive.b, 1.0f);

			aiColor3D reflective;
			mat->Get(AI_MATKEY_COLOR_REFLECTIVE, reflective);
			newMesh.reflective = glm::vec4(reflective.r, reflective.g, reflective.b, 1.0f);

			std::vector<texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, texture::textureType_t::diffuse, "diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<texture> normalMaps = loadMaterialTextures(mat, aiTextureType_NORMALS, texture::textureType_t::normal, "normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, texture::textureType_t::specular, "specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<texture> heightMaps = loadMaterialTextures(mat, aiTextureType_HEIGHT, texture::textureType_t::height, "height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			std::vector<texture> roughMaps = loadMaterialTextures(mat, aiTextureType_SHININESS, texture::textureType_t::roughness, "roughness");
			textures.insert(textures.end(), roughMaps.begin(), roughMaps.end());
		}

		glGenBuffers(1, &newMesh.vertexBufferHandle);
		glGenBuffers(1, &newMesh.indexBufferHandle);
		glGenVertexArrays(1, &newMesh.vertexArrayHandle);

		glBindVertexArray(newMesh.vertexArrayHandle);
		glBindBuffer(gl_array_buffer, newMesh.vertexBufferHandle);
		glBufferData(gl_array_buffer, sizeof(vertexAttribute_t) * verts.size(), verts.data(), gl_static_draw);

		glBindBuffer(gl_element_array_buffer, newMesh.indexBufferHandle);
		glBufferData(gl_element_array_buffer, sizeof(unsigned int) * newMesh.indices.size(), newMesh.indices.data(), gl_static_draw);

		//might cause more issues than prevent
		unsigned int attribID = 0;

		glEnableVertexAttribArray(attribID);
		glVertexAttribBinding(attribID, 0);
		glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::position);
		glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::position);

		if(mesh->HasNormals())
		{
			glEnableVertexAttribArray(attribID);
			glVertexAttribBinding(attribID, 0);
			glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::normal);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::normal);
		}

		if(mesh->HasTangentsAndBitangents())
		{
			glEnableVertexAttribArray(attribID);
			glVertexAttribBinding(attribID, 0);
			glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::tangent);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::tangent);
			
			glEnableVertexAttribArray(attribID);
			glVertexAttribBinding(attribID, 0);
			glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::biNormal);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::biNormal);
		}
		
		if(mesh->HasBones())
		{
			//if there are skeletal animations, load up the animation indices and weights
			glEnableVertexAttribArray(attribID);
			glVertexAttribBinding(attribID, 0);
			glVertexAttribIFormat(attribID, 4, GL_UNSIGNED_INT, vertexOffset::boneIndex);
			glVertexAttribIPointer(attribID++, 4, GL_UNSIGNED_INT, sizeof(vertexAttribute_t), (char*)vertexOffset::boneIndex);
			
			glEnableVertexAttribArray(attribID);
			glVertexAttribBinding(attribID, 0);
			glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::weight);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::weight);
		}

		glEnableVertexAttribArray(attribID);
		glVertexAttribBinding(attribID, 0);
		glVertexAttribFormat(attribID, 2, GL_FLOAT, GL_FALSE, vertexOffset::uv);
		glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::uv);
		
		newMesh.vertices = std::move(verts);
		newMesh.textures = std::move(textures);

		glBindVertexArray(0);
		glBindBuffer(gl_array_buffer, 0);
		glBindBuffer(gl_element_array_buffer, 0);

		//this only works if using one giant vector as a mesh
		//CalculateMeshVertexOffset(newMesh);

		meshes.push_back(newMesh);
	}

	//note this is BEFORE this mesh is added to the mesh vector (only works if all vertices are thrown in a single mesh)
	void CalculateMeshVertexOffset(mesh_t& mesh)
	{
		if(meshes.size() == 0)
		{
			mesh.vertexOffset = 0;
			return;
		}
		else
		{
			//ok grab the vertex offset of the last mesh and add it's vertex list size
			mesh_t lastMesh = meshes[meshes.size() - 1];
			mesh.vertexOffset = lastMesh.vertexOffset + lastMesh.vertices.size();
		}
	}

	void ExtractBoneOffsets(aiMesh* mesh, std::vector<vertexAttribute_t>& verts, mesh_t& currentMesh) //current mesh is if we move data to the mesh
	{
		printf("mesh %s has %i bones \n", mesh->mName.C_Str(), mesh->mNumBones);
		//for each bone in the current mesh,
		for (size_t boneIter = 0; boneIter < mesh->mNumBones; boneIter++)
		{
			int boneIndex = 0;
			std::string boneName = mesh->mBones[boneIter]->mName.C_Str();

			//ok but how do we know if the bone data is relevant to the current submesh?
			/*bool shouldGrab = false;
			//if the other meshes already have the bone then skip
			for(auto iter : meshes)
			{
				if(iter.boneLookup.count(boneName) > 0)
				{
					//ok then we don't load this bone
					shouldGrab = false;
				}
				else
				{
					shouldGrab = true;
				}
			}*/

			//has this bone already been found? (redundant when each mesh links to every bone)
			if (boneLookup.find(boneName) == boneLookup.end())//&& shouldGrab)
			{
				boneIndex = currentMesh.numBones;
				currentMesh.numBones++;
				rawTransforms.push_back(glm::mat4(0.0f));
			}
			else
			{
				boneIndex = boneLookup[boneName];
			}

			{
				//add bone name and bone offset to their containers
				if(boneLookup.count(boneName) == 0)
				{
					boneLookup.emplace(boneName, boneIndex);
				}
				
				glm::mat4 boneTransform;
				boneTransform[0].x = mesh->mBones[boneIndex]->mOffsetMatrix.a1;
				boneTransform[0].y = mesh->mBones[boneIndex]->mOffsetMatrix.b1;
				boneTransform[0].z = mesh->mBones[boneIndex]->mOffsetMatrix.c1;
				boneTransform[0].w = mesh->mBones[boneIndex]->mOffsetMatrix.d1;

				boneTransform[1].x = mesh->mBones[boneIndex]->mOffsetMatrix.a2;
				boneTransform[1].y = mesh->mBones[boneIndex]->mOffsetMatrix.b2;
				boneTransform[1].z = mesh->mBones[boneIndex]->mOffsetMatrix.c2;
				boneTransform[1].w = mesh->mBones[boneIndex]->mOffsetMatrix.d2;

				boneTransform[2].x = mesh->mBones[boneIndex]->mOffsetMatrix.a3;
				boneTransform[2].y = mesh->mBones[boneIndex]->mOffsetMatrix.b3;
				boneTransform[2].z = mesh->mBones[boneIndex]->mOffsetMatrix.c3;
				boneTransform[2].w = mesh->mBones[boneIndex]->mOffsetMatrix.d3;

				boneTransform[3].x = mesh->mBones[boneIndex]->mOffsetMatrix.a4;
				boneTransform[3].y = mesh->mBones[boneIndex]->mOffsetMatrix.b4;
				boneTransform[3].z = mesh->mBones[boneIndex]->mOffsetMatrix.c4;
				boneTransform[3].w = mesh->mBones[boneIndex]->mOffsetMatrix.d4; //am i copying this matrix correctly?
				rawTransforms[boneIndex] = boneTransform;
			}

			for (size_t weightsIter = 0; weightsIter < mesh->mBones[boneIndex]->mNumWeights; weightsIter++)
			{
				unsigned int boneID = boneLookup[boneName];
				aiVertexWeight weight = mesh->mBones[boneID]->mWeights[weightsIter];
				//for every weight grouping associated with this bone
				if (weight.mWeight < 0.0f)
				{
					printf("something broke \n");
				}
				UpdateBoneData(verts, weight, boneIter);
			}
		}
		boneBuffer.data.finalTransforms = std::vector<glm::mat4>(rawTransforms.size());
	}

	void UpdateBoneData(std::vector<vertexAttribute_t>& verts, aiVertexWeight& const weight, unsigned int boneIndex)
	{
		for (size_t iter = 0; iter < 4; iter++)
		{
			//for all 4 vertex attribute slots check if the current vertex weights are 0. if they are, fill in that data
			unsigned int vertexID = weight.mVertexId;
			if (verts[vertexID].weight[iter] == 0.0f)
			{
				verts[vertexID].boneIndex[iter] = boneIndex;
				verts[vertexID].weight[iter] = weight.mWeight;
				return; //leave early to prevent the entire set of weights to be written to
			}
		}
	}

	void BindBoneTransforms(unsigned int meshID, unsigned int uniformSlot)
	{
		boneBuffer.BindToSlot(uniformSlot, gl_shader_storage_buffer);
	}

	std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType texType, texture::textureType_t inTexType, std::string uniformName)
	{
		std::vector<texture> textures;

		for(size_t iter = 0; iter < mat->GetTextureCount(texType); iter++)
		{
			aiString str;
			mat->GetTexture(texType, iter, &str);
			std::string temp = str.C_Str();

			std::string shorter = temp.substr(temp.find_last_of('\\\\') + 1);
			std::string localPath = directory + '/' + shorter;

			bool skip = false;

			for (size_t j = 0; j < loadedTextures.size(); j++)
			{
				if (loadedTextures[j].GetFilePath().compare(localPath) == 0)
				{
					textures.push_back(loadedTextures[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				//textureDescriptor texDesc;
				//texDesc.internalFormat = GL_RGBA;
				//texDesc.target = gl_texture_2d;


				texture newTex(localPath, inTexType, uniformName);
				newTex.LoadTexture();
				textures.push_back(newTex);
				loadedTextures.push_back(newTex);
			}
		}
		return textures;
	}

	/*std::vector<glm::vec4> GetMeshPosData(unsigned int meshID)
	{
		std::vector<glm::vec4> posData;
		aiMesh* mesh = assimpScene->mMeshes[node->mMeshes[iter]];
		//if ignore collision is on, skip the node with the prefix UCX_
		std::string ue4String = "UCX_";
		std::string nodeName = mesh->mName.C_Str();
		bool isCollision = (nodeName.substr(0, 4).compare(ue4String) == 0);

		for (unsigned int vertexIter = 0; vertexIter < mesh->mNumVertices; vertexIter++)
		{
			if (mesh->mVertices != nullptr)
			{
				posData.push_back(glm::vec4(mesh->mVertices[vertexIter].x,
					mesh->mVertices[vertexIter].y, mesh->mVertices[vertexIter].z, 1.0f));
			}
		}
	}*/

	const aiScene*							assimpScene;
	std::string								resourcePath;
	std::vector<mesh_t>						meshes;
	std::string								directory;

	bool									isGUIActive;

	aiNode*									skeletonNode;
	bool									skeletonFound;
	unsigned int							skeletonID;
	unsigned int							boneIndex;

	glm::vec3								position;
	glm::vec3								scale;
	glm::vec3								rotation;

	std::vector<texture>					loadedTextures;
	std::vector<std::vector<glm::vec4>>		posData;

	//animation data
	std::vector<glm::mat4>					rawTransforms;
	std::map<std::string, unsigned int>		boneLookup;
	bufferHandler_t<boneTransforms_t>		boneBuffer;
	glm::mat4 globalInverse;

	//change these from std::vector to map?

	bool									ignoreCollision;
	bool									isPicked;
	bool									keepData;
};
