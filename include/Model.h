#pragma once
#include "VertexAttribute.h"
#define AI_CONFIG_IMPORT_REMOVE_EMPTY_BONES false
#define NUM_BONES_PER_VEREX 4

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

enum VB_TYPES {
	INDEX_BUFFER,
	POS_VB,
	NORMAL_VB,
	TEXCOORD_VB,
	BONE_VB,
	NUM_VBs
};

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

	bool									isCollision;

	unsigned int							vertexOffset;
	unsigned int							indexOffset;

	unsigned int							numVertices;
	unsigned int							numIndices;

	unsigned int							vertexBufferHandle;
	unsigned int							indexBufferHandle;

	mesh_t()
	{
		textures = std::vector<texture>();

		diffuse = glm::vec4(0);
		specular = glm::vec4(0);
		ambient = glm::vec4(0);
		emissive = glm::vec4(0);
		reflective = glm::vec4(0);

		vertexArrayHandle = 0;
		isCollision = false;
		vertexOffset = 0;
		indexOffset = 0;
	}

	mesh_t(std::vector<vertexAttribute_t> inVertices, std::vector<unsigned int> inIndices, std::vector<texture> inTextures) : 
		textures(inTextures)
	{
		diffuse = glm::vec4(0);
		specular = glm::vec4(0);
		ambient = glm::vec4(0);
		emissive = glm::vec4(0);
		reflective = glm::vec4(0);

		vertexArrayHandle = 0;
		isCollision = false;
		vertexOffset = 0;
		indexOffset = 0;
	}
};

#define NUM_BONES_PER_VEREX 4
struct BoneInfo
{
	glm::mat4 BoneOffset;
	glm::mat4 FinalTransformation;

	BoneInfo()
	{
		BoneOffset = glm::mat4(0);
		FinalTransformation = glm::mat4(0);
	}
};

struct MeshEntry {
	MeshEntry()
	{
		NumIndices = 0;
		BaseVertex = 0;
		BaseIndex = 0;
		MaterialIndex = 0xFFFFFFFF;
	}

	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

struct VertexBoneData
{
	unsigned int IDs[4];
	float Weights[4];

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		ZERO_MEM(IDs);
		ZERO_MEM(Weights);
	}

	void AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
			if (Weights[i] == 0.0) {
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		assert(0);
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
		numBones = 0;
		hasBones = false;
		hasTangentsAndBiTangents = false;
		hasNormals = false;

		/*vertices = std::vector<vertexAttribute_t>();
		indices = std::vector<unsigned int>();
		numVertices = 0;
		numIndices = 0;*/

		m_NumBones = 0;
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
		assimpScene = aiImportFile(resourcePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality); //change this to max quality later
		assert(assimpScene != nullptr);
		
		m_GlobalInverseTransform = ConvertToGLM(assimpScene->mRootNode->mTransformation);
		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);


		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

		InitFromScene(assimpScene, resourcePath.c_str());









		directory = resourcePath.substr(0, resourcePath.find_last_of('/'));



		//load the model into OpenGL
		ExtractAnimations(assimpScene);
		ExtractNode(assimpScene->mRootNode);
		

		glm::mat4 rootTransform = ConvertToGLM(assimpScene->mRootNode->mTransformation);
		globalInverse = glm::inverse(rootTransform);

