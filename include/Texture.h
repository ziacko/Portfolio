#ifndef TEXTURE_H
#define TEXTURE_H

class texture
{
public:

	texture(const char* path = "../../resources/textures/earth_diffuse.tga", const char* uniformName = "defaultTexture",
		GLint internalFormat = GL_RGBA, GLenum target = GL_TEXTURE_2D,
		GLint currentMipmapLevel = 0, GLint mipmapLevels = 0, GLint border = 0, GLenum dataType = GL_UNSIGNED_BYTE,
		GLenum internalDataType = gl_rgba32f, GLint xOffset = 0, GLint yOffset = 0,
		GLenum minFilterSetting = GL_LINEAR, GLenum magFilterSetting = GL_LINEAR,
		GLenum wrapSSetting = gl_clamp_to_edge, GLenum wrapTSetting = gl_clamp_to_edge, GLenum wrapRSetting = gl_clamp_to_edge)
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

		glActiveTexture(gl_texture0 + handle);
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
			format = gl_rg;
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

		case gl_texture_1d_array:
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

		case gl_texture_2d_array:
		{
			break;
		}

		case gl_texture_2d_multisample:
		{
			break;
		}

		case gl_texture_2d_multisample_array:
		{
			break;
		}

		case gl_texture_3d:
		{
			break;
		}

		case gl_texture_buffer:
		{
			break;
		}

		case gl_texture_rectangle:
		{
			break;
		}

		case gl_texture_cube_map:
		{
			break;
		}

		case gl_texture_cube_map_array:
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
				this->wrapSSetting = gl_clamp_to_edge;
				break;
			}

			case 1:
			{
				this->wrapSSetting = gl_mirror_clamp_to_edge;
				break;
			}

			case 2:
			{
				this->wrapSSetting = gl_clamp_to_border;
				break;
			}

			case 3:
			{
				this->wrapSSetting = GL_REPEAT;
				break;
			}

			case 4:
			{
				this->wrapSSetting = gl_mirrored_repeat;
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
				this->wrapTSetting = gl_clamp_to_edge;
				break;
			}

			case 1:
			{
				this->wrapTSetting = gl_mirror_clamp_to_edge;
				break;
			}

			case 2:
			{
				this->wrapTSetting = gl_clamp_to_border;
				break;
			}

			case 3:
			{
				this->wrapTSetting = GL_REPEAT;
				break;
			}

			case 4:
			{
				this->wrapTSetting = gl_mirrored_repeat;
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
				this->wrapRSetting = gl_clamp_to_edge;
				break;
			}

			case 1:
			{
				this->wrapRSetting = gl_mirror_clamp_to_edge;
				break;
			}

			case 2:
			{
				this->wrapRSetting = gl_clamp_to_border;
				break;
			}

			case 3:
			{
				this->wrapRSetting = GL_REPEAT;
				break;
			}

			case 4:
			{
				this->wrapRSetting = gl_mirrored_repeat;
				break;
			}
		}

		BindTexture();
		glTexParameteri(target, gl_texture_wrap_r, this->wrapRSetting);
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
