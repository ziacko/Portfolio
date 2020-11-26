#ifndef COMMAND_H
#define COMMAND_H

#include <Scene3D.h>

class commandScene : public scene3D
{
public:

	commandScene(const char* windowName = "Ziyad Barakat's Portfolio(command)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 200.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/Model.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX")) :
		scene3D(windowName, camera3D, shaderConfigPath, model)
	{
		testModel = model;
		//materialSettingsBuffer = new baseMaterialSettings_t();
		wireframe = false;
	}

	~commandScene() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();
	}

protected:
	
	camera::up cameraUpProc;
	camera::down cameraDownProc;
	camera::left cameraLeftProc;
	camera::right cameraRightProc;
	camera::forwards cameraForwardsProc;
	camera::backwards cameraBackwardsProc;


};

#endif