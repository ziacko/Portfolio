#ifndef UTILITIES_H
#define UTILITIES_H

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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(bitmap);

	return textureID;
}

#endif