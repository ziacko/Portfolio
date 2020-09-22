#ifndef CELL_SHADING_H
#define CELL_SHADING_H
#include <TexturedScene.h>

struct cellShadeSettings_t
{
	float		redModifier;
	float		greenModifier;
	float		blueModifier;
	float		cellDistance;

	cellShadeSettings_t(GLfloat cellDistance = 1.0f, GLfloat redModifier = 1.0f,
		GLfloat greenModifier = 1.0f, GLfloat blueModifier = 1.0f)
	{
		this->redModifier = redModifier;
		this->greenModifier = greenModifier;
		this->blueModifier = blueModifier;
		this->cellDistance = cellDistance;
	}

	~cellShadeSettings_t(){};
};

class cellShadingScene : public texturedScene
{
public:

	cellShadingScene(bufferHandler_t<cellShadeSettings_t> cellShade = bufferHandler_t<cellShadeSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (Cell Shading)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/CellShading.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->cellBuffer = cellShade;
	}

	~cellShadingScene(){};

protected:

	bufferHandler_t<cellShadeSettings_t>		cellBuffer;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::SliderFloat("red modifier", &cellBuffer.data.redModifier, 0.0f, 1.0f);
		ImGui::SliderFloat("green modifier", &cellBuffer.data.greenModifier, 0.0f, 1.0f);
		ImGui::SliderFloat("blue modifier", &cellBuffer.data.blueModifier, 0.0f, 1.0f);
		ImGui::SliderFloat("cell distance", &cellBuffer.data.cellDistance, 0.0f, 1.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		cellBuffer.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		cellBuffer.Update(gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif
