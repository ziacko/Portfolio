#ifndef TEXTURE_H
#define TEXTURE_H

class texture
{
public:

	texture(const char* path = "../../resources/textures/earth_diffuse.tga", const char* uniformName = "defaultTexture",
		GLint internalFormat = GL_RGBA, GLenum target = GL_TEXTURE_2D,
		GLint currentMipmapLevel = 0, GLint mipmapLevels = 0, GLint border = 0, GLenum dataType = GL_UNSIGNED_BYTE,
		GLenum internalDataType = GL_RGBA32F, GLint xOffset = 0, GLint yOffset = 0,
		GLenum minFilterSetting = GL_LINEAR, GLenum magFilterSetting = GL_LINEAR,
		GLenum wrapSSetting = GL_CLAMP_TO_EDGE, GLenum wrapTSetting = GL_CLAMP_TO_EDGE, GLenum wrapRSetting = GL_CLAMP_TO_EDGE)
	{
		this->path = path;
		this->uniformName = uniformName;
		this->width = 0;
		this->height = 0;
		this->channels = 0;
		this->format = 0;
		this->internalFormat = internalFormat;
		this->target = target;
		this->currentMipmapLevel = currentMipmapLevel;
		this->mipmapLevels = mipmapLevels;
		this->border = border;
		this->dataType = dataType;
		this->internalDataType = internalDataType;
		this->xOffset = xOffset;
		this->yOffset = yOffset;

		this->minFilterSetting = minFilterSetting;
		this->magFilterSetting = magFilterSetting;
		this->wrapSSetting = wrapSSetting;
		this->wrapTSetting = wrapTSetting;
		this->wrapRSetting = wrapRSetting;

		this->handle = 0;

		//LoadTexture();
	}

	void BindTexture()
	{
		glBindTexture(target, handle);
	}

	void GetUniformLocation(GLuint programHandle)
	{
		uniformHandle = glGetUniformLocation(programHandle, uniformName);
		glUniform1i(uniformHandle, handle);

		glActiveTexture(GL_TEXTURE0 + handle);
		glBindTexture(target, handle);
	}

	void UnbindTexture()
	{
		glBindTexture(target, NULL);
	}

	void LoadTexture()
	{
		stbi_set_flip_vertically_on_load(true);
		char* data = (char*)stbi_load(path, &this->width, &this->height, &channels, 0);

		switch (channels)
		{
		case 1:
		{
			format = GL_R;
			break;
		}

		case 2:
		{
			format = GL_RG;
			break;
		}

		case 3:
		{
			format = GL_RGB;
			break;
		}

		case 4:
		{
			format = GL_RGBA;
			break;
		}
		}

		/*FIBITMAP* bitmap = nullptr;
		//printf(" error: could not load bitmap: %s \n", path);
		FREE_IMAGE_FORMAT FIformat = FreeImage_GetFileType(path, 0);

		if (FIformat != FIF_UNKNOWN && FreeImage_FIFSupportsReading(FIformat))
		{
			bitmap = FreeImage_Load(FIformat, path);
		}

		if (bitmap == nullptr)
		{
			printf(" error: could not load bitmap: %s \n", path);
			return;
		}

		if (width == NULL)
		{
			width = FreeImage_GetWidth(bitmap);
		}

		if (height == NULL)
		{
			height = FreeImage_GetHeight(bitmap);
		}

		bitsPerPixel = FreeImage_GetBPP(bitmap);

		FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(bitmap);

		if (colorType != FIC_RGBALPHA)
		{
			FIBITMAP* newBitMap = FreeImage_ConvertTo32Bits(bitmap);
			FreeImage_Unload(bitmap);
			bitmap = newBitMap;
			bitsPerPixel = FreeImage_GetBPP(bitmap);
			colorType = FreeImage_GetColorType(bitmap);
		}

		data = (char*)FreeImage_GetBits(bitmap);
		FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);*/

		//data = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_RGBA);
		//glGetStringi(GL_EXTENSIONS, 1);
		//handle = SOIL_load_OGL_texture(path, SOIL_LOAD_RGBA, handle, SOIL_FLAG_INVERT_Y);
		//gli::texture test(gli::load(path));
		//gli::load

		glGenTextures(1, &handle);
		glBindTexture(target, handle);

		switch (target)
		{
		case GL_TEXTURE_1D:
		{
			break;
		}

		case GL_TEXTURE_1D_ARRAY:
		{
			break;
		}

		case GL_TEXTURE_2D:
		{
			//glTexStorage2D(target, currentMipmapLevel, internalDataType, width, height);
			//glTexSubImage2D(target, currentMipmapLevel, xOffset, yOffset, width, height, format, dataType, data);

			glTexImage2D(target, mipmapLevels, internalFormat, width, height, border, format, dataType, data);

			glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapSSetting);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapTSetting);
			break;
		}

		case GL_TEXTURE_2D_ARRAY:
		{
			break;
		}

		case GL_TEXTURE_2D_MULTISAMPLE:
		{
			break;
		}

		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
		{
			break;
		}

		case GL_TEXTURE_3D:
		{
			break;
		}

		case GL_TEXTURE_BUFFER:
		{
			break;
		}

		case GL_TEXTURE_RECTANGLE:
		{
			break;
		}

		case GL_TEXTURE_CUBE_MAP:
		{
			break;
		}

		case GL_TEXTURE_CUBE_MAP_ARRAY:
		{
			break;
		}

		default:
		{

			break;
		}
		}

		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilterSetting);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilterSetting);

		UnbindTexture();
	}

	void SetMinFilter(GLenum minFilterSetting)
	{
		switch (minFilterSetting)
		{
		case 0:
		{
			this->minFilterSetting = GL_LINEAR;
			break;
		}

		case 1:
		{
			this->minFilterSetting = GL_NEAREST;
			break;
		}
		}
		BindTexture();
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, this->minFilterSetting);
		UnbindTexture();
	}

	void SetMagFilter(GLenum magFilterSetting)
	{
		switch (magFilterSetting)
		{
			case 0:
			{
				this->magFilterSetting = GL_LINEAR;
				break;
			}

			case 1:
			{
				this->magFilterSetting = GL_NEAREST;
				break;
			}
		}

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, this->magFilterSetting);
		UnbindTexture();
	}

	void SetWrapS(GLenum wrapSetting)
	{
		switch (wrapSetting)
		{
			case 0:
			{
				this->wrapSSetting = GL_CLAMP_TO_EDGE;
				break;
			}

			case 1:
			{
				this->wrapSSetting = GL_MIRROR_CLAMP_TO_EDGE;
				break;
			}

			case 2:
			{
				this->wrapSSetting = GL_CLAMP_TO_BORDER;
				break;
			}

			case 3:
			{
				this->wrapSSetting = GL_REPEAT;
				break;
			}

			case 4:
			{
				this->wrapSSetting = GL_MIRRORED_REPEAT;
				break;
			}
		}

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_WRAP_S, this->wrapSSetting);
		UnbindTexture();
	}

	void SetWrapT(GLenum wrapSetting)
	{
		switch (wrapSetting)
		{
			case 0:
			{
				this->wrapTSetting = GL_CLAMP_TO_EDGE;
				break;
			}

			case 1:
			{
				this->wrapTSetting = GL_MIRROR_CLAMP_TO_EDGE;
				break;
			}

			case 2:
			{
				this->wrapTSetting = GL_CLAMP_TO_BORDER;
				break;
			}

			case 3:
			{
				this->wrapTSetting = GL_REPEAT;
				break;
			}

			case 4:
			{
				this->wrapTSetting = GL_MIRRORED_REPEAT;
				break;
			}
		}

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_WRAP_T, this->wrapTSetting);
		UnbindTexture();
	}

	void SetWrapR(GLenum wrapSetting)
	{
		switch (wrapSetting)
		{
			case 0:
			{
				this->wrapRSetting = GL_CLAMP_TO_EDGE;
				break;
			}

			case 1:
			{
				this->wrapRSetting = GL_MIRROR_CLAMP_TO_EDGE;
				break;
			}

			case 2:
			{
				this->wrapRSetting = GL_CLAMP_TO_BORDER;
				break;
			}

			case 3:
			{
				this->wrapRSetting = GL_REPEAT;
				break;
			}

			case 4:
			{
				this->wrapRSetting = GL_MIRRORED_REPEAT;
				break;
			}
		}

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_WRAP_R, this->wrapRSetting);
		UnbindTexture();
	}

protected:

	//gli::texture	data;

	GLuint			handle;
	const char*		path;
	GLuint			uniformHandle;
	const char*		uniformName;

	GLint			width;
	GLint			height;
	GLenum			format;
	GLint			internalFormat;
	GLenum			target;
	GLint			currentMipmapLevel;
	GLint			mipmapLevels;
	GLint			border;
	GLenum			dataType;
	GLenum			internalDataType;
	GLint			xOffset;
	GLint			yOffset;

	GLint			channels;

	GLenum			minFilterSetting;
	GLenum			magFilterSetting;
	GLenum			wrapSSetting;
	GLenum			wrapTSetting;
	GLenum			wrapRSetting;

	GLuint			bitsPerPixel;
	char*			data;
};
#endif
