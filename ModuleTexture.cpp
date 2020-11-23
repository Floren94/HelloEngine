#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "il.h"
#include "ilu.h"

// Called before render is available
bool ModuleTexture::Init()
{
	ilInit(); /* Initialization of DevIL */
	iluInit();

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

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

unsigned ModuleTexture::LoadTexture(const char* file_name) {
	ILuint texid;

	ILinfo info;

	ilGenImages(1, &texid);
	ilBindImage(texid);

	ILboolean success;
	//ILenum Error;
	unsigned textureID;

	success = ilLoadImage(file_name);
	if (success == IL_TRUE)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	}
	if (success == IL_FALSE)
	{
		LOG("Error loading texture...");
	}

	iluGetImageInfo(&info);
	if (info.Origin == IL_ORIGIN_UPPER_LEFT) {
		iluFlipImage();
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ilDeleteImage(texid);
	return textureID;
}

void ModuleTexture::FreeTexture(unsigned textID) {
	glDeleteTextures(1, &textID);
}