		//Evaluate("Take 001", 1.f, false, 24, 0);
	}

	void BeginExtraction(aiNode* node, const aiScene* scene)
	{
		//so assuming the root node has children
		ExtractNode(node);
		ExtractAnimations(scene);
	}

	void Evaluate(std::string animationName, float seconds, bool loop, float framesPerSec, unsigned int animIndex = 0)
	{
		if (assimpScene->mNumAnimations > 0)
		{
			//get anim ticks per second
			float ticksPS = assimpScene->mAnimations[animIndex]->mTicksPerSecond;
			if (ticksPS == 0.f)
			{
				//set standard ticks per second to 24 for now
				ticksPS = 24.0f;
			}

			float animDuration = assimpScene->mAnimations[animIndex]->mDuration / framesPerSec;

			float tickTime = seconds * ticksPS;
			float animTimeInTicks = fmod(tickTime, assimpScene->mAnimations[animIndex]->mDuration); //duration is animation frames

			float frameTime = 0.0f;

			if (loop)
			{
				frameTime = glm::max<float>(fmod(seconds, animDuration), 0);
			}

			else
			{
				frameTime = glm::min<float>(glm::max<float>(seconds, 0), animDuration);
			}

			frameTime *= ticksPS;

			//this hack should allow us to skip ahead a bit
			if (skeletonFound)
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
	}

	void SearchNodeHeirarchy(float animationTime, aiNode* node, const glm::mat4& parentTransform, unsigned int animIndex = 0)
	{
		std::string nodeName = node->mName.C_Str();
		aiAnimation* anim = assimpScene->mAnimations[animIndex];

		glm::mat4 trans = ConvertToGLM(node->mTransformation);

		//need the aiNodeAnim from the current animation being accessed
		const aiNodeAnim* nodeAnimation = animNodeNames[nodeName];
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
			//trans = S * R * P;
			trans = P * R * S;

			//looks like we found the root animation node. tag it to save performance later
			if(!skeletonFound)
			{
				skeletonNode = node;
				skeletonFound = true;
			}
		}

		glm::mat4 globalTransform = parentTransform * trans;

		//for each bone, look for all the meshes that use this bone and update the final transform of that bone
		//for (size_t meshIter = 0; meshIter < meshes.size(); meshIter++)
		{
			if (boneLookup.count(std::string(node->mName.C_Str())) > 0)
			{
				unsigned int boneIndex = boneLookup[node->mName.C_Str()];
				boneBuffer.data.finalTransforms[boneIndex] = globalInverse * globalTransform * rawTransforms[boneIndex];
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

		int positionIndex = GetPositionIndex(animationTime, nodeAnim);
		int rotationIndex = GetRotationIndex(animationTime, nodeAnim);
		int scaleIndex = GetScaleIndex(animationTime, nodeAnim);

		if (positionIndex > -1 && positionIndex + 1 < nodeAnim->mNumPositionKeys)
		{
			float deltaPositionTime = nodeAnim->mPositionKeys[positionIndex + 1].mTime - nodeAnim->mPositionKeys[positionIndex].mTime;
			float positionFactor = (animationTime - nodeAnim->mPositionKeys[positionIndex].mTime) / deltaPositionTime;
			assert(positionFactor >= 0.0f && positionFactor <= 1.0f);
			glm::vec3 startPosition = glm::vec3(nodeAnim->mPositionKeys[positionIndex].mValue.x, nodeAnim->mPositionKeys[positionIndex].mValue.y, nodeAnim->mPositionKeys[positionIndex].mValue.z);
			glm::vec3 endPosition = glm::vec3(nodeAnim->mPositionKeys[positionIndex + 1].mValue.x, nodeAnim->mPositionKeys[positionIndex + 1].mValue.y, nodeAnim->mPositionKeys[positionIndex + 1].mValue.z);

			glm::vec3 delta = endPosition - startPosition;
			outPosition = startPosition + positionFactor * delta;

			//does this need to be normalized?
			outPosition = glm::lerp(startPosition, endPosition, positionFactor);
		}

		if (rotationIndex > -1 && rotationIndex < nodeAnim->mNumRotationKeys + 1)
		{
			float deltaRotationTime = nodeAnim->mRotationKeys[rotationIndex + 1].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime;
			float rotationFactor = (animationTime - nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaRotationTime;
			assert(rotationFactor >= 0.0f && rotationFactor <= 1.0f);
			glm::quat startRotation = glm::quat(nodeAnim->mRotationKeys[rotationIndex].mValue.x, nodeAnim->mRotationKeys[rotationIndex].mValue.y, nodeAnim->mRotationKeys[rotationIndex].mValue.z, nodeAnim->mRotationKeys[rotationIndex].mValue.w);
			glm::quat endRotation = glm::quat(nodeAnim->mRotationKeys[rotationIndex + 1].mValue.x, nodeAnim->mRotationKeys[rotationIndex + 1].mValue.y, nodeAnim->mRotationKeys[rotationIndex + 1].mValue.z, nodeAnim->mRotationKeys[rotationIndex + 1].mValue.w);

			aiQuaternion outQuat;
			aiQuaternion::Interpolate(outQuat, nodeAnim->mRotationKeys[rotationIndex].mValue, nodeAnim->mRotationKeys[rotationIndex + 1].mValue, rotationFactor);
			outQuat = outQuat.Normalize();
			
			//i hope this works
			outRotation.x = outQuat.x;
			outRotation.y = outQuat.y;
			outRotation.z = outQuat.z;
			outRotation.w = outQuat.w;

			//does this need to be normalized?
			//outRotation = glm::lerp(startRotation, endRotation, rotationFactor);
		}

		if (scaleIndex > -1 && scaleIndex < nodeAnim->mNumScalingKeys + 1)
		{
			float deltaRotationTime = nodeAnim->mScalingKeys[scaleIndex + 1].mTime - nodeAnim->mScalingKeys[scaleIndex].mTime;
			float scaleFactor = (animationTime - nodeAnim->mScalingKeys[scaleIndex].mTime) / deltaRotationTime;
			assert(scaleFactor >= 0.0f && scaleFactor <= 1.0f);
			glm::vec3 startScale = glm::vec3(nodeAnim->mScalingKeys[scaleIndex].mValue.x, nodeAnim->mScalingKeys[scaleIndex].mValue.y, nodeAnim->mScalingKeys[scaleIndex].mValue.z);
			glm::vec3 endScale = glm::vec3(nodeAnim->mScalingKeys[scaleIndex + 1].mValue.x, nodeAnim->mScalingKeys[scaleIndex + 1].mValue.y, nodeAnim->mScalingKeys[scaleIndex + 1].mValue.z);

			glm::vec3 delta = endScale - startScale;
			outScale = startScale + scaleFactor * delta;

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
				animationNames.emplace(scene->mAnimations[iter]->mName.C_Str(), scene->mAnimations[iter]);
				ExtractAnimationNodes(scene->mAnimations[iter]);
			}
		}
	}

	void ExtractAnimationNodes(const aiAnimation* anim)
	{
		for (size_t iter = 0; iter < anim->mNumChannels; iter++)
		{
			animNodeNames.emplace(anim->mChannels[iter]->mNodeName.C_Str(), anim->mChannels[iter]);
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
				hasNormals = true;
				normal = glm::vec4(mesh->mNormals[vertexIter].x,
					mesh->mNormals[vertexIter].y, mesh->mNormals[vertexIter].z, 1.0f);
			}

			if (mesh->HasTangentsAndBitangents())
			{
				hasTangentsAndBiTangents = true;
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

		//vertices.insert(vertices.end(), verts.begin(), verts.end());
		//CalculateMeshVertexOffset(newMesh, vertices, mesh);
		newMesh.vertices = verts;
		//go through bones, assign weights and vertex indices to vertex attribute
		if (mesh->HasBones())
		{
			hasBones = true;
			ExtractBoneOffsets(mesh, newMesh, verts);
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

		glBindVertexArray(0);
		glBindBuffer(gl_array_buffer, 0);
		glBindBuffer(gl_element_array_buffer, 0);

		LoadIntoGL(mesh, newMesh);

		newMesh.textures = std::move(textures);

		meshes.push_back(newMesh);
	}

	//note this is BEFORE this mesh is added to the mesh vector (only works if all vertices are thrown in a single mesh)
	/*void CalculateMeshVertexOffset(mesh_t& mesh, const std::vector<vertexAttribute_t>& verts, const aiMesh* assimpMesh)
	{
		//if no meshes so far, set vtx and idx offsets to 0
		if(meshes.size() == 0)
		{
			mesh.vertexOffset = 0;
			mesh.indexOffset = 0;
		}
		else
		{
			//ok grab the vertex offset of the last mesh and add it's vertex list size
			mesh_t lastMesh = meshes[meshes.size() - 1];

			mesh.vertexOffset = numVertices;
			mesh.indexOffset = numIndices;
		}
		numVertices += assimpMesh->mNumVertices;
		numIndices += assimpMesh->mNumFaces * 3;
		mesh.numVertices = assimpMesh->mNumVertices;
		mesh.numIndices = assimpMesh->mNumFaces * 3;
	}*/

	void ExtractBoneOffsets(aiMesh* mesh, mesh_t& currentMesh, std::vector<vertexAttribute_t>& verts) //current mesh is if we move data to the mesh
	{
		printf("mesh %s has %i bones \n", mesh->mName.C_Str(), mesh->mNumBones);
		//for each bone in the current mesh,
		for (size_t boneIter = 0; boneIter < mesh->mNumBones; boneIter++)
		{
			int boneIndex = 0;
			std::string boneName = mesh->mBones[boneIter]->mName.C_Str();

			//add bone name and bone offset to their containers
			if(boneLookup.count(boneName) == 0)
			{
				boneIndex = numBones;
				numBones++;
				boneLookup.emplace(boneName, 0);

				glm::mat4 boneTransform = ConvertToGLM(mesh->mBones[boneIter]->mOffsetMatrix);
				rawTransforms.push_back(boneTransform);
				boneLookup[boneName] = boneIndex;
			}
			else
			{
				boneIndex = boneLookup[boneName];
			}

			//for each bone referenced in this submesh
			printf("OGLDev: this mesh %s uses bone %s with %i weights \n", mesh->mName.C_Str(), mesh->mBones[boneIter]->mName.C_Str(), mesh->mBones[boneIter]->mNumWeights);
			for (size_t weightsIter = 0; weightsIter < mesh->mBones[boneIter]->mNumWeights; weightsIter++)
			{
				unsigned int boneID = boneLookup[boneName];
				aiVertexWeight weight = mesh->mBones[boneIter]->mWeights[weightsIter];
				UpdateBoneData(verts, weight, boneIndex, weight.mVertexId);
			}
		}
		boneBuffer.data.finalTransforms = std::vector<glm::mat4>(rawTransforms.size());
	}

	void UpdateBoneData(std::vector<vertexAttribute_t>& verts, aiVertexWeight& const weight, unsigned int boneIndex, unsigned int vertexID)
	{
		for (size_t iter = 0; iter < 4; iter++)
		{
			//for all 4 vertex attribute slots check if the current vertex weights are 0. if they are, fill in that data
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
		//meshes[meshID].boneBuffer.BindToSlot(uniformSlot, gl_shader_storage_buffer);
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

	void LoadIntoGL(aiMesh* aimesh, mesh_t& mesh)
	{
		glGenBuffers(1, &mesh.vertexBufferHandle);
		glGenBuffers(1, &mesh.indexBufferHandle);
		glGenVertexArrays(1, &mesh.vertexArrayHandle);

		glBindVertexArray(mesh.vertexArrayHandle);
		glBindBuffer(gl_array_buffer, mesh.vertexBufferHandle);
		glBufferData(gl_array_buffer, sizeof(vertexAttribute_t) * mesh.vertices.size(), mesh.vertices.data(), gl_static_draw);

		glBindBuffer(gl_element_array_buffer, mesh.indexBufferHandle);
		glBufferData(gl_element_array_buffer, sizeof(unsigned int) * mesh.indices.size(), mesh.indices.data(), gl_static_draw);

		//might cause more issues than prevent
		unsigned int attribID = 0;

		glEnableVertexAttribArray(attribID);
		glVertexAttribBinding(attribID, 0);
		glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::position);
		glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::position);

		if (hasNormals)
		{
			glEnableVertexAttribArray(attribID);
			glVertexAttribBinding(attribID, 0);
			glVertexAttribFormat(attribID, 4, GL_FLOAT, GL_FALSE, vertexOffset::normal);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::normal);
		}

		if (hasTangentsAndBiTangents)
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

		if (hasBones)
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

	glm::mat4 ConvertToGLM(const aiMatrix4x4& aiMat)
	{
		glm::mat4 outMat;

		outMat[0].x = aiMat.a1;
		outMat[0].y = aiMat.b1;
		outMat[0].z = aiMat.c1;
		outMat[0].w = aiMat.d1;

		outMat[1].x = aiMat.a2;
		outMat[1].y = aiMat.b2;
		outMat[1].z = aiMat.c2;
		outMat[1].w = aiMat.d2;

		outMat[2].x = aiMat.a3;
		outMat[2].y = aiMat.b3;
		outMat[2].z = aiMat.c3;
		outMat[2].w = aiMat.d3;

		outMat[3].x = aiMat.a4;
		outMat[3].y = aiMat.b4;
		outMat[3].z = aiMat.c4;
		outMat[3].w = aiMat.d4;

		return outMat;
	}

	glm::mat4 ConvertToGLM(const aiMatrix3x3& aiMat)
	{
		glm::mat4 outMat;
		outMat[0].x = aiMat.a1;
		outMat[0].y = aiMat.b1;
		outMat[0].z = aiMat.c1;
		outMat[0].w = 0.0f;

		outMat[1].x = aiMat.a2;
		outMat[1].y = aiMat.b2;
		outMat[1].z = aiMat.c2;
		outMat[1].w = 0.0f;

		outMat[2].x = aiMat.a3;
		outMat[2].y = aiMat.b3;
		outMat[2].z = aiMat.c3;
		outMat[2].w = 0.0f;

		outMat[3].x = 0.0f;
		outMat[3].y = 0.0f;
		outMat[3].z = 0.0f;
		outMat[3].w = 1.0f;

		return outMat;
	}

	///////////////////////////////////////////////////////////////////////////////

	void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
	{
		printf("mesh %s has %i bones \n", pMesh->mName.C_Str(), pMesh->mNumBones);
		for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
			unsigned int BoneIndex = 0;
			std::string BoneName(pMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfo bi;
				m_BoneInfo.push_back(bi);
				m_BoneInfo[BoneIndex].BoneOffset = ConvertToGLM(pMesh->mBones[i]->mOffsetMatrix);
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else {
				BoneIndex = m_BoneMapping[BoneName];
			}
			printf("Mine: this mesh %s uses bone %s with %i weights \n", pMesh->mName.C_Str(), BoneName.c_str(), pMesh->mBones[i]->mNumWeights);
			for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
				unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;

				Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}

	void InitMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<VertexBoneData>& Bones,
		std::vector<unsigned int>& Indices)
	{
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		// Populate the vertex attribute vectors
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

			Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
			Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
			TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
		}

		LoadBones(MeshIndex, paiMesh, Bones);

		// Populate the index buffer
		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}

	void InitFromScene(const aiScene* pScene, const std::string& Filename)
	{
		m_Entries.resize(pScene->mNumMeshes);

		std::vector<glm::vec3> Positions;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> TexCoords;
		std::vector<VertexBoneData> Bones;
		std::vector<unsigned int> Indices;

		unsigned int NumVertices = 0;
		unsigned int NumIndices = 0;

		// Count the number of vertices and indices
		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			m_Entries[i].BaseVertex = NumVertices;
			m_Entries[i].BaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += m_Entries[i].NumIndices;
		}

		// Reserve space in the vectors for the vertex attributes and indices
		Positions.reserve(NumVertices);
		Normals.reserve(NumVertices);
		TexCoords.reserve(NumVertices);
		Bones.resize(NumVertices);
		Indices.reserve(NumIndices);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
		}

		// Generate and populate the buffers with vertex attributes and the indices
		glBindBuffer(gl_array_buffer, m_Buffers[POS_VB]);
		glBufferData(gl_array_buffer, sizeof(Positions[0]) * Positions.size(), &Positions[0], gl_static_draw);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(gl_array_buffer, m_Buffers[TEXCOORD_VB]);
		glBufferData(gl_array_buffer, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], gl_static_draw);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(gl_array_buffer, m_Buffers[NORMAL_VB]);
		glBufferData(gl_array_buffer, sizeof(Normals[0]) * Normals.size(), &Normals[0], gl_static_draw);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(gl_array_buffer, m_Buffers[BONE_VB]);
		glBufferData(gl_array_buffer, sizeof(Bones[0]) * Bones.size(), &Bones[0], gl_static_draw);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

		glBindBuffer(gl_element_array_buffer, m_Buffers[INDEX_BUFFER]);
		glBufferData(gl_element_array_buffer, sizeof(Indices[0]) * Indices.size(), &Indices[0], gl_static_draw);
	}

	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}
		assert(0);

		return 0;
	}

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);
		return 0;
	}

	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);
		return 0;
	}

	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}

	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
	{
		glm::mat4 Identity;
		Identity = glm::identity<glm::mat4>();

		float TicksPerSecond = (float)(assimpScene->mAnimations[0]->mTicksPerSecond != 0 ? assimpScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)assimpScene->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(AnimationTime, assimpScene->mRootNode, Identity);

		Transforms.resize(m_NumBones);

		for (unsigned int i = 0; i < m_NumBones; i++) {
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}

	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
	{
		std::string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = assimpScene->mAnimations[0];

		glm::mat4 NodeTransformation = ConvertToGLM(pNode->mTransformation);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			glm::mat4 ScalingM;
			ScalingM = glm::scale(glm::identity<glm::mat4>(), glm::vec3(Scaling.x, Scaling.y, Scaling.z));

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			glm::mat4 RotationM = glm::mat4(ConvertToGLM(RotationQ.GetMatrix()));

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			glm::mat4 TranslationM;
			TranslationM = glm::translate(glm::identity<glm::mat4>(), glm::vec3(Translation.x, Translation.y, Translation.z));

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}

		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
			unsigned int BoneIndex = m_BoneMapping[NodeName];
			m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}

	void Render()
	{
		glBindVertexArray(m_VAO);

		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

			//assert(MaterialIndex < m_Textures.size());

			/*if (m_Textures[MaterialIndex]) {
				m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
			}*/

			glDrawElementsBaseVertex(GL_TRIANGLES,
				m_Entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
				m_Entries[i].BaseVertex);
		}

		// Make sure the VAO is not changed from the outside    
		glBindVertexArray(0);
	}

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
	std::map<std::string, aiAnimation*>		animationNames;
	std::map<std::string, aiNodeAnim*>		animNodeNames;
	glm::mat4								globalInverse;

	std::map<std::string, unsigned int>		boneLookup;
	bufferHandler_t<boneTransforms_t>		boneBuffer;
	std::vector<glm::mat4>					rawTransforms;
	unsigned int							numBones;

	bool									ignoreCollision;
	bool									isPicked;
	bool									keepData;

	/*std::vector<vertexAttribute_t>			vertices;
	std::vector<unsigned int>				indices;
	unsigned int							numVertices;
	unsigned int							numIndices;

	unsigned int							vertexBufferHandle;
	unsigned int							indexBufferHandle;*/

	bool									hasBones;
	bool									hasTangentsAndBiTangents;
	bool									hasNormals;

	//OGL Dev crap
	std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
	unsigned int m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;
	glm::mat4 m_GlobalInverseTransform;    
	std::vector<MeshEntry> m_Entries;

	GLuint m_VAO;
	GLuint m_Buffers[NUM_VBs];
};
