#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "il.h"
#include "ilu.h"

ModuleTexture::ModuleTexture()
{

}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{
	ilInit(); /* Initialization of DevIL */
	iluInit();

	ILuint texid;

	ilGenImages(1, &texid); /* Generation of one image name */
	ilBindImage(texid); /* Binding of image name */

	ILboolean success;
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
		glGenTextures(1, &textureID); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, textureID); /* Binding of texture name */

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /* We will use linear
		  interpolation for minifying filter */
		ilDeleteImage(texid);
	}
	return true;
}

// Called every draw update
update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	return true;
}