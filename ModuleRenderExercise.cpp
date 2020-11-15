#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "GL/glew.h"
#include "il.h"


SDL_Event event;
ILuint texid; /* ILuint is a 32bit unsigned integer.
  Variable texid will be used to store image name. */
ILboolean success; /* ILboolean is type similar to GLboolean and can equal GL_FALSE (0) or GL_TRUE (1)
  it can have different value (because it's just typedef of unsigned char), but this sould be
  avoided.
  Variable success will be used to determine if some function returned success or failure. */
GLuint image;
int finished;

ModuleRenderExercise::ModuleRenderExercise()
{
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
	App->program->DestroyVBO(triangle);
}

// Called before RenderExercise is available
bool ModuleRenderExercise::Init()
{
	LOG("Creating Renderer context");

	glewInit();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();
	// … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	triangle = App->program->CreateTriangleVBO();

	unsigned int vertex_id = App->program->CompileShader(GL_VERTEX_SHADER, App->program->LoadShaderSource("Shaders/Vertex.vert"));
	unsigned int fragment_id = App->program->CompileShader(GL_FRAGMENT_SHADER, App->program->LoadShaderSource("Shaders/Fragment.frag"));

	program = App->program->CreateProgram(vertex_id, fragment_id);



	ilInit(); /* Initialization of DevIL */
	ILuint();

	ilGenImages(1, &texid); /* Generation of one image name */
	ilBindImage(texid); /* Binding of image name */

	success = ilLoadImage("Textures/Lenna.png"); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			SDL_Quit();
			return -1;
		}
		glGenTextures(1, &texid); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, texid); /* Binding of texture name */
		
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */
	}
	else
	{
		/* Error occured */
		SDL_Quit();
		return -1;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texid);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
	ilDeleteImages(1, &texid); /* Because we have already copied image data into texture data
	  we can release memory used by image. */

	App->program->RenderVBO(triangle, program);
	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{

	int w = 0;
	int h = 0;
	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	///* Draw a quad */
	//glBegin(GL_QUADS);
	//glTexCoord2i(0, 0); glVertex2i(0, 0);
	//glTexCoord2i(0, 1); glVertex2i(0, 480);
	//glTexCoord2i(1, 1); glVertex2i(640, 480);
	//glTexCoord2i(1, 0); glVertex2i(640, 0);
	//glEnd();
	App->program->RenderVBO(triangle, program);
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying RenderExercise");
	glDeleteTextures(1, &image);
	SDL_Quit();
	return true;
}