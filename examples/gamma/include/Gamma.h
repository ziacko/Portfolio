#ifndef GAMMA_H
#define GAMMA_H
#include <TexturedScene.h>

class gammaScene : public texturedScene
{
public:

	gammaScene(
		glm::vec3 gammaSettings = glm::vec3(1.2, 3.7, 2.0),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (gamma)",
		camera* gammaCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Gamma.txt")
		: texturedScene(defaultTexture, windowName, gammaCamera, shaderConfigPath)
	{
		this->gamma = bufferHandler_t<glm::vec3>(gammaSettings);
	}

	~gammaScene(){};

protected:

	bufferHandler_t<glm::vec3>		gamma;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::SliderFloat("gamma red", &gamma.data.r, 0.f, 10.0f);
		ImGui::SliderFloat("gamma green", &gamma.data.g, 0.0f, 10.0f);
		ImGui::SliderFloat("gamma blue", &gamma.data.b, 0.0f, 10.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		gamma.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		gamma.Update(gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif