//created by Ziyad Barakat 2015

#ifndef TINYMODELS_H
#define TINYMODELS_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <fbxsdk.h>
#include <algorithm>
#include <set>
#define PI 3.14159265359f
#define TAU 6.28318530717958657692f
#define HALFPI 1.57079632679489661923f;
#define THREEHALFPI 4.71238898038468985769f;

#define	EPSILON 0.00000000001f;
#define	DEG2RAD 0.01745329251994329577f;
#define	RAD2DEG 57.2957795130823208768f;

inline int  Max(int x, int y){ return (x > y) ? x : y; }
inline int  Min(int x, int y){ return (x < y) ? x : y; }
inline float  Maxf(float x, float y){ return (x > y) ? x : y; }
inline float  Minf(float x, float y){ return (x < y) ? x : y; }


template <typename type> class ModelManager;


template<typename type>
struct vertex_t
{	
	vertex_t()
	{
		//init the vertices
		for (int i = 0; i < 4; i++)
		{
			position[i] = 0;
			color[i] = 0;
			normal[i] = 0;
			tangent[i] = 0;
			biNormal[i] = 0;
			indices[i] = 0;
			weights[i] = 0;

			if (i < 2)
			{
				uv[i] = 0;
				uv2[i] = 0;
			}

			if (i == 3)
			{
				position[i] = 1;
				color[i] = 1;
				normal[i] = 1;
				tangent[i] = 1;
				biNormal[i] = 1;
				indices[i] = 1;
				weights[i] = 1;
			}
		}
	}

	enum offsets_t
	{
		positionOffset = 0,
		colorOffset = positionOffset + sizeof(type) * 4,
		normalOffset = colorOffset + sizeof(type) * 4,
		tangentOffset = normalOffset + sizeof(type) * 4,
		biNormalOffset = tangentOffset + sizeof(type) * 4,
		indicesOffset = biNormalOffset + sizeof(type) * 4,
		weightsOffset = indicesOffset + sizeof(type) * 4,
		uvOffset = weightsOffset + sizeof(type) * 4
	};

	type position[4]; 
	type color[4]; 
	type normal[4]; 
	type tangent[4];
	type biNormal[4]; 
	type indices[4];
	type weights[4];

	type uv[2];
	type uv2[2];

	int controlPointIndex;
};

template<typename type>
struct material_t
{
	enum textureTypes
	{
		diffuseTexture = 0,
		ambientTexture,
		glowTexture,
		specularTexture,
		glossTexture,
		normalTexture,
		alphaTexture,
		displacementTexture,
		textureTypesCount
	};

	material_t()
	{
		for (int i = 0; i < 4; i++)
		{
			ambient[i] = 0;
			diffuse[i] = 0;
			specular[i] = 0;
			emissive[i] = 0;
		}

		memset(name, 0, 255);
		memset(textureFileNames, 0, textureTypesCount * 255);
		memset(textureIDs, 0, textureTypesCount * (sizeof(unsigned int)));
	}

	char name[255];
	type ambient[4];
	type diffuse[4];
	type specular[4];
	type emissive[4];

	char textureFileNames[textureTypesCount][255];
	unsigned int textureIDs[textureTypesCount];
};

template<typename type>
class node_t
{
public:

	enum nodeType_t
	{
		node = 0,
		mesh,
		light,
		camera,
		nodeTypeCount
	};

	node_t() : nodeType(node), parent(nullptr), userData(nullptr)
	{
		for(unsigned int TransformIter = 0; TransformIter < 16; TransformIter++)
		{
			localTransform[TransformIter] = 0;
			globalTransform[TransformIter] = 0;
		}

		for (unsigned int TransformIter = 0; TransformIter < 4; TransformIter++)
		{
			localTransform[TransformIter * 5] = 1;
			globalTransform[TransformIter * 5] = 1;
		}
	}

	virtual ~node_t()
	{
		for (auto Iter : children)
		{
			delete Iter;
		}
	}

	unsigned int nodeType;
	char name[255];
	type localTransform[16];
	type globalTransform[16];
	
	node_t<type>* parent;
	std::vector<node_t<type>*> children;
	void* userData;
};

template<typename type>
class meshNode_t : public node_t<type>
{
public:

	meshNode_t() : material(nullptr)
	{
		this->nodeType = node_t<type>::nodeType_t::mesh;
	}

	virtual ~meshNode_t(){};

	char name[255];
	material_t<type>* material;
	std::vector<vertex_t<type>> vertices;
	std::vector<unsigned int> indices;
};

template<typename type>
class lightNode_t : public node_t<type>
{
public:

	lightNode_t()
	{
		this->nodeType = node_t<type>::nodeType_t::light;
	}
	virtual ~lightNode_t(){};

	enum lightType_t
	{
		point = 0,
		directional,
		spot,
	};

	char name[255];
	lightType_t lightType;
	bool on;
	type color[4];
	
	type innerAngle;
	type outerAngle;

	type attenuation[4];
};

template<typename type>
class cameraNode_t : public node_t<type>
{
public:

	cameraNode_t()
	{
		this->nodeType = node_t<type>::nodeType_t::camera;
	};
	virtual ~cameraNode_t(){};

	char name[255];
	type aspectRatio;
	type FOV;
	type nearPlane;
	type farPlane;
	type viewMatrix[16];
};

template<typename type>
struct keyFrame_t
{
public:
	keyFrame_t() : key(0)
	{
		rotation[3] = 1;
		translation[3] = 1;
		scale[3] = 1;
	}
	unsigned int key;
	type rotation[4];
	type translation[4];
	type scale[4];
};

template<typename type>
struct track_t
{
public:
	track_t() : boneIndex(0), keyFrameCount(0), keyFrames(nullptr){};
	~track_t(){};

	unsigned int boneIndex;
	unsigned int keyFrameCount;
	keyFrame_t<type>* keyFrames;

};

template<typename type>
struct animation_t
{
public:
	animation_t() : trackCount(0), tracks(nullptr), startFrame(0), endFrame(0){}
	~animation_t() {};
	unsigned int TotalFrames() const
	{
		return endFrame - startFrame;
	}

	float TotalTime(float FPS = 24.0f) const
	{
		return (endFrame - startFrame) / FPS;
	}

	char name[255];
	unsigned int trackCount;
	track_t<type>* tracks;
	unsigned int startFrame;
	unsigned int endFrame;
};

template<typename type>
class skeleton_t
{
public:
	skeleton_t() : 
		boneCount(0), nodes(nullptr), bones(nullptr), 
		bindPoses(nullptr), userData(nullptr){};

	~skeleton_t()
	{
		delete[] nodes;
		delete[] bones;
		delete[] bindPoses;
	}

