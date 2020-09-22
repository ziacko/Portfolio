#pragma once

struct vertex_t
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

	vertex_t(glm::vec4 position, glm::vec4 normal, glm::vec4 tangent,
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
};

enum vertexAttribute_t
{
	positionOffset = offsetof(vertex_t, position),
	colorOffset = offsetof(vertex_t, color),
	normalOffset = offsetof(vertex_t, normal),
	tangentOffset = offsetof(vertex_t, tangent),
	biNormalOffset = offsetof(vertex_t, biNormal),
	indicesOffset = offsetof(vertex_t, indices),
	weightsOffset = offsetof(vertex_t, weights),
	uvOffset = offsetof(vertex_t, uv),
	uv2Offset = offsetof(vertex_t, uv2)
};

struct mesh_t
{
	std::string								name;

	std::vector<vertex_t>					vertices;
	std::vector<unsigned int>				indices;
	std::vector<texture>					textures;

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
		vertices = std::vector<vertex_t>();
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

	mesh_t(std::vector<vertex_t> inVertices, std::vector<unsigned int> inIndices, std::vector<texture> inTextures) : 
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

	model_t(const char* resourcePath = "../../resources/models/SoulSpear.fbx", bool ignoreCollision = false)
	{
		this->resourcePath = resourcePath;
		position = glm::vec3(0.0f, -2.0f, -3.0f);
		scale = glm::vec3(1.0f);
		rotation = glm::vec3(0.0f);
		this->ignoreCollision = ignoreCollision;
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
		ExtractNode(assimpScene->mRootNode);
	}

	void BeginExtraction(aiNode* node, const aiScene* scene)
	{
		//so assuming the root node has children

	}

	void ExtractNode(aiNode* node)
	{
		//extract each mesh
		for (size_t iter = 0; iter < node->mNumMeshes; iter++)
		{
			ExtractMesh(assimpScene->mMeshes[node->mMeshes[iter]]);
		}

		for (size_t iter = 0; iter < node->mNumChildren; iter++)
		{
			ExtractNode(node->mChildren[iter]);
		}
	}

	void ExtractMesh(aiMesh* mesh)
	{
		mesh_t newMesh;
		newMesh.name = mesh->mName.C_Str();
		std::vector<vertex_t> verts;
		std::vector<texture> textures;

		//if ignore collision is on, skip the node with the prefix UCX_
		std::string ue4String = "UCX_";
		std::string nodeName = mesh->mName.C_Str();
		newMesh.isCollision = (nodeName.substr(0, 4).compare(ue4String) == 0);

		for (unsigned int vertexIter = 0; vertexIter < mesh->mNumVertices; vertexIter++)
		{
			glm::vec4 position = glm::vec4(0, 0, 0, 0);
			glm::vec4 normal = glm::vec4(0, 0, 0, 0);
			glm::vec4 tangent = glm::vec4(0, 0, 0, 0);
			glm::vec4 biTangent = glm::vec4(0, 0, 0, 0);
			glm::vec2 uv = glm::vec2(0, 0);

			if (mesh->mVertices != nullptr)
			{
				position = glm::vec4(mesh->mVertices[vertexIter].x,
					mesh->mVertices[vertexIter].y, mesh->mVertices[vertexIter].z, 1.0f);
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
			verts.push_back(vertex_t(position, normal, tangent, biTangent, uv));
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
		glBufferData(gl_array_buffer, sizeof(vertex_t) * verts.size(), verts.data(), gl_static_draw);

		glBindBuffer(gl_element_array_buffer, newMesh.indexBufferHandle);
		glBufferData(gl_element_array_buffer, sizeof(unsigned int) * newMesh.indices.size(), newMesh.indices.data(), gl_static_draw);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (char*)vertexAttribute_t::positionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (char*)vertexAttribute_t::normalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (char*)vertexAttribute_t::tangentOffset);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (char*)vertexAttribute_t::biNormalOffset);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (char*)vertexAttribute_t::uvOffset);
		
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

	const aiScene*							assimpScene;
	std::string								resourcePath;
	std::vector<mesh_t>						meshes;
	std::string								directory;

	bool									isGUIActive;

	glm::vec3								position;
	glm::vec3								scale;
	glm::vec3								rotation;

	std::vector<texture>					loadedTextures;

	bool									ignoreCollision;
};
