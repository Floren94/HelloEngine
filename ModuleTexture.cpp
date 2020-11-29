#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleEditor.h"
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
	unsigned textureID;

	success = ilLoadImage(file_name);
	if (success == IL_FALSE)
	{
		// texture not in ./
		App->editor->AddLogToConsole("Failed to locate texture \n");
		bool checkString = false;
		std::string texturePath = file_name;
		std::string fbxpath = App->input->GetDropPath();
		App->editor->AddLogToConsole("Searching texture at fbx directory\n");
		if (fbxpath.find('\\') != std::string::npos) {
			fbxpath = fbxpath.substr(0, fbxpath.find('.')) + texturePath.substr(texturePath.find('.'), texturePath.length() - 1);
			success = ilLoadImage(fbxpath.c_str());
		}

		if (success == IL_FALSE) {
			App->editor->AddLogToConsole("Failed to find texture at fbx directory\n");
			App->editor->AddLogToConsole("Searching texture at /Assets\n");
			if (texturePath.find("\\") != std::string::npos && success == IL_FALSE) {
				checkString = true;
				texturePath = ".//Assets//" + texturePath.substr(texturePath.find_last_of("\\", texturePath.length() - 1));
			}
			else {
				texturePath = ".//Assets//" + texturePath;
			}
			success = ilLoadImage(texturePath.c_str());
		}

		if (success == IL_FALSE)
		{
			App->editor->AddLogToConsole("Failed to find texture at ./Assets directory\n");
			App->editor->AddLogToConsole("Searching texture at /Textures\n");
			texturePath = file_name;

			if (checkString) {
				texturePath = ".//Game//Textures//" + texturePath.substr(texturePath.find_last_of("\\", texturePath.length() - 1));
			}
			else {
				texturePath = ".//Textures//" + texturePath;
			}
			success = ilLoadImage(texturePath.c_str());

			if(success == IL_FALSE) App->editor->AddLogToConsole("--Failed to load Texture\n");;
		}
	}
	if (success == IL_TRUE)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		App->editor->AddLogToConsole("Texture Loaded\n");
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

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	ilDeleteImage(texid);
	return textureID;
}


void ModuleTexture::FreeTexture(unsigned textID) {
	glDeleteTextures(1, &textID);
}