	void Evaluate(const animation_t<type>* animation,
		float time, bool looping = true, float FPS = 24.0f)
	{
		if (animation != nullptr)
		{
			int animationFrames = animation->endFrame - animation->startFrame;
			float animationDuration = animationFrames / FPS;

			float frameTime = 0;
			if (looping)
			{
				frameTime = Maxf(fmod(time, animationDuration), 0);
			}
			else
			{
				frameTime = Minf(Maxf(time, 0), animationDuration);
			}

			unsigned int frame = animation->startFrame + (int)(frameTime * FPS);

			const track_t<type>* track = nullptr;
			const keyFrame_t<type>* start = nullptr;
			const keyFrame_t<type>* end = nullptr;

			for (unsigned int trackIndex = 0; trackIndex < animation->trackCount;
				trackIndex++, track = &(animation->tracks[trackIndex]), start = nullptr, end = nullptr)
			{
				for (unsigned int keyFrameIndex = 0; keyFrameIndex < track->keyFrameCount - 1; keyFrameIndex++)
				{
					if (track->keyFrames[keyFrameIndex].Key <= frame &&
						track->keyFrames[keyFrameIndex + 1].Key >= frame)
					{
						start = &(track->keyFrames[keyFrameIndex]);
						end = &(track->keyFrames[keyFrameIndex + 1]);
						break;
					}
				}

				if (start != nullptr && end != nullptr)
				{
					float startTime = (start->key - animation->startFrame) / FPS;
					float endTime = (end->key - animation->startFrame) / FPS;

					float frameScale = Maxf(0, Minf(1, (frameTime - startTime) / (endTime - startTime)));

					type transform[4];
					type scale[4];

					for (unsigned int transformScaleIndex = 0; transformScaleIndex < 4; transformScaleIndex++)
					{
						transform[transformScaleIndex] = start->translation[transformScaleIndex] * (1 - frameScale) + end->translation[transformScaleIndex] * frameScale;
						scale[transformScaleIndex] = start->scale[transformScaleIndex] * (1 - frameScale) + end->scale[transformScaleIndex] * frameScale;
					}

					FbxQuaternion start(start->rotation[0], start->rotation[1], start->rotation[2], start->rotation[3]);
					FbxQuaternion end(end->rotation[0], end->rotation[1], end->rotation[2], end->rotation[3]);
					FbxQuaternion rotation;

					unsigned int cosHalfTheta = start[3] * end[3] + start[0] * end[0] + start[1] * end[1] + start[2] * end[2];

					if (abs(cosHalfTheta) >= 1.0)
					{
						for (unsigned int quatIndex = 0; quatIndex < 4; quatIndex++)
						{
							rotation[quatIndex] = start[quatIndex];
						}
					}

					else
					{
						unsigned int halfTheta = acos(cosHalfTheta);
						unsigned int sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);

						if (fabs(sinHalfTheta) < 0.0001)
						{
							for (unsigned int quatIndex = 0; quatIndex < 4; quatIndex++)
							{
								rotation[quatIndex] = (start[quatIndex] * 0.5 + end[quatIndex] * 0.5);
							}
						}
						else
						{
							unsigned int ratioA = sin((1 - frameScale) * halfTheta) / sinHalfTheta;
							unsigned int ratioB = sin(frameScale * halfTheta) / sinHalfTheta;
							for (int quatIndex = 0; quatIndex < 4; quatIndex++)
							{
								rotation[quatIndex] = (start[quatIndex] * ratioA + end[quatIndex] * ratioB);
							}
						}

						rotation.Normalize();

						FbxAMatrix matrix;
						matrix.SetTQS(FbxVector4(transform[0], transform[1], transform[3]), rotation, FbxVector4(scale[0], scale[1], scale[2]));

						FbxVector4 row0 = matrix.GetRow(0);
						FbxVector4 row1 = matrix.GetRow(1);
						FbxVector4 row2 = matrix.GetRow(2);
						FbxVector4 row3 = matrix.GetRow(3);

						for (unsigned int j = 0; j < 4; j++)
						{
							for (unsigned int k = 0; k < 4; k++)
							{
								switch (j)
								{
								case 0:
								{
									nodes[track->boneIndex]->localTransform[j * k] = (type)row0[k];
									break;
								}

								case 1:
								{
									nodes[track->boneIndex]->localTransform[j * k] = (type)row1[k];
									break;
								}

								case 2:
								{
									nodes[track->boneIndex]->localTransform[j * k] = (type)row2[k];
									break;
								}

								case 3:
								{
									nodes[track->boneIndex]->localTransform[j * k] = (type)row3[k];
									break;
								}
								default:
								{
									break;
								}
								}								
							}
						}
						if (nodes[track->boneIndex]->parent != nullptr)
						{
							nodes[track->boneIndex]->globalTransform = nodes[track->boneIndex]->localTransform * nodes[track->boneIndex]->parent->GlobalTransform;
						}

						else
						{
							nodes[track->boneIndex]->globalTransform = nodes[track->boneIndex]->localTransform;
						}
					}
				}
			}
		}
		static type matrix[16] = { 1, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 1 };

		for (unsigned int i = 0; i < boneCount; i++)
		{
			//make a function to multiply matrices together.
			bones[i] = bindPoses[i] * nodes[i]->globalTransform * matrix;
		}
	}

	unsigned int boneCount;
	node_t<type>** nodes;
	type** bones;
	type** bindPoses;
	void* userData;
};

template<typename type>
struct scene_t
{
	scene_t()
	{
		root = nullptr;
		assistor = new importAssistor_t();
	}

	~scene_t()
	{
		Unload();
	}
	
	meshNode_t<type>* GetMeshByName(const char* Name)
	{
		auto MeshIter = meshes.find(Name);
		if (MeshIter != meshes.end())
		{
			return MeshIter->second();
		}
		return nullptr;
	}

	meshNode_t<type>* GetMeshByIndex(unsigned int Index)
	{
		meshNode_t<type>* NewMesh = nullptr;
		auto MeshIter = meshes.begin();
		unsigned int Size = meshes.size();

		for (unsigned int IndexIter = 0;
			IndexIter <= Index && Index < Size;
			IndexIter++, MeshIter++)
		{
			NewMesh = MeshIter->second;
		}
		return NewMesh;
	}

	lightNode_t<type>* GetLightByName(const char* Name)
	{
		auto LightIter = lights.find(Name);
		if (LightIter != lights.end())
		{
			return LightIter->second();
		}
		return nullptr;
	}

	lightNode_t<type>* GetLightByIndex(unsigned int Index)
	{
		lightNode_t<type>* NewLight = nullptr;
		auto LightIter = lights.begin();
		unsigned int Size = lights.size();

		for (unsigned int IndexIter = 0;
			IndexIter <= Index && Index < Size;
			IndexIter++, LightIter++)
		{
			NewLight = LightIter->second;
		}
		return NewLight;
	}

	cameraNode_t<type>* GetCameraByName(const char* Name)
	{
		auto CameraIter = cameras.find(Name);
		if (CameraIter != cameras.end())
		{
			return CameraIter->second;
		}
		return nullptr;
	}

	material_t<type>* GetMaterialByName(const char* Name)
	{
		auto MaterialIter = materials.find(Name);
		if (MaterialIter != materials.end())
		{
			return MaterialIter->second;
		}
		return nullptr;
	}

	animation_t<type>* GetAnimationByName(const char* Name)
	{
		auto AnimationIter = animations.find(Name);
		if (AnimationIter != animations.end())
		{
			return AnimationIter->second;
		}
		return nullptr;
	}

	

	

	cameraNode_t<type>* GetCameraByIndex(unsigned int Index)
	{
		cameraNode_t<type>* NewCamera = nullptr;
		auto CameraIter = cameras.begin();
		unsigned int Size = cameras.size();

		for (unsigned int IndexIter = 0;
			IndexIter <= Index && Index < Size;
			IndexIter++, CameraIter++)
		{
			NewCamera = CameraIter->second;
		}
		return NewCamera;
	}

	material_t<type>* GetMaterialByIndex(unsigned int Index)
	{
		material_t<type>* NewMaterial = nullptr;
		auto MaterialIter = materials.begin();
		unsigned int Size = materials.size();

		for (unsigned int IndexIter = 0;
			IndexIter <= Index && Index < Size;
			IndexIter++, MaterialIter++)
		{
			NewMaterial = MaterialIter->second;
		}
		return NewMaterial;
	}

	animation_t<type>* GetAnimationByIndex(unsigned int Index)
	{
		animation_t<type>* NewAnimation = nullptr;
		auto AnimationIter = animations.begin();
		unsigned int Size = animations.size();

		for (unsigned int IndexIter = 0;
			IndexIter <= Index && Index < Size;
			IndexIter++, AnimationIter++)
		{
			NewAnimation = AnimationIter->second;
		}
		return NewAnimation;
	}

	void Unload()
	{
		delete root;
		root = nullptr;

		for (auto Iter : materials)
		{
			delete Iter;
		}

		for (unsigned int SkeletonIter = 0; SkeletonIter < skeletons.size(); SkeletonIter++)
		{
			delete skeletons[SkeletonIter];
		}

		for (auto Iter : animations)
		{
			for (unsigned int AnimationIter = 0; AnimationIter < Iter->TrackCount; AnimationIter++)
			{
				delete[] Iter.second->Tracks[AnimationIter].KeyFrames;
				delete[] Iter.second->Tracks;
				delete Iter.second;
			}
		}

		meshes.clear();
		lights.clear();
		cameras.clear();
		materials.clear();
		animations.clear();

		skeletons.clear();
	}

	void CollectBones(void* Objects)
	{
		FbxNode* NewNode = (FbxNode*)Objects;

		if (NewNode->GetNodeAttribute() != nullptr &&
			NewNode->GetNodeAttribute()->GetAttributeType() ==
			FbxNodeAttribute::eSkeleton)
		{
			unsigned int Index = assistor->boneIndexMap.size();

			char Name[255];
			strncpy(Name, NewNode->GetName(), 255);
			assistor->boneIndexMap[Name] = Index;

			for (int ChildIter = 0; ChildIter < NewNode->GetChildCount(); ChildIter++)
			{
				CollectBones((void*)NewNode->GetChild(ChildIter));
			}
		}
	}

	bool Load(const char* FileName)
	{
		if (root != nullptr)
		{
			printf("Scene already loaded!\n");
			return false;
		}

		FbxManager* Manager = nullptr;
		FbxScene* Scene = nullptr;

		Manager = FbxManager::Create();
		if (!Manager)
		{
			printf("unable to create a FBX Manager \n");
			return false;
		}

		FbxIOSettings* IOSettings = FbxIOSettings::Create(Manager, IOSROOT);
		Manager->SetIOSettings(IOSettings);

		FbxString AppPath = FbxGetApplicationDirectory();
		Manager->LoadPluginsDirectory(AppPath.Buffer());

		Scene = FbxScene::Create(Manager, "");

		int FileMajor, FileMinor, FileRevision;
		int SDKMajor, SDKMinor, SDKRevision;
		unsigned int Iter;
		bool Result;

		FbxManager::GetFileFormatVersion(SDKMajor, SDKMinor, SDKRevision);

		FbxImporter* Importer = FbxImporter::Create(Manager, "");

		const bool ImportStatus = Importer->Initialize(FileName, -1, Manager->GetIOSettings());
		Importer->GetFileVersion(FileMajor, FileMinor, FileRevision);

		if (!ImportStatus)
		{
			Importer->Destroy();
			return false;
		}

		Result = Importer->Import(Scene);
		Importer->Destroy();
		if (!Result)
		{
			printf("unable to open FBX file\n");
			return false;
		}

		FbxAxisSystem::OpenGL.ConvertScene(Scene);

		FbxNode* RootNode = Scene->GetRootNode();

		if (RootNode != nullptr)
		{
			assistor->currentScene = Scene;
			assistor->evaluator = Scene->GetAnimationEvaluator();

			root = new node_t<type>();
			strcpy(root->name, "root");
			type InvertZMatrix[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1 };

			for (int i = 0; i < 16; i++)
			{
				root->localTransform[i] = InvertZMatrix[i];
				root->globalTransform[i] = root->localTransform[i];
			}
			
			ambientLight[0] = (type)Scene->GetGlobalSettings().GetAmbientColor().mRed;
			ambientLight[1] = (type)Scene->GetGlobalSettings().GetAmbientColor().mGreen;
			ambientLight[2] = (type)Scene->GetGlobalSettings().GetAmbientColor().mBlue;
			ambientLight[3] = (type)Scene->GetGlobalSettings().GetAmbientColor().mAlpha;

			for (Iter = 0; Iter < RootNode->GetChildCount(); Iter++)
			{
				CollectBones((void*)RootNode->GetChild(Iter));
			}

			for (Iter = 0; Iter < RootNode->GetChildCount(); Iter++)
			{
				ExtractObject(root, (void*)RootNode->GetChild(Iter));
			}

			if (assistor->bones.size() > 0)
			{
				skeleton_t<type>* Skeleton = new skeleton_t<type>();
				Skeleton->boneCount = assistor->bones.size();
				Skeleton->nodes = new node_t<type>*[Skeleton->boneCount];
				memset(Skeleton->bones, 0, (sizeof(type) * 16) * Skeleton->boneCount);
				memset(Skeleton->bindPoses, 0, (sizeof(type) * 16) * Skeleton->boneCount);

				for (Iter = 0; Iter < Skeleton->boneCount; Iter++)
				{
					Skeleton->nodes[Iter] = assistor->bones[Iter];
					Skeleton->bones[Iter] = Skeleton->nodes[Iter]->localTransform;
				}

				ExtractSkeleton(Skeleton, Scene);
				skeletons.push_back(Skeleton);
				ExtractAnimation(Scene);
			}
			Manager->Destroy();
			path = (char*)FileName;
			return true;
		}

		else
		{
			return false;
		}
		
	}

	void ExtractObject(node_t<type>* Parent, void* Object)
	{
		FbxNode* FBXNode = (FbxNode*)Object;
		node_t<type>* TinyNode = nullptr;

		FbxNodeAttribute::EType AttributeType;
		unsigned int Iter = 0;

		bool IsBone = false;
		if (FBXNode->GetNodeAttribute() != nullptr)
		{
			AttributeType = FBXNode->GetNodeAttribute()->GetAttributeType();

			switch (AttributeType)
			{
				case FbxNodeAttribute::eMarker:
				{
					break;
				}

				case FbxNodeAttribute::eSkeleton:
				{
					IsBone = true;
					break;
				}

				case FbxNodeAttribute::eMesh:
				{
					TinyNode = new meshNode_t<type>();
					ExtractMesh((meshNode_t<type>*)TinyNode, FBXNode);
					if (strlen(FBXNode->GetName()) > 0)
					{
						strncpy(TinyNode->name, FBXNode->GetName(), 255 - 1);
					}
					meshes[TinyNode->name] = (meshNode_t<type>*)TinyNode;
					break;
				}

				case FbxNodeAttribute::eNurbs:
				{
					//umm what is a nurb?
					break;
				}

				case FbxNodeAttribute::ePatch:
				{
					break;
				}

				case FbxNodeAttribute::eCamera:
				{
					TinyNode = new cameraNode_t<type>();
					ExtractCamera((cameraNode_t<type>*)TinyNode, FBXNode);

					if (strlen(FBXNode->GetName()) > 0)
					{
						strncpy(TinyNode->name, FBXNode->GetName(), 254);
					}
					cameras[TinyNode->name] = (cameraNode_t<type>*)TinyNode;
					break;
				}

				case FbxNodeAttribute::eLight:
				{
					TinyNode = new lightNode_t<type>();
					ExtractLight((lightNode_t<type>*)TinyNode, FBXNode);

					if (strlen(FBXNode->GetName()) > 0)
					{
						strncpy(TinyNode->name, FBXNode->GetName(), 254);
					}
					lights[TinyNode->name] = (lightNode_t<type>*)TinyNode;
					break;
				}
			}
		}

		if (TinyNode == nullptr)
		{
			TinyNode = new node_t<type>();
			if (strlen(FBXNode->GetName()) > 0)
			{
				strncpy(TinyNode->name, FBXNode->GetName(), 254);
			}
		}

		Parent->children.push_back(TinyNode);
		TinyNode->parent = Parent;
		FbxAMatrix Local = assistor->evaluator->GetNodeLocalTransform(FBXNode);

		FbxVector4 Row0 = Local.GetRow(0);
		FbxVector4 Row1 = Local.GetRow(1);
		FbxVector4 Row2 = Local.GetRow(2);
		FbxVector4 Row3 = Local.GetRow(3);

		TinyNode->localTransform[0] = Row0.mData[0];
		TinyNode->localTransform[1] = Row0.mData[1];
		TinyNode->localTransform[2] = Row0.mData[2];
		TinyNode->localTransform[3] = Row0.mData[3];

		for (int RowIter = 0; RowIter < 4; RowIter++)
		{
			for (int i = 0; i < 4; i++)
			{
				switch (RowIter)
				{
				case 0:
				{
					TinyNode->localTransform[i + (i * RowIter)] = Row0.mData[i];
					break;
				}

				case 1:
				{
					TinyNode->localTransform[i + (i * RowIter)] = Row1.mData[i];
					break;
				}

				case 2:
				{
					TinyNode->localTransform[i + (i * RowIter)] = Row2.mData[i];
					break;
				}

				case 3:
				{
					TinyNode->localTransform[i + (i * RowIter)] = Row3.mData[i];
					break;
				}

				default:
				{
					break;
				}
						
				}
			}
		}

		//TinyNode->GlobalTransform = TinyNode->LocalTransform * Parent->GlobalTransform;
		if (IsBone)
		{
			assistor->bones.push_back(TinyNode);
		}

		for (int Iter = 0; Iter < FBXNode->GetChildCount(); Iter++)
		{
			ExtractObject(TinyNode, (void*)FBXNode->GetChild(Iter));
		}
	}

	void ExtractMesh(meshNode_t<type>* Mesh, void* Object)
	{
		FbxNode* FBXNode = (FbxNode*)Object;
		FbxMesh* FBXMesh = (FbxMesh*)FBXNode->GetNodeAttribute();

		int PolyIter, J;
		unsigned int PolyCount = FBXMesh->GetPolygonCount();
		FbxVector4* ControlPoints = FBXMesh->GetControlPoints();

		vertex_t<type> Vertex;
		unsigned int VertexIndex[4] = {};
		unsigned int VertexID = 0;

		for (PolyIter = 0; PolyIter < PolyCount; PolyIter++)
		{
			int L;
			int PolySize = FBXMesh->GetPolygonSize(PolyIter);

			for (J = 0; J < PolySize && J < 4; J++)
			{
				unsigned int ControlPointIndex = FBXMesh->GetPolygonVertex(PolyIter, J);
				Vertex.controlPointIndex = ControlPointIndex;

				FbxVector4 Position = ControlPoints[ControlPointIndex];
				Vertex.position[0] = (type)Position[0];
				Vertex.position[1] = (type)Position[1];
				Vertex.position[2] = (type)Position[2];
				Vertex.position[3] = (type)1;

				for (L = 0; L < FBXMesh->GetElementVertexColorCount(); L++)
				{
					FbxGeometryElementVertexColor* GEVC = FBXMesh->GetElementVertexColor(L);
					switch (GEVC->GetMappingMode())
					{
						case FbxGeometryElement::eByControlPoint:
						{
							switch (GEVC->GetReferenceMode())
							{
								case FbxGeometryElement::eDirect:
								{
									FbxColor Color = GEVC->GetDirectArray().GetAt(ControlPointIndex);
									Vertex.color[0] = (type)Color.mRed;
									Vertex.color[1] = (type)Color.mGreen;
									Vertex.color[2] = (type)Color.mBlue;
									Vertex.color[3] = (type)Color.mAlpha;
									break;
								}
						
								case FbxGeometryElement::eIndexToDirect:
								{
									unsigned int ID = GEVC->GetIndexArray().GetAt(ControlPointIndex);
									FbxColor Color = GEVC->GetDirectArray().GetAt(ID);

									Vertex.color[0] = (type)Color.mRed;
									Vertex.color[1] = (type)Color.mGreen;
									Vertex.color[2] = (type)Color.mBlue;
									Vertex.color[3] = (type)Color.mAlpha;
									break;
								}
								default:
								{
									break;
								}
							}
							break;
						}

						case FbxGeometryElement::eByPolygonVertex:
						{
							switch (GEVC->GetReferenceMode())
							{
								case FbxGeometryElement::eDirect:
								{
									FbxColor Color = GEVC->GetDirectArray().GetAt(ControlPointIndex);
									Vertex.color[0] = (type)Color.mRed;
									Vertex.color[1] = (type)Color.mGreen;
									Vertex.color[2] = (type)Color.mBlue;
									Vertex.color[3] = (type)Color.mAlpha;
									break;
								}

								case FbxGeometryElement::eIndexToDirect:
								{
									unsigned int ID = GEVC->GetIndexArray().GetAt(ControlPointIndex);
									FbxColor Color = GEVC->GetDirectArray().GetAt(ID);

									Vertex.color[0] = (type)Color.mRed;
									Vertex.color[1] = (type)Color.mGreen;
									Vertex.color[2] = (type)Color.mBlue;
									Vertex.color[3] = (type)Color.mAlpha;
									break;
								}
								default:
								{
									break;
								}
							}
						}

						case FbxGeometryElement::eByPolygon:
						{
							break;
						}

						case FbxGeometryElement::eAllSame:
						{
							break;
						}

						case FbxGeometryElement::eNone:
						{
							break;
						}
					}
				}

				for (L = 0; L < FBXMesh->GetElementUVCount(); L++)
				{
					FbxGeometryElementUV* UV = FBXMesh->GetElementUV(L);

					switch (UV->GetMappingMode())
					{
					case FbxGeometryElement::eByControlPoint:
					{
						switch (UV->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							{
								FbxVector2 uv = UV->GetDirectArray().GetAt(ControlPointIndex);
								Vertex.uv[0] = (type)uv[0];
								Vertex.uv[1] = (type)uv[1];
								break;
							}

							case FbxGeometryElement::eIndexToDirect:
							{
								unsigned int ID = UV->GetIndexArray().GetAt(ControlPointIndex);
								FbxVector2 uv = UV->GetDirectArray().GetAt(ID);
								Vertex.uv[0] = (type)uv[0];
								Vertex.uv[1] = (type)uv[1];
								break;
							}

							default:
							{
								break;
							}
						}
						break;
					}
					case FbxGeometryElement::eByPolygonVertex:
					{
						unsigned int TextureUVIndex = FBXMesh->GetTextureUVIndex(PolyIter, J);
						switch (UV->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
							{
								FbxVector2 uv = UV->GetDirectArray().GetAt(TextureUVIndex);

								Vertex.uv[0] = (type)uv[0];
								Vertex.uv[1] = (type)uv[1];
								break;
							}

							default:
							{
								break;
							}
						}
						break;
					}
					case FbxGeometryElement::eByPolygon:
					case FbxGeometryElement::eAllSame:
					case FbxGeometryElement::eNone:
					{
						break;
					}
					}
				}
				for (L = 0; L < FBXMesh->GetElementNormalCount(); L++)
				{
					FbxGeometryElementNormal* Normal = FBXMesh->GetElementNormal(L);
					if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
					{
						switch (Normal->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							{
								FbxVector4 normal = Normal->GetDirectArray().GetAt(ControlPointIndex);
								Vertex.normal[0] = (type)normal[0];
								Vertex.normal[1] = (type)normal[1];
								Vertex.normal[2] = (type)normal[2];
								Vertex.normal[3] = (type)normal[3];
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								unsigned int ID = Normal->GetIndexArray().GetAt(ControlPointIndex);
								FbxVector4 normal = Normal->GetDirectArray().GetAt(ID);
								Vertex.normal[0] = (type)normal[0];
								Vertex.normal[1] = (type)normal[1];
								Vertex.normal[2] = (type)normal[2];
								Vertex.normal[3] = (type)normal[3];
								break;
							}
							default:
							{
								break;
							}
						}
					}
					else if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (Normal->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							{
								FbxVector4 normal = Normal->GetDirectArray().GetAt(VertexID);
							
								Vertex.normal[0] = (type)normal[0];
								Vertex.normal[1] = (type)normal[1];
								Vertex.normal[2] = (type)normal[2];
								Vertex.normal[3] = (type)normal[3];
								break;
							}
							case FbxGeometryElement::eIndexToDirect:
							{
								unsigned int ID = Normal->GetIndexArray().GetAt(VertexID);
								FbxVector4 normal = Normal->GetDirectArray().GetAt(ID);
								Vertex.normal[0] = (type)normal[0];
								Vertex.normal[1] = (type)normal[1];
								Vertex.normal[2] = (type)normal[2];
								Vertex.normal[3] = (type)normal[3];
								break;
							}

							default:
							{
								break;
							}
						}
					}
				}

				Mesh->vertices.push_back(Vertex);
				VertexIndex[J] = Mesh->vertices.size() - 1;
				VertexID++;
			}
			Mesh->indices.push_back(VertexIndex[0]);
			Mesh->indices.push_back(VertexIndex[1]);
			Mesh->indices.push_back(VertexIndex[2]);

			if (PolySize == 4)
			{
				Mesh->indices.push_back(VertexIndex[0]);
				Mesh->indices.push_back(VertexIndex[2]);
				Mesh->indices.push_back(VertexIndex[3]);
			}
		}
		CalculateTangentsBinormals(Mesh->vertices, Mesh->indices);
		ExtractSkin(Mesh, (void*)FBXMesh);

