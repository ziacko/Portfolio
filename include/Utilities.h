#ifndef UTILITIES_H
#define UTILITIES_H
/* Returns a list of files in a directory (except the ones that begin with a dot) */

void GetFilesInDirectory(std::vector<std::string>& output, std::string directory)
{
#if defined(TW_WINDOWS)
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	std::string file_name;
	std::string full_file_name;
	

	//call new every time?
	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
	{
		return; /* No files found */
	}

	do
	{
		file_name = file_data.cFileName;
		full_file_name = directory + "/" + file_name;

		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
		{
			//delete full_file_name;
			continue;
		}

		if (is_directory)
		{
			//delete full_file_name;
			continue;
		}
		output.push_back(full_file_name);
	} 
	
	while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	/*DIR *dir;
	class dirent *ent;
	class stat* st;

	dir = opendir(directory.c_str());
	while ((ent = readdir(dir)) != NULL) {
		const std::string file_name = ent->d_name;
		const std::string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	}
	closedir(dir);*/
#endif
} // GetFilesInDirectory

/*
GLuint LoadTexture(const char* texture, GLuint format = GL_RGBA, GLuint* width = nullptr, GLuint* height = nullptr)
{
	FIBITMAP* bitmap = nullptr;

	FREE_IMAGE_FORMAT FIFormat = FreeImage_GetFileType(texture, 0);

	if (FIFormat != FIF_UNKNOWN && FreeImage_FIFSupportsReading(FIFormat))
	{
		bitmap = FreeImage_Load(FIFormat, texture);
	}

	if (bitmap == nullptr)
	{
		return 0;
	}

	if (width != nullptr)
	{
		*width = FreeImage_GetWidth(bitmap);
	}

	if (height != nullptr)
	{
		*height = FreeImage_GetHeight(bitmap);
	}

	GLuint bitsPerPixel = FreeImage_GetBPP(bitmap);

	FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(bitmap);

	if (colorType != FIC_RGBALPHA)
	{
		FIBITMAP* newBitMap = FreeImage_ConvertTo32Bits(bitmap);
		FreeImage_Unload(bitmap);
		bitmap = newBitMap;
		bitsPerPixel = FreeImage_GetBPP(bitmap);
		colorType = FreeImage_GetColorType(bitmap);
	}

	BYTE* data = FreeImage_GetBits(bitmap);

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);

	GLenum type = (imageType == FIT_RGBF || imageType == FIT_FLOAT) ? GL_FLOAT : GL_UNSIGNED_BYTE;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), 0, format, type, data);

	//just need a linear min and mag filter for out textures for now
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//set the texture wrapping to edge clamping
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_clamp_to_edge);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_clamp_to_edge);

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(bitmap);

	return textureID;
}
*/

#endif
