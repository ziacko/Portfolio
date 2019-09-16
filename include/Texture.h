#ifndef TEXTURE_H
#define TEXTURE_H
#include <stb_image.h>
#include <stb_image_write.h>
#include <gli/gli.hpp>

class texture
{
public:

	enum class textureType_t
	{
		image,
		diffuse,
		normal,
		specular,
		height,
		roughness,
		metallic,
		ambientOcclusion,
		albedo
	};

	texture(std::string path = "../../resources/textures/earth_diffuse.tga", std::string uniformName = "defaultTexture",
		textureType_t texType = textureType_t::image, GLint internalFormat = GL_RGBA, GLenum target = GL_TEXTURE_2D,
		GLint currentMipmapLevel = 0, GLint mipmapLevels = 0, GLint border = 0, GLenum dataType = GL_UNSIGNED_BYTE,
		GLenum internalDataType = GL_RGBA8, GLint xOffset = 0, GLint yOffset = 0,
		GLenum minFilterSetting = GL_LINEAR, GLenum magFilterSetting = GL_LINEAR,
		GLenum wrapSSetting = GL_REPEAT, GLenum wrapTSetting = GL_REPEAT, GLenum wrapRSetting = gl_clamp_to_edge)
	{
		this->path = path;
		this->uniformName = uniformName;
		this->width = 0;
		this->height = 0;
		this->channels = 4;
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

		this->texType = texType;
		isResident = false;
		//LoadTexture();
	}

	virtual void BindTexture()
	{
		glBindTexture(target, handle);
	}

	virtual void GetUniformLocation(GLuint programHandle)
	{
		uniformHandle = glGetUniformLocation(programHandle, uniformName.c_str());
		glUniform1i(uniformHandle, handle);

		SetActive();
	}

	virtual void SetActive()
	{
		glActiveTexture(gl_texture0 + handle);
		glBindTexture(target, handle);
	}
	
	virtual void SetActive(GLuint texUnit)
	{
		glActiveTexture(gl_texture0 + texUnit);
		glBindTexture(target, handle);
	}

	void UnbindTexture()
	{
		glActiveTexture(gl_texture0 + handle);
		glBindTexture(target, NULL);
	}

	static void UnbindTexture(GLenum target, unsigned int texUnit)
	{
		glActiveTexture(gl_texture0 + texUnit);
		glBindTexture(target, NULL);
	}

	virtual void OverloadTextureUnit(GLuint texUnit)
	{
		glActiveTexture(gl_texture0 + texUnit);
		glBindTexture(target, handle);
	}

	void LoadTexture()
	{
		stbi_set_flip_vertically_on_load(true);
		char* data = (char*)stbi_load(path.c_str(), &this->width, &this->height, &channels, 0);

		//if stbi fails then use gli instead. if that fails give up
		if (data == nullptr)
		{
			gli::texture tex = gli::load(path);
			if (!tex.empty())
			{
				gliLoad(tex);
			}
			else
			{
				printf("couldn't load texture: %s \n", path.c_str());
				return;
			}
		}

		else
		{
			stbLoad(data);
		}

		residentHandle = glGetTextureHandleARB(handle);
	}

	virtual void ReloadTexture(const char* path)
	{
		stbi_set_flip_vertically_on_load(true);
		char* data = (char*)stbi_load(path, &this->width, &this->height, &channels, 0);

		if (data != nullptr)
		{
			stbLoad(data);
		}

		else
		{

			gli::texture tex = gli::load(path);

			if (tex.empty())
			{
				return;
			}
			gliLoad(tex);
		}
		SetPath(path);
	}

	virtual void SetMinFilter(GLenum minFilterSetting)
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

		case 2:
		{
			this->minFilterSetting = GL_NEAREST_MIPMAP_NEAREST;
			break;
		}

		case 3:
		{
			this->minFilterSetting = GL_NEAREST_MIPMAP_LINEAR;
			break;
		}

		case 4:
		{
			this->minFilterSetting = GL_LINEAR_MIPMAP_NEAREST;
			break;
		}

		case 5:
		{
			this->minFilterSetting = GL_LINEAR_MIPMAP_LINEAR;
			break;
		}
		}
		BindTexture();
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, this->minFilterSetting);
		UnbindTexture();
	}

	virtual void SetMagFilter(GLenum magFilterSetting)
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

		case 2:
		{
			this->magFilterSetting = GL_NEAREST_MIPMAP_NEAREST;
			break;
		}

		case 3:
		{
			this->magFilterSetting = GL_NEAREST_MIPMAP_LINEAR;
			break;
		}

		case 4:
		{
			this->magFilterSetting = GL_LINEAR_MIPMAP_NEAREST;
			break;
		}

		case 5:
		{
			this->magFilterSetting = GL_LINEAR_MIPMAP_LINEAR;
			break;
		}
		}

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, this->magFilterSetting);
		UnbindTexture();
	}

	virtual void SetWrapS(GLenum wrapSetting)
	{
		this->wrapSSetting = wrapSetting;
		/*switch (wrapSetting)
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
		}*/

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_WRAP_S, this->wrapSSetting);
		UnbindTexture();
	}

	virtual void SetWrapT(GLenum wrapSetting)
	{
		this->wrapTSetting = wrapSetting;
		/*switch (wrapSetting)
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
		}*/

		BindTexture();
		glTexParameteri(target, GL_TEXTURE_WRAP_T, this->wrapTSetting);
		UnbindTexture();
	}

	virtual void SetWrapR(GLenum wrapSetting)
	{
		wrapRSetting = wrapSetting;
		/*switch (wrapSetting)
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
		}*/

		BindTexture();
		glTexParameteri(target, gl_texture_wrap_r, this->wrapRSetting);
		UnbindTexture();
	}

	virtual void SetPath(const char* newPath)
	{
		path = newPath;
	}

	virtual void SetTextureType(textureType_t newType)
	{
		this->texType = newType;
	}

	std::string GetFilePath()
	{
		return path;
	}

	std::string GetUniformName()
	{
		return uniformName;
	}

	unsigned int GetHandle()
	{
		return handle;
	}

	unsigned int GetResidentHandle()
	{
		return residentHandle;
	}

	glm::vec2 GetSize()
	{
		return glm::vec2(width, height);
	}

	std::vector<float> GetPixels()
	{
		int bytes = width * height * 2;

		GLfloat* pixels = new GLfloat[bytes];

		glGetTexImage(target, currentMipmapLevel, internalFormat, dataType, pixels);

		std::vector<float> result;
		result.assign(pixels, pixels + bytes);
		return result;
	}

	//copy another texture into itself. just 2D textures right now
	void Copy(texture* otherTexture)
	{
		glCopyImageSubData(otherTexture->handle, otherTexture->target, otherTexture->currentMipmapLevel, 0, 0, 0,
			handle, target, currentMipmapLevel, 0, 0, 0,
			width, height, 1);
	}

	void ToggleResident()
	{
		isResident = !isResident;

		if (isResident)
		{
			glMakeTextureHandleResidentARB(residentHandle);
		}

		else
		{
			glMakeTextureHandleNonResidentARB(residentHandle);
		}

	}

	void Initialize()
	{
		
		
	}

protected:
	GLuint			handle;
	std::string		path;
	GLuint			uniformHandle;
	std::string		uniformName;

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

	textureType_t	texType;

	GLuint64		residentHandle;
	bool			isResident;

private:

	void stbLoad(char* data)
	{
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
			glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapSSetting);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapTSetting);

			//glTexStorage2D(target, mipmapLevels, internalDataType, width, height);
			//glTexSubImage2D(target, currentMipmapLevel, xOffset, yOffset, width, height, internalFormat, dataType, data);

			glTexImage2D(target, currentMipmapLevel, internalFormat, width, height, border, format, dataType, data);
			
			if (mipmapLevels > 0)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			
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

		if (mipmapLevels > 0)
		{
			glTexParameteri(GL_TEXTURE_2D, gl_texture_max_level, mipmapLevels);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}

		else
		{
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilterSetting);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilterSetting);
		}

		float aniso = 0.0f;
		glGetFloatv(gl_max_texture_max_anisotropy, &aniso);
		glTexParameterf(GL_TEXTURE_2D, gl_texture_max_anisotropy, aniso);

		UnbindTexture();
	}

	void gliLoad(gli::texture tex)
	{
		tex = gli::flip(tex);
		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const gliFormat = GL.translate(tex.format(), tex.swizzles());
		target = GL.translate(tex.target());
		mipmapLevels = tex.levels();
		currentMipmapLevel = tex.levels();
		internalDataType = gliFormat.Internal;
		format = gliFormat.External;
		dataType = gliFormat.Type;

		glm::vec3 res = tex.extent();
		width = res.x;
		height = res.y;

		assert(gli::is_compressed(tex.format()) && tex.target() == gli::TARGET_2D);

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
			glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapSSetting);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapTSetting);
			glTexParameteri(target, gl_texture_base_level, 0);
			glTexParameteri(target, gl_texture_max_level, static_cast<GLint>(tex.levels() - 1));
			glTexParameteriv(target, gl_texture_swizzle_rgba, &gliFormat.Swizzles[0]);
			glTexImage2D(target, currentMipmapLevel, format, width, height, border, format, dataType, data);
			for (std::size_t level = 0; level < tex.levels(); level++)
			{
				glm::tvec3<GLsizei> extents(tex.extent(level));
				glCompressedTexSubImage2D(
					target, static_cast<GLint>(level), 0, 0, extents.x, extents.y,
					gliFormat.Internal, static_cast<GLsizei>(tex.size(level)), tex.data(0, 0, level));
			}
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
};
#endif
