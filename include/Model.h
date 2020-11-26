#pragma once
#include "VertexAttribute.h"

#define NUM_BONES_PER_VEREX 4

struct boneTransforms_t
{
	std::vector<glm::mat4> transforms;
};

struct mesh_t
{
	std::string								name;

	std::vector<vertexAttribute_t>			vertices;
	std::vector<unsigned int>				indices;
	std::vector<texture>					textures;
	bufferHandler_t<boneTransforms_t>		boneTransforms;

	glm::vec4								diffuse;
	glm::vec4								specular;
	glm::vec4								ambient;
	glm::vec4								emissive;
	glm::vec4								reflective;

	unsigned int							vertexArrayHandle;
	unsigned int							vertexBufferHandle;
	unsigned int							normalBufferHandle;
	unsigned int							uvBufferHandle;
	unsigned int							tangentBufferHandle;
	unsigned int							biTangentBufferHandle;
	unsigned int							weightBufferHandle;
	unsigned int							indexBufferHandle;

	bool									isCollision;

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
		normalBufferHandle = 0;
		uvBufferHandle = 0;
		tangentBufferHandle = 0;
		biTangentBufferHandle = 0;
		weightBufferHandle = 0;
		indexBufferHandle = 0;
		isCollision = false;
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
		normalBufferHandle = 0;
		uvBufferHandle = 0;
		tangentBufferHandle = 0;
		biTangentBufferHandle = 0;
		weightBufferHandle = 0;
		indexBufferHandle = 0;
		isCollision = false;
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
		assimpScene = aiImportFile(resourcePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		assert(assimpScene != nullptr);

		directory = resourcePath.substr(0, resourcePath.find_last_of('/'));

		//load the model into OpenGL
		ExtractAnimations(assimpScene);
		ExtractNode(assimpScene->mRootNode);
	}

	void BeginExtraction(aiNode* node, const aiScene* scene)
	{
		//so assuming the root node has children

	}

	void Evaluate(aiAnimation* animation, float time, bool loop, float framesPerSec)
	{
		if(animation != nullptr)
		{
			int animFrames = animation->mDuration;//?not num frames though



		}

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
				printf("name %s | \t", scene->mAnimations[iter]->mName.C_Str());
				printf("duration %f \t", scene->mAnimations[iter]->mDuration);
				printf("ticks per sec %f", scene->mAnimations[iter]->mTicksPerSecond);
				printf("\n");
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

		//go through bones, assign weights and vertex indices to vertex attribute
		if (mesh->HasBones())
		{
			for (size_t boneIter = 0; boneIter < mesh->mNumBones; boneIter++)
			{
				glm::vec4 weight = glm::vec4(0);
				glm::vec4 indices = glm::vec4(0);
				for (size_t weightsIter = 0; weightsIter < mesh->mBones[boneIter]->mNumWeights; weightsIter++)
				{
					size_t vertexID = mesh->mBones[boneIter]->mWeights[weightsIter].mVertexId;
					verts[vertexID].boneIndex = (GLuint)boneIter; //is this the right index?
					verts[vertexID].weight = mesh->mBones[boneIter]->mWeights[weightsIter].mWeight;


					/*for (size_t iter = 0; iter < NUM_BONES_PER_VEREX; iter++)
					{
						if (verts[vertexID].weight[iter] == 0.0f)
						{
							//wait if all 4 members are the same, why use vec4's? memory alignment?
							verts[vertexID].boneIndex[iter] = boneIter; //is this the right index?
							verts[vertexID].weight[iter] = mesh->mBones[boneIter]->mWeights[weightsIter].mWeight;
						}
					}*/

					glm::mat4 boneTransform;
					boneTransform[0].x = mesh->mBones[boneIter]->mOffsetMatrix.a1;
					boneTransform[0].y = mesh->mBones[boneIter]->mOffsetMatrix.b1;
					boneTransform[0].z = mesh->mBones[boneIter]->mOffsetMatrix.c1;
					boneTransform[0].w = mesh->mBones[boneIter]->mOffsetMatrix.d1;

					boneTransform[1].x = mesh->mBones[boneIter]->mOffsetMatrix.a2;
					boneTransform[1].y = mesh->mBones[boneIter]->mOffsetMatrix.b2;
					boneTransform[1].z = mesh->mBones[boneIter]->mOffsetMatrix.c2;
					boneTransform[1].w = mesh->mBones[boneIter]->mOffsetMatrix.d2;

					boneTransform[2].x = mesh->mBones[boneIter]->mOffsetMatrix.a3;
					boneTransform[2].y = mesh->mBones[boneIter]->mOffsetMatrix.b3; //why is some of this garbled?
					boneTransform[2].z = mesh->mBones[boneIter]->mOffsetMatrix.c3;
					boneTransform[2].w = mesh->mBones[boneIter]->mOffsetMatrix.d3;

					boneTransform[3].x = mesh->mBones[boneIter]->mOffsetMatrix.a4;
					boneTransform[3].y = mesh->mBones[boneIter]->mOffsetMatrix.b4;
					boneTransform[3].z = mesh->mBones[boneIter]->mOffsetMatrix.c4;
					boneTransform[3].w = mesh->mBones[boneIter]->mOffsetMatrix.d4;
					newMesh.boneTransforms.data.transforms.push_back(boneTransform);

					//ok how do we map this per vertex?
					//needs 4 weights and 4 indices per vertex
					//bones uniform needs to be a matrix
					/*bone_t bone;
					bone.indexes.push_back(vertexID);
					bone.weights.push_back(mesh->mBones[boneIter]->mWeights[weightsIter].mWeight);
					//ok. how does all this map to a vec4 for both indices and weights?
					bones.push_back(bone);*/
				}
			}
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
		glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::position);

		if(mesh->HasNormals())
		{
			glEnableVertexAttribArray(attribID);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::normal);
		}

		if(mesh->HasTangentsAndBitangents())
		{
			glEnableVertexAttribArray(attribID);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::tangent);
			
			glEnableVertexAttribArray(attribID);
			glVertexAttribPointer(attribID++, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::biNormal);
		}
		
		if(mesh->HasBones())
		{
			//if there are skeletal animations, load up the animation indices and weights
			glEnableVertexAttribArray(attribID);			
			glVertexAttribPointer(attribID++, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::boneIndex);
			
			glEnableVertexAttribArray(attribID);
			glVertexAttribPointer(attribID++, 1, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::weight);
		}

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::uv);
		
		newMesh.vertices = std::move(verts);
		newMesh.textures = std::move(textures);

		glBindVertexArray(0);
		glBindBuffer(gl_array_buffer, 0);
		glBindBuffer(gl_element_array_buffer, 0);

		meshes.push_back(newMesh);
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

	glm::vec3								position;
	glm::vec3								scale;
	glm::vec3								rotation;

	std::vector<texture>					loadedTextures;
	std::vector<std::vector<glm::vec4>>		posData;

	bool									ignoreCollision;
	bool									isPicked;
	bool									keepData;
};