		Mesh->material = ExtractMaterial(FBXMesh);
	}

	void ExtractSkin(meshNode_t<type>* Mesh, void* Node)
	{
		FbxGeometry* Geometry = (FbxGeometry*)Node;
		unsigned int I, J, K;
		unsigned int ClusterCount;
		FbxCluster* Cluster;
		char Name[255];

		int VertCount = Mesh->vertices.size();

		FbxSkin* Skin = (FbxSkin*)Geometry->GetDeformer(0, FbxDeformer::eSkin);

		if (Skin != nullptr)
		{
			ClusterCount = Skin->GetClusterCount();

			for (J = 0; J != ClusterCount; J++)
			{
				Cluster = Skin->GetCluster(J);
				if (Cluster->GetLink() == nullptr)
				{
					continue;
				}
				strncpy(Name, Cluster->GetLink()->GetName(), 255);
				unsigned int BoneIndex = assistor->boneIndexMap[Name];

				unsigned int IndexCount = Cluster->GetControlPointIndicesCount();
				int* Indices = Cluster->GetControlPointIndices();
				double* Weights = Cluster->GetControlPointWeights();

				for (K = 0; K < IndexCount; K++)
				{
					for (I = 0; I < VertCount; I++)
					{
						if (Mesh->vertices[I].controlPointIndex == Indices[K])
						{
							if (Mesh->vertices[I].weights[0] == 0)
							{
								Mesh->vertices[I].weights[0] = (type)Weights[K];
								Mesh->vertices[I].indices[0] = (type)BoneIndex;
							}
							else if (Mesh->vertices[I].weights[1] == 0)
							{
								Mesh->vertices[I].weights[1] = (type)Weights[K];
								Mesh->vertices[I].indices[1] = (type)BoneIndex;
							}
							else if (Mesh->vertices[I].weights[2] == 0)
							{
								Mesh->vertices[I].weights[2] = (type)Weights[K];
								Mesh->vertices[I].indices[2] = (type)BoneIndex;
							}
							else
							{
								Mesh->vertices[I].weights[3] = (type)Weights[K];
								Mesh->vertices[I].indices[3] = (type)BoneIndex;
							}
						}
					}
				}
			}
		}
	}

	void ExtractLight(lightNode_t<type>* Light, void* Object)
	{
		FbxNode* FBXNode = (FbxNode*)Object;
		FbxLight* FBXLight = (FbxLight*)FBXNode->GetNodeAttribute();

		Light->lightType = (typename lightNode_t<type>::lightType_t)FBXLight->LightType.Get();
		Light->on = FBXLight->CastLight.Get();
		Light->color[0] = (type)FBXLight->Color.Get()[0];
		Light->color[1] = (type)FBXLight->Color.Get()[1];
		Light->color[2] = (type)FBXLight->Color.Get()[2];
		Light->color[3] = (type)FBXLight->Intensity.Get();

		Light->innerAngle = (type)FBXLight->InnerAngle.Get() * DEG2RAD;
		Light->outerAngle = (type)FBXLight->OuterAngle.Get() * DEG2RAD;

		switch (FBXLight->DecayType.Get())
		{
			case 0:
			{
				Light->attenuation[0] = 1;
			}
			case 1:
			{
				Light->attenuation[1] = 1;
			}
			case 2:
			{
				Light->attenuation[2] = 1;
			}
			default:
			{
				break;
			}
		}
	}

	void ExtractCamera(cameraNode_t<type>* Camera, void* Object)
	{
		FbxNode* FBXNode = (FbxNode*)Object;
		FbxCamera* FBXCamera = (FbxCamera*)FBXNode->GetNodeAttribute();

		if (FBXCamera->ProjectionType.Get() != FbxCamera::eOrthogonal)
		{
			Camera->FOV = (type)FBXCamera->FieldOfView.Get() * DEG2RAD;
		}
		else
		{
			Camera->FOV = 0;
		}

		if (FBXCamera->GetAspectRatioMode() != FbxCamera::eWindowSize)
		{
			Camera->aspectRatio = (type)FBXCamera->AspectWidth.Get() / (type)FBXCamera->AspectHeight.Get();
		}
		else
		{
			Camera->aspectRatio = 0;
		}

		Camera->nearPlane = (type)FBXCamera->NearPlane.Get();
		Camera->farPlane = (type)FBXCamera->FarPlane.Get();

		type Eye[3], To[3], Up[3];

		
		for (unsigned int i = 0; i < 3; i++)
		{
			Eye[i] = (type)FBXCamera->Position.Get()[i];

			if (FBXNode->GetTarget() != nullptr)
			{
				To[i] = (type)FBXNode->GetTarget()->LclTranslation.Get()[i];
			}
			else
			{
				To[i] = (type)FBXCamera->InterestPosition.Get()[i];
			}

			if (FBXNode->GetTargetUp())
			{
				Up[i] = (type)FBXNode->GetTargetUp()->LclTranslation.Get()[i];
			}

			else
			{
				Up[i] = (type)FBXCamera->UpVector.Get()[i];
			}
		}

		for (unsigned int i = 0; i < 3; i++)
		{
			Up[i] = (Up[i] / Up[i]);
		}
		
		//create a view matrix

	}

	material_t<type>* ExtractMaterial(void* Mesh)
	{
		FbxGeometry* Geometry = (FbxGeometry*)Mesh;

		int MaterialCount = 0;
		FbxNode* Node = nullptr;

		if (Geometry)
		{
			Node = Geometry->GetNode();
			if (Node)
			{
				MaterialCount = Node->GetMaterialCount();
			}
		}

		if (MaterialCount > 0)
		{
			FbxSurfaceMaterial* Material = Node->GetMaterial(0);

			char MaterialName[255];
			strncpy(MaterialName, Material->GetName(), 254);

			auto Iter = materials.find(MaterialName);

			if (Iter != materials.end())
			{
				return Iter->second;
			}

			else
			{
				material_t<type>* TinyMaterial = new material_t<type>;
				memcpy(TinyMaterial->name, MaterialName, 255);

				const FbxImplementation* Implementation = GetImplementation(Material, FBXSDK_IMPLEMENTATION_HLSL);

				if (Implementation == nullptr)
				{
					Implementation = GetImplementation(Material, FBXSDK_IMPLEMENTATION_CGFX);
				}

				if (Implementation != nullptr)
				{
					FbxBindingTable const* RootTable = Implementation->GetRootTable();
					FbxString FileName = RootTable->DescAbsoluteURL.Get();
					FbxString TechniqueName = RootTable->DescTAG.Get();

					FBXSDK_printf("Unsupported hardware shader material! \n");
					FBXSDK_printf("File: %s\n", FileName.Buffer());
					FBXSDK_printf("Technique: %s\n\n", TechniqueName.Buffer());
				}
				else if (Material->GetClassId().Is(FbxSurfacePhong::ClassId))
				{
					FbxSurfacePhong* Phong = (FbxSurfacePhong*)Material;

					for (int i = 0; i < 4; i++)
					{

						if (i < 3)
						{
							TinyMaterial->ambient[i] = (type)Phong->Ambient.Get()[i];
							TinyMaterial->diffuse[i] = (type)Phong->Diffuse.Get()[i];
							TinyMaterial->specular[i] = (type)Phong->Specular.Get()[i];
							TinyMaterial->emissive[i] = (type)Phong->Emissive.Get()[i];							
						}

						else
						{
							TinyMaterial->ambient[3] = (type)Phong->AmbientFactor.Get();
							TinyMaterial->diffuse[3] = 1.0f - (type)Phong->TransparencyFactor.Get();
							TinyMaterial->specular[3] = (type)Phong->Shininess.Get();
							TinyMaterial->emissive[3] = (type)Phong->EmissiveFactor.Get();
						}
					}
				}

				else if (Material->GetClassId().Is(FbxSurfaceLambert::ClassId))
				{
					FbxSurfaceLambert* Lambert = (FbxSurfaceLambert*)Material;

					for (int i = 0; i < 4; i++)
					{
						if (i < 3)
						{
							TinyMaterial->ambient[i] = Lambert->Ambient.Get()[i];
							TinyMaterial->diffuse[i] = Lambert->Diffuse.Get()[i];
							TinyMaterial->emissive[i] = Lambert->Emissive.Get()[i];
						}
						else
						{
							TinyMaterial->ambient[3] = (type)Lambert->AmbientFactor.Get();
							TinyMaterial->diffuse[3] = 1.0f - (type)Lambert->TransparencyFactor.Get();
							TinyMaterial->emissive[3] = (type)Lambert->EmissiveFactor.Get();
						}
						TinyMaterial->specular[i] = 0;
					}
					
				}

				else
				{
					FBXSDK_printf("Unknown material type \n");
				}

				unsigned int textureLookup[] =
				{
					FbxLayerElement::eTextureDiffuse - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureAmbient - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureEmissive - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureSpecular - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureShininess - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureNormalMap - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureTransparency - FbxLayerElement::sTypeTextureStartIndex,
					FbxLayerElement::eTextureDisplacement - FbxLayerElement::sTypeTextureStartIndex,
				};

				for (unsigned int TextureIter = 0; TextureIter < material_t<type>::textureTypesCount; TextureIter++)
				{
					FbxProperty Property = Material->FindProperty(FbxLayerElement::sTextureChannelNames[textureLookup[TextureIter]]);
					if (Property.IsValid() && 
						Property.GetSrcObjectCount<FbxTexture>() > 0)
					{
						FbxFileTexture* FileTexture = FbxCast<FbxFileTexture>(Property.GetSrcObject<FbxTexture>(0));

						if (FileTexture)
						{
							const char* LastForward = strrchr(FileTexture->GetFileName(), '/');
							const char* LastBackward = strrchr(FileTexture->GetFileName(), '\\');
							const char* FileName = FileTexture->GetFileName();

							if (LastForward != nullptr && LastForward > LastBackward)
							{
								FileName = LastForward + 1;
							}
							else if (LastBackward != nullptr)
							{
								FileName = LastBackward + 1;
							}

							if (strlen(FileName) >= 255)
							{
								FBXSDK_printf("Texture filename too long!: %s\n", FileName);
							}
							else
							{
								strcpy(TinyMaterial->textureFileNames[TextureIter], FileName);
							}
						}
					}
				}
				materials[TinyMaterial->name] = TinyMaterial;
				return TinyMaterial;
			}
		}
	}

	void ExtractAnimation(void* Scene)
	{
		FbxScene* FBXScene = (FbxScene*)Scene;

		for (unsigned int i = 0; i < FBXScene->GetSrcObjectCount<FbxAnimStack>(); i++)
		{
			FbxAnimStack* AnimationStack = FBXScene->GetSrcObject<FbxAnimStack>(i);

			animation_t<type>* Animation = new animation_t<type>();
			strncpy(Animation->name, AnimationStack->GetName(), 255);

			std::vector<track_t<type>> Tracks;

			int AnimationLayers = AnimationStack->GetMemberCount(FbxCriteria::ObjectType(FbxAnimLayer::ClassId));
			for (unsigned int LayerIter = 0; LayerIter < AnimationLayers; LayerIter++)
			{
				FbxAnimLayer* AnimationLayer = AnimationStack->GetMember<FbxAnimLayer>(LayerIter);
				ExtractAnimationTrack(Tracks, AnimationLayer, FBXScene->GetRootNode());
			}

			Animation->startFrame = 999999999;
			Animation->endFrame = 0;

			Animation->trackCount = Tracks.size();

			if (Animation->trackCount > 0)
			{
				Animation->tracks = new track_t<type>[Animation->trackCount];
				memcpy(Animation->tracks, Tracks.data(), sizeof(track_t<type>) * Animation->trackCount);

				for (unsigned int j = 0; j < Animation->trackCount; j++)
				{
					for (unsigned int k = 0; k < Animation->tracks[j].keyFrameCount; k++)
					{
						Animation->startFrame = (Animation->startFrame > Animation->tracks[j].keyFrames[k].key) ? Animation->startFrame : Animation->tracks[j].keyFrames[k].key;
						Animation->endFrame = (Animation->endFrame < Animation->tracks[j].keyFrames[k].key) ? Animation->endFrame : Animation->tracks[j].keyFrames[k].key;
					}
				}
			}
			animations[Animation->name] = Animation;
		}
	}

	void ExtractAnimationTrack(std::vector<track_t<type>>& Tracks, void* Layer, void* Node)
	{
		FbxAnimLayer* AnimLayer = (FbxAnimLayer*)Layer;
		FbxNode* FBXNode = (FbxNode*)Node;

		skeleton_t<type>* Skeleton = skeletons[0];

		int BoneIndex = -1;

		for (unsigned int i = 0; i < Skeleton->boneCount; i++)
		{
			if (strncmp(Skeleton->nodes[i]->name, FBXNode->GetName(), 255) == 0)
			{
				BoneIndex = i;
				break;
			}
		}
		if (BoneIndex >= 0)
		{
			FbxAnimCurve* AnimCurve = nullptr;
			std::map<int, FbxTime> KeyFrameTimes;

			int KeyCount = 0;
			int Count;
			char TimeString[256];

			AnimCurve = FBXNode->LclTranslation.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclTranslation.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclTranslation.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclRotation.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclRotation.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclRotation.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclScaling.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclScaling.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			AnimCurve = FBXNode->LclScaling.GetCurve(AnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
			if (AnimCurve)
			{
				KeyCount = AnimCurve->KeyGetCount();
				for (Count = 0; Count < KeyCount; Count++)
				{
					int Key = atoi(AnimCurve->KeyGetTime(Count).GetTimeString(TimeString, FbxUShort(256)));
					KeyFrameTimes[Key] = AnimCurve->KeyGetTime(Count);
				}
			}

			if (KeyFrameTimes.size() > 0)
			{
				track_t<type> Track;

				Track.boneIndex = BoneIndex;
				Track.keyFrameCount = KeyFrameTimes.size();
				Track.keyFrames = new keyFrame_t<type>[Track.keyFrameCount];

				int Index = 0;

				for (auto Iter : KeyFrameTimes)
				{
					Track.keyFrames[Index].key = Iter.first;

					FbxAMatrix LocalMatrix = assistor->evaluator->GetNodeLocalTransform(FBXNode, Iter.second);

					FbxQuaternion rotation = LocalMatrix.GetQ();
					FbxVector4 translation = LocalMatrix.GetT();
					FbxVector4 scale = LocalMatrix.GetS();

					for (int i = 0; i < 4; i++)
					{
						if (i < 3)
						{
							Track.keyFrames[Index].rotation[i] = (type)rotation[i];
							Track.keyFrames[Index].translation[i] = (type)translation[i];
							Track.keyFrames[Index].scale[i] = (type)scale[i];
						}
						else
						{
							Track.keyFrames[Index].rotation[i] = 0;
							Track.keyFrames[Index].scale[i] = 0;
						}
					}
					Index++;
				}
				Tracks.push_back(Track);
			}
			
		}

		for (unsigned int i = 0; i < FBXNode->GetChildCount(); i++)
		{
			ExtractAnimationTrack(Tracks, AnimLayer, FBXNode->GetChild(i));
		}
	}

	void ExtractSkeleton(skeleton_t<type>* Skeleton, void* Scene)
	{
		FbxScene* FBXScene = (FbxScene*)Scene;

		int PoseCount = FBXScene->GetPoseCount();

		char Name[255];

		for (unsigned int i = 0; i < PoseCount; i++)
		{
			FbxPose* Pose = FBXScene->GetPose(i);

			for (unsigned int j = 0; j < Pose->GetCount(); j++)
			{
				strncpy(Name, Pose->GetNodeName(j).GetCurrentName(), 255);

				FbxMatrix PoseMatrix = Pose->GetMatrix(j);
				FbxMatrix BindMatrix = PoseMatrix.Inverse();

				for (unsigned int k = 0; k < Skeleton->boneCount; k++)
				{
					if (strcmp(Name, Skeleton->nodes[k]->name) == 0)
					{
						FbxVector4 Row0 = BindMatrix.GetRow(0);
						FbxVector4 Row1 = BindMatrix.GetRow(1);
						FbxVector4 Row2 = BindMatrix.GetRow(2);
						FbxVector4 Row3 = BindMatrix.GetRow(3);
						
						for (unsigned int l = 0; l < 4; l++)
						{
							switch (l)
							{
							case 0:
							{
								Skeleton->bindPoses[l][k] = (type)Row0.mData[l];
								break;
							}
							case 1:
							{
								Skeleton->bindPoses[l][k] = (type)Row1.mData[l];
								break;
							}
							case 2:
							{
								Skeleton->bindPoses[l][k] = (type)Row2.mData[l];
								break;
							}
							case 3:
							{
								Skeleton->bindPoses[l][k] = (type)Row3.mData[l];
								break;
							}
							default:
							{
								break;
							}
							}
						}
					}
				}
			}
		}
	}

	/*bool VertexExists(const std::vector<TVertex<type>>& Vertices, const TVertex<type>& Vertex, unsigned int& Index)
	{
		auto Iter = std::find(std::begin(Vertices), std::begin(Vertices), Vertex);

		if(Iter != Vertices.end())
		{
			Index = Iter - Vertices.begin();
			return true;
		}

		return false;
	}*/

	void CalculateTangentsBinormals(std::vector<vertex_t<type>>& Vertices, const std::vector<unsigned int>& Indices)
	{
		/*unsigned int VertexCount = Vertices.size();
		type* Tan1 = new type[VertexCount * 2];
		type* Tan2 = Tan1 + VertexCount;
		memset(Tan1, 0, VertexCount * sizeof(Tan1) * 2);

		unsigned int IndexCount = Indices.size();
		for (unsigned int a = 0; a < IndexCount; a += 3)
		{
			long I1 = Indices[a];
			long I2 = Indices[a + 1];
			long I3 = Indices[a + 2];

			type Vertex1[4] = Vertices[I1].position;
			type Vertex2[4] = Vertices[I2].position;
			type Vertex3[4] = Vertices[I3].position;

			type UV1[2] = Vertices[I1].uv;
			type UV2[2] = Vertices[I2].uv;
			type UV3[2] = Vertices[I3].uv;

			type X1 = Vertex2[0] - Vertex1[0];
			type X2 = Vertex3[0] - Vertex1[0];
			type Y1 = Vertex2[1] - Vertex1[1];
			type Y2 = Vertex3[1] - Vertex1[1];
			type Z1 = Vertex2[2] - Vertex1[2];
			type Z2 = Vertex3[2] - Vertex1[2];

			type S1 = UV2[0] - UV1[0];
			type S2 = UV3[0] - UV1[0];
			type T1 = UV2[1] - UV1[1];
			type T2 = UV3[1] - UV1[1];

			float R = 1.0f / (S2 * T2 - S2 * T1);

			type SDir[4] =
			{
				(T2 * X1 - T1 * X2) * R,
				(T2 * Y1 - T1 * Y2) * R,
				(T2 * Z1 - T1 * Z2) * R,
				0
			};

			type TDir[4] =
			{
				(S1 * X2 - S2 * X1) * R,
				(S1 * Y2 - S2 * Y1) * R,
				(S1 * Z2 - S2 * Z1) * R,
				0
			};

			Tan1[I1] += SDir;
			Tan1[I2] += SDir;
			Tan1[I3] += SDir;

			Tan2[I1] += TDir;
			Tan2[I2] += TDir;
			Tan2[I3] += TDir;
		}

		for (unsigned int a = 0; a < VertexCount; a++)
		{
			const type Normal[4] = Vertices[a].normal;
			const type Tangent[4] = Tan1[a];

			Vertices[a].tangent = (Tangent - Normal * DotProduct(Normal, Tangent));
			Vertices[a].tangent->normalize;

			Vertices[a].tangent[3] = (DotProduct(CrossProduct(Normal, Tangent, Tan2[a])) < 0.0f) ? -1.0f : 1.0f;

			Vertices[a].biNormal[0]
		}

		delete[] Tan1;*/
	}

	unsigned int NodeCount(node_t<type>* Node)
	{
		unsigned int NumNodes = 1;
		for (unsigned int NodeIter = 0; NodeIter < Node->children.size(); NodeIter++)
		{
			NumNodes += NodeCount(Node->children[NodeIter]);
		}
		return NumNodes;
	}

	bool SaveTinyModel(const char* FileName)
	{
		FILE* File = fopen(FileName, "wb");

		unsigned int I, J = 0;
		unsigned int Address = 0;

		fwrite(&ambientLight, sizeof(type), 4, File);

		fwrite(&materials.size(), sizeof(unsigned int), 1, File);

		for (auto MaterialIter : materials)
		{
			Address = (unsigned int)MaterialIter->second;

			fwrite(&Address, sizeof(unsigned int), 1, File);

			fwrite(&MaterialIter->second, sizeof(material_t<type>), 1, File);
		}

		unsigned int ObjectCount = NodeCount(root);
		fwrite(&ObjectCount, sizeof(unsigned int), 1, File);
		SaveNodeData(root, File);

		ObjectCount = skeletons.size();
		fwrite(&ObjectCount, sizeof(unsigned int), 1, File);

		for (unsigned int SkeletonIter = 0; SkeletonIter < skeletons.size(); SkeletonIter++)
		{
			fwrite(&skeletons[SkeletonIter]->boneCount, sizeof(unsigned int), 1, File);
			fwrite(&skeletons[SkeletonIter]->bindPoses, sizeof(type) * 16, skeletons[SkeletonIter].boneCount, File);
			fwrite(&skeletons[SkeletonIter]->nodes, sizeof(node_t<type>*), skeletons[SkeletonIter].boneCount, File);
		}

		ObjectCount = animations.size();
		fwrite(&ObjectCount, sizeof(unsigned int), 1, File);

#if defined(_MSC_VER)
		for each(auto Iter in animations)
#else
		for (auto Iter : animations)
#endif
		{
			fwrite(Iter->second->name, sizeof(char), 255, File);
			fwrite(&Iter->second->startFrame, sizeof(unsigned int), 255, File);
			fwrite(&Iter->second->endFrame, sizeof(unsigned int), 255, File);
			fwrite(&Iter->second->trackCount, sizeof(unsigned int), 255, File);

			for (unsigned int TrackIter = 0; TrackIter < Iter->second->trackCount; TrackIter++)
			{
				fwrite(&Iter->second->tracks[TrackIter].BoneIndex, sizeof(unsigned int), 1, File);
				fwrite(&Iter->second->tracks[TrackIter].KeyFrameCount, sizeof(unsigned int), 1, File);
				fwrite(Iter->second->tracks[TrackIter].KeyFrames, sizeof(keyFrame_t<type>), Iter->second->tracks[TrackIter].KeyFrameCount, File);
			}
		}

		fclose(File);
		return true;
	}

	void SaveNodeData(node_t<type>* Node, FILE* File)
	{
		unsigned int Address = (unsigned int)Node;
		fwrite(&Address, sizeof(unsigned int), 1, File);

		fwrite(&Node->nodeType, sizeof(unsigned int), 1, File);

		fwrite(&Node->name, sizeof(char), 255, File);

		Address = (unsigned int)Node->parent;
		fwrite(&Address, sizeof(unsigned int), 1, File);

		fwrite(&Node->localTransform, sizeof(type), 16, File);
		fwrite(&Node->globalTransform, sizeof(type), 16, File);

		switch (Node->nodeType)
		{
		case node_t<type>::TMESH:
		{
			SaveMeshData((meshNode_t<type>*)Node, File);
			break;
		}

		case node_t<type>::TLIGHT:
		{
			SaveLightData((lightNode_t<type>*)Node, File);
			break;
		}

		case node_t<type>::TCAMERA:
		{
			SaveCameraData((lightNode_t<type>*)Node, File);
			break;
		}

		default:
		{
			break;
		}
		}

		unsigned int ChildCount = Node->children.size();
		fwrite(&ChildCount, sizeof(unsigned int), 1, File);

		for (unsigned int ChildIter = 0; ChildIter < ChildCount; ChildIter++)
		{
			Address = (unsigned int)Node->children[ChildIter];
			fwrite(&Address, sizeof(unsigned int), 1, File);
		}

		for (unsigned int NodeIter = 0; NodeIter < Node->children.size(); NodeIter++)
		{
			SaveNodeData(Node->children[NodeIter], File);
		}
	}

	void SaveMeshData(meshNode_t<type>* Mesh, FILE* File)
	{
		unsigned int Address = (unsigned int)Mesh->material;
		fwrite(&Address, sizeof(unsigned int), 1, File);

		unsigned int VertexCount = Mesh->vertices.size();
		fwrite(VertexCount, sizeof(unsigned int), 1, File);

		fwrite(Mesh->vertices.data(), sizeof(vertex_t<type>), VertexCount, File);

		unsigned int IndexCount = Mesh->indices.size();
		fwrite(IndexCount, sizeof(unsigned int), 1, File);

		fwrite(Mesh->indices.data(), sizeof(unsigned int), IndexCount, File);
	}

	void SaveLightData(lightNode_t<type>* Light, FILE* File)
	{
		unsigned int Value = Light->lightType;
		fwrite(&Value, sizeof(unsigned int), 1, File);

		Value = Light->on ? 1 : 0;
		fwrite(&Value, sizeof(unsigned int), 1, File);

		fwrite(&Light->color, sizeof(type), 4, File);

		fwrite(&Light->innerAngle, sizeof(type), 1, File);
		fwrite(&Light->outerAngle, sizeof(type), 1, File);

		fwrite(&Light->attenuation, sizeof(type), 4, File);
	}

	void SaveCameraData(cameraNode_t<type>* Camera, FILE* File)
	{
		fwrite(&Camera->aspectRatio, sizeof(type), 1, File);
		fwrite(&Camera->FOV, sizeof(type), 1, File);
		fwrite(&Camera->near, sizeof(type), 1, File);
		fwrite(&Camera->far, sizeof(type), 1, File);

		fwrite(&Camera->viewMatrix, sizeof(type), 16, File);
	}

	bool LoadTinyModel(const char* FileName)
	{
		FILE* File = fopen(FileName, "rb");

		unsigned int I, J = 0;
		unsigned int Address = 0;
		unsigned int NodeType = node_t<type>::node;

		fread(&ambientLight, sizeof(type), 4, File);

		unsigned int MaterialCount, NodeCount, SkeletonCount, AnimationCount = 0;
		fread(&MaterialCount, sizeof(unsigned int), 1, File);

		std::map<unsigned int, material_t<type>*> MaterialMap;
		std::map<unsigned int, node_t<type>*> NodeMap;

		for (I = 0; I < MaterialCount; I++)
		{
			material_t<type>* Material = new material_t<type>();

			fread(&Address, sizeof(unsigned int), 1, File);

			fread(&Material, sizeof(material_t<type>), 1, File);

			MaterialMap[Address] = Material;
			materials[Material->name] = Material;
		}

		fread(&NodeCount, sizeof(unsigned int), 1, File);
		if (NodeCount > 0)
		{
			LoadNode(NodeMap, MaterialMap, File);
			ReLink(root, NodeMap);
		}

		fread(&SkeletonCount, sizeof(unsigned int), 1, File);
		for (I = 0; I < SkeletonCount; I++)
		{
			skeleton_t<type>* Skeleton = new skeleton_t<type>();

			fread(&Skeleton->boneCount, sizeof(unsigned int), 1, File);

			Skeleton->bindPoses = new type[Skeleton->boneCount][16];
			Skeleton->bones = new type[Skeleton->boneCount][16];
			Skeleton->nodes = new node_t<type> *[Skeleton->boneCount];

			fread(Skeleton->bindPoses, sizeof(type) * 16, Skeleton->boneCount, File);
			fread(Skeleton->nodes, sizeof(node_t<type>*), Skeleton->boneCount, File);

			for (J = 0; J < Skeleton->boneCount; J++)
			{
				Skeleton->nodes[J] = NodeMap[(unsigned int)Skeleton->nodes[J]];
			}

			skeletons.push_back(Skeleton);
		}

		fread(&AnimationCount, sizeof(unsigned int), 1, File);

		for (I = 0; I < AnimationCount; I++)
		{
			animation_t<type>* Animation = new animation_t<type>();
			
			fread(Animation->name, sizeof(char), 255, File);
			fread(&Animation->startFrame, sizeof(unsigned int), 1, File);
			fread(&Animation->endFrame, sizeof(unsigned int), 1, File);
			fread(&Animation->trackCount, sizeof(unsigned int), 1, File);

			Animation->tracks = new track_t<type>[Animation->trackCount];

			for (J = 0; J < Animation->trackCount; J++)
			{
				fread(&Animation->tracks[J].BoneIndex, sizeof(unsigned int), 1, File);
				fread(&Animation->tracks[J].KeyFrameCount, sizeof(unsigned int), 1, File);

				Animation->tracks[J].KeyFrames = new keyFrame_t<type>[Animation->tracks[J].KeyFrameCount];
				fread(Animation->tracks[J].KeyFrames, sizeof(keyFrame_t<type>), Animation->tracks[J].KeyFrameCount, File);
			}

			animations[Animation->name] = Animation;
		}
		fclose(File);
		return true;
	}

	void LoadNode(std::map<unsigned int, node_t<type>*>& Nodes,
		std::map<unsigned int, material_t<type>*>& MaterialMap, FILE* File)
	{
		unsigned int Address = 0;
		unsigned int Parent = 0;
		unsigned int NodeType = node_t<type>::node;

		fread(Address, sizeof(unsigned int), 1, File);
		fread(NodeType, sizeof(unsigned int), 1, File);
		
		char Name[255];
		fread(Name, sizeof(char), 255, File);

		fread(&Parent, sizeof(unsigned int), 1, File);

		type LocalTransform[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
		type GlobalTransform[16] = LocalTransform;

		fread(LocalTransform, sizeof(type), 16, File);
		fread(GlobalTransform, sizeof(type), 16, File);

		node_t<type>* TinyNode = nullptr;

		switch (NodeType)
		{
		case node_t<type>::node:
		{
			TinyNode = new node_t<type>();
			break;
		}

		case node_t<type>::TMESH:
		{
			TinyNode = new meshNode_t<type>();
			LoadMeshData((meshNode_t<type>*)TinyNode, MaterialMap, File);
			break;
		}

		case node_t<type>::TLIGHT:
		{
			TinyNode = new lightNode_t<type>();
			LoadLightData((lightNode_t<type>*)TinyNode, File);
			break;
		}

		case node_t<type>::TCAMERA:
		{
			TinyNode = new cameraNode_t<type>();
			LoadCameraData((cameraNode_t<type>*)TinyNode, File);
			break;
		}

		default:
		{
			break;
		}
		}

		TinyNode->localTransform = LocalTransform;
		TinyNode->globalTransform = GlobalTransform;
		Nodes[Address] = TinyNode;

		TinyNode->parent = (node_t<type>*)Parent;

		if (root == nullptr)
		{
			root = TinyNode;
		}
		
		TinyNode->nodeType = NodeType;
		strncpy(TinyNode->name, Name, 255);

		switch (NodeType)
		{
		case node_t<type>::TMESH:
		{
			meshes[TinyNode->name] = (meshNode_t<type>*)TinyNode;
			break;
		}

		case node_t<type>::TLIGHT:
		{
			lights[TinyNode->name] = (lightNode_t<type>*)TinyNode;
			break;
		}

		case node_t<type>::TCAMERA:
		{
			cameras[TinyNode->name] = (cameraNode_t<type>*)TinyNode;
			break;
		}

		default:
		{
			break;
		}
		}

		unsigned int ChildCount = 0;
		fread(&ChildCount, sizeof(unsigned int), 1, File);

		unsigned int ChildIter = 0;

		for (ChildIter = 0; ChildIter < ChildCount; ChildIter++)
		{
			fread(&Address, sizeof(unsigned int), 1, File);
			TinyNode->children.push_back((node_t<type>*)Address);
		}

		for (ChildIter = 0; ChildIter < ChildCount; ChildIter++)
		{
			LoadNode(Nodes, MaterialMap, File);
		}
	}

	void LoadMeshData(meshNode_t<type>* Mesh, std::map<unsigned int, material_t<type>*> MaterialMap, FILE* File)
	{
		unsigned int Address = 0;
		fread(&Address, sizeof(unsigned int), 1, File);
		Mesh->material = MaterialMap[Address];

		unsigned int VertexCount = 0;
		fread(&VertexCount, sizeof(unsigned int), 1, File);

		if (VertexCount > 0)
		{
			vertex_t<type>* Vertices = new vertex_t<type>[VertexCount];
			fread(Vertices, sizeof(vertex_t<type>), VertexCount, File);
			for (unsigned int VertexIter = 0; VertexIter < VertexCount; VertexIter++)
			{
				Mesh->vertices.push_back(Vertices[VertexIter]);
			}
			delete[] Vertices;
		}

		unsigned int IndexCount = 0;
		fread(&IndexCount, sizeof(unsigned int), 1, File);

		if (IndexCount > 0)
		{
			unsigned int* Indices = new unsigned int[IndexCount];
			fread(Indices, sizeof(unsigned int), IndexCount, File);
			for (unsigned int IndexIter = 0; IndexIter < IndexIter; IndexIter++)
			{
				Mesh->indices.push_back(Indices[IndexIter]);
			}
			delete[] Indices;
		}

	}

	void LoadLightData(lightNode_t<type>* Light, FILE* File)
	{
		unsigned int Value = 0;
		fread(&Value, sizeof(unsigned int), 1, File);
		Light->lightType = Value;

		Value = 0;
		fread(&Value, sizeof(unsigned int), 1, File);
		Light->lightType = Value != 0;

		fread(&Light->color, sizeof(type), 4, File);

		fread(&Light->innerAngle, sizeof(type), 1, File);
		fread(&Light->outerAngle, sizeof(type), 1, File);

		fread(&Light->attenuation, sizeof(type), 4, File);
	}

	void LoadCameraData(cameraNode_t<type>* Camera, FILE* File)
	{
		fread(&Camera->aspectRatio, sizeof(type), 1, File);
		fread(&Camera->FOV, sizeof(type), 1, File);
		fread(&Camera->near, sizeof(type), 1, File);
		fread(&Camera->far, sizeof(type), 1, File);

		fread(&Camera->viewMatrix, sizeof(type), 16, File);
	}

	void ReLink(node_t<type>* Node, std::map<unsigned int, node_t<type>*>& Nodes)
	{
		if (Node->parent != nullptr)
		{
			Node->parent = Nodes[(unsigned int)Node->parent];
		}
		unsigned int ChildCount = Node->children.size();
		for (unsigned int ChildIter = 0; ChildIter < ChildCount; ChildIter++)
		{
			Node->children[ChildIter] = Nodes[(unsigned int)Node->children[ChildIter]];
			ReLink(Node->children[ChildIter], Nodes);
		}
	}

	struct importAssistor_t
	{
		importAssistor_t() : currentScene(nullptr), evaluator(nullptr)
		{
			//Evaluator = new FbxAnimEvaluator();
		}
		~importAssistor_t()
		{
			evaluator = nullptr;
			currentScene = nullptr;
		}

		FbxScene* currentScene;
		FbxAnimEvaluator* evaluator;
		std::vector<node_t<type>*> bones;

		std::map<const char*, unsigned int> boneIndexMap;
	};

	node_t<type>* root;

	char* path;
	type ambientLight[4];

	importAssistor_t* assistor;

	std::map<const char*, meshNode_t<type>*> meshes;
	std::map<const char*, lightNode_t<type>*> lights;
	std::map<const char*, cameraNode_t<type>*> cameras;
	std::map<const char*, material_t<type>*> materials;
	std::map<const char*, animation_t<type>*> animations;

	std::vector<skeleton_t<type>*> skeletons;
};

/*template<typename type>
class ModelManager
{
public:



	ModelManager()
	{

	}

	ImportAssistor* GetImportAssistor
	{
		return GetInstance()->Assistor;
	}

	static ModelManager* GetInstance()
	{
		if (ModelManager::Instance == nullptr)
		{
			ModelManager::Instance = new ModelManager();
			return ModelManager::Instance;
		}
		else
		{
			return ModelManager::Instance;
		}
	}

	static void Initalize()
	{
		GetInstance()->Assistor = nullptr;
	}

private:
	static ModelManager* Instance;
	ImportAssistor* Assistor;

	std::map<const char*, TScene<type>*> Scenes;

	void DisplayContent(TScene<type>* Scene);
	void DisplayHierarchy(TScene<type>* Scene);
	void DisplayPose(TScene<type>* Scene);
	void DisplayAnimation(TScene<type>* Scene);
};

template<typename type>
ModelManager<type>* ModelManager<type>::Instance = nullptr;*/
#endif
