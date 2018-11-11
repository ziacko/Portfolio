#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <sstream>
#include <fstream>

#include <GL/glew.h>

#include "glm-0.9.2.6/glm/glm.hpp"
#include "glm-0.9.2.6/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.2.6/glm/gtc/type_ptr.hpp"

#include "src/keyboard.h"
#include "src/joystick.h"
#include "src/glhelper.h"
#include "src/timer.h"
#include "src/misc.h"

#define WIDTH  1280
#define HEIGHT 720

int main(int argc, char *argv[]) {
	// input devices
	cKeyboard kb;
	cJoystick js; joystick_position jp[2];

	// timers
	cTimer t0; double elapsed0;
	cTimer t1; double elapsed1;

	// buffer for screen grabs
	unsigned char *buffer = new unsigned char[WIDTH * HEIGHT * 4];

	// application is active.. fullscreen flag.. screen grab.. video grab..
	bool active = true, fullscreen = false, grab = false, video = false;

	// setup an opengl context
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Surface *screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, (fullscreen ? SDL_FULLSCREEN : 0) | SDL_HWSURFACE | SDL_OPENGL);

	// initialize the extension wrangler
	glewInit();

	// for handling events
	SDL_Event event;

	// some output
	std::cout << glGetString(GL_VERSION)<< std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION)<< std::endl;
	std::cout << glewGetString(GLEW_VERSION)<< std::endl;
	std::cout << glGetString(GL_EXTENSIONS)<< std::endl;

	// set up the cube map texture
	SDL_Surface *xpos = IMG_Load("media/xpos.png");	SDL_Surface *xneg = IMG_Load("media/xneg.png");
	SDL_Surface *ypos = IMG_Load("media/ypos.png");	SDL_Surface *yneg = IMG_Load("media/yneg.png");
	SDL_Surface *zpos = IMG_Load("media/zpos.png");	SDL_Surface *zneg = IMG_Load("media/zneg.png");
	GLuint cubemap_texture;
	setupCubeMap(cubemap_texture, xpos, xneg, ypos, yneg, zpos, zneg);
	SDL_FreeSurface(xneg);	SDL_FreeSurface(xpos);
	SDL_FreeSurface(yneg);	SDL_FreeSurface(ypos);
	SDL_FreeSurface(zneg);	SDL_FreeSurface(zpos);

	// set our viewport, clear color and depth, and enable depth testing
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// load our shaders and compile them.. create a program and link it
	GLuint glShaderV = glCreateShader(GL_VERTEX_SHADER);
	GLuint glShaderF = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* vShaderSource = loadFile("src/vertex.sh");
	const GLchar* fShaderSource = loadFile("src/fragment.sh");
	glShaderSource(glShaderV, 1, &vShaderSource, NULL);
	glShaderSource(glShaderF, 1, &fShaderSource, NULL);
	delete [] vShaderSource;
	delete [] fShaderSource;
	glCompileShader(glShaderV);
	glCompileShader(glShaderF);
	GLuint glProgram = glCreateProgram();
	glAttachShader(glProgram, glShaderV);
	glAttachShader(glProgram, glShaderF);
	glLinkProgram(glProgram);
	glUseProgram(glProgram);

	// shader logs
	int  vlength,    flength;
	char vlog[2048], flog[2048];
	glGetShaderInfoLog(glShaderV, 2048, &vlength, vlog);
	glGetShaderInfoLog(glShaderF, 2048, &flength, flog);
	std::cout << vlog << std::endl << std::endl << flog << std::endl << std::endl;

	// grab the pvm matrix and vertex location from our shader program
	GLint PVM    = glGetUniformLocation(glProgram, "PVM");
	GLint vertex = glGetAttribLocation(glProgram, "vertex");

	// these won't change for now
	glm::mat4 Projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); 
	glm::mat4 View       = glm::mat4(1.0f);
	glm::mat4 Model      = glm::scale(glm::mat4(1.0f),glm::vec3(50,50,50));

	// cube vertices for vertex buffer object
	GLfloat cube_vertices[] = {
	  -1.0,  1.0,  1.0,
	  -1.0, -1.0,  1.0,
	   1.0, -1.0,  1.0,
	   1.0,  1.0,  1.0,
	  -1.0,  1.0, -1.0,
	  -1.0, -1.0, -1.0,
	   1.0, -1.0, -1.0,
	   1.0,  1.0, -1.0,
	};
	GLuint vbo_cube_vertices;
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// cube indices for index buffer object
	GLushort cube_indices[] = {
	  0, 1, 2, 3,
	  3, 2, 6, 7,
	  7, 6, 5, 4,
	  4, 5, 1, 0,
	  0, 3, 7, 4,
	  1, 2, 6, 5,
	};
	GLuint ibo_cube_indices;
	glGenBuffers(1, &ibo_cube_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// rotation angles
	float alpha = 0.0f, beta = 0.0f;

	while(active) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				active = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_g:
					grab = true;
					break;
				case SDLK_v:
					video ^= true; elapsed1 = 0.0;
					break;
				case SDLK_f:
					fullscreen ^= true;
					screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, (fullscreen ? SDL_FULLSCREEN : 0) | SDL_HWSURFACE | SDL_OPENGL);
					break;
				}
				break;
			}
		}

		// time elapsed since last frame
		elapsed0 = t0.elapsed(true);

		// update frame based on input state
		if (kb.getKeyState(KEY_UP))    alpha += 180.0f*elapsed0;
		if (kb.getKeyState(KEY_DOWN))  alpha -= 180.0f*elapsed0;
		if (kb.getKeyState(KEY_LEFT))  beta  -= 180.0f*elapsed0;
		if (kb.getKeyState(KEY_RIGHT)) beta  += 180.0f*elapsed0;
		jp[0] = js.joystickPosition(0);
		alpha += jp[0].y*elapsed0*180.0f;
		beta  += jp[0].x*elapsed0*180.0f;
		//jp[1] = js.joystickPosition(1); 

		// rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// cube map texture should already be bound
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), alpha, glm::vec3(-1.0f, 0.0f, 0.0f));
		glm::mat4 RotateY = glm::rotate(RotateX, beta, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 M = Projection * View * Model * RotateY;
		glUniformMatrix4fv(PVM, 1, GL_FALSE, glm::value_ptr(M));

		// vertex buffer should already be bound
		//glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		//glEnableVertexAttribArray(vertex);
		//glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// index buffer should already be bound
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
		glDrawElements(GL_QUADS, sizeof(cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		SDL_GL_SwapBuffers();

		if (grab) {
			grab = false;
			saveTGA(buffer, WIDTH, HEIGHT);
		}

		if (video) {
			elapsed1 += t1.elapsed(true);
			if (elapsed1 >= 1/24.0) {
				saveTGA(buffer, WIDTH, HEIGHT, true);
				elapsed1 = 0.0;
			}
		}
	}

	// release vertex and index buffer object
	glDeleteBuffers(1, &ibo_cube_indices);
	glDeleteBuffers(1, &vbo_cube_vertices);

	// release cube map
	deleteCubeMap(cubemap_texture);

	// detach shaders from program and release
	glDetachShader(glProgram, glShaderF);
	glDetachShader(glProgram, glShaderV);
	glDeleteShader(glShaderF);
	glDeleteShader(glShaderV);
	glDeleteProgram(glProgram);

	SDL_Quit();

	delete[] buffer;

	return 0;
}