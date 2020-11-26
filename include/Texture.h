#ifndef TEXTURE_H
#define TEXTURE_H
#include <stb_image.h>
#include <stb_image_write.h>
#include <gli/gli.hpp>

struct textureDescriptor
{
	textureDescriptor(GLenum target = GL_TEXTURE_2D, GLenum dataType = GL_UNSIGNED_BYTE,
		GLenum format = GL_RGBA, GLint internalFormat = GL_RGBA8,		
		GLenum minFilterSetting = GL_LINEAR, GLenum magFilterSetting = GL_LINEAR,
		GLenum wrapSSetting = GL_REPEAT, GLenum wrapTSetting = GL_REPEAT, GLenum wrapRSetting = gl_clamp_to_edge,
		GLenum access = gl_read_write)
	{
		dimensions = glm::vec3(1280, 720, 1);
		this->channels = 4;
		this->format = format;
		this->bitsPerPixel = 8;

		this->currentMipmapLevel = 0;
		this->mipmapLevels = 0;
		this->border = 0;
		this->xOffset = 0;
		this->yOffset = 0;

		this->internalFormat = internalFormat;
		this->target = target;
		this->dataType = dataType;

		this->minFilterSetting = minFilterSetting;
		this->magFilterSetting = magFilterSetting;
		this->wrapSSetting = wrapSSetting;
		this->wrapTSetting = wrapTSetting;
		this->wrapRSetting = wrapRSetting;

		this->isImmutable = false;
		this->access = access;
	}

	//size and pixel depth density settings
	glm::ivec3		dimensions;
	GLint			channels;
	GLenum			format;

	//texture formats and types
	GLint			internalFormat;
	GLenum			target;
	GLint			currentMipmapLevel;
	GLint			mipmapLevels;
	GLint			border;
	GLenum			dataType;
	GLint			xOffset;
	GLint			yOffset;

	//filtering settings
	GLenum			minFilterSetting;
	GLenum			magFilterSetting;
	GLenum			wrapSSetting;
	GLenum			wrapTSetting;
	GLenum			wrapRSetting;

	GLuint			bitsPerPixel;
	GLenum			access;

	bool			isImmutable;
};

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

	texture(std::string path = "../../resources/textures/earth_diffuse.tga", textureType_t texType = textureType_t::image,
		std::string uniformName = "defaultTexture", textureDescriptor texDescriptor = textureDescriptor())
	{
		this->path = path;
		this->uniformName = uniformName;
		this->handle = 0;
		this->texType = texType;

		this->texDesc = texDescriptor;

		isResident = false;
		residentHandle = 0;
		this->data = nullptr;
		uniformHandle = 0;
	}
	
	virtual ~texture(){};
	
	virtual void GetUniformLocation(GLuint programHandle)
	{
		uniformHandle = glGetUniformLocation(programHandle, uniformName.c_str());
		glUniform1i(uniformHandle, handle);

		SetActive();
	}

	virtual void BindTexture()
	{
		glBindTexture(texDesc.target, handle);
	}

	virtual void SetActive()
	{
		glBindTextureUnit(handle, handle);
	}
	
	virtual void SetActive(GLuint texUnit)
	{
		glBindTextureUnit(texUnit, handle);
	}

	void UnbindTexture() const
	{
		glBindTexture(texDesc.target, NULL);
	}

	static void UnbindTexture(GLenum target)
	{
		glBindTexture(target, NULL);
	}

	void BindAsImage(GLuint texUnit)
	{
		glBindImageTexture(texUnit, handle, texDesc.currentMipmapLevel, 0, 0, gl_write_only, texDesc.internalFormat);
	}

	virtual void OverloadTextureUnit(GLuint texUnit)
	{
		glActiveTexture(gl_texture0 + texUnit);
		glBindTexture(texDesc.target, handle);
	}

	void LoadTexture()
	{
		stbi_set_flip_vertically_on_load(true);
		char* data = (char*)stbi_load(path.c_str(), &texDesc.dimensions.x, &texDesc.dimensions.y, &texDesc.channels, 0);

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
		char* data = (char*)stbi_load(path, &texDesc.dimensions.x, &texDesc.dimensions.y, &texDesc.channels, 0);

		if (data != nullptr)
		{
			stbLoad(data, true);
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
				texDesc.minFilterSetting = GL_LINEAR;
				break;
			}

			case 1:
			{
				texDesc.minFilterSetting = GL_NEAREST;
				break;
			}

			case 2:
			{
				texDesc.minFilterSetting = GL_NEAREST_MIPMAP_NEAREST;
				break;
			}

			case 3:
			{
				texDesc.minFilterSetting = GL_NEAREST_MIPMAP_LINEAR;
				break;
			}

			case 4:
			{
				texDesc.minFilterSetting = GL_LINEAR_MIPMAP_NEAREST;
				break;
			}

			case 5:
			{
				texDesc.minFilterSetting = GL_LINEAR_MIPMAP_LINEAR;
				break;
			}

			default:
			{
				break;
			}
		}
		BindTexture();
		glTexParameteri(texDesc.target, GL_TEXTURE_MIN_FILTER, texDesc.minFilterSetting);
		//UnbindTexture();
	}

	virtual void SetMagFilter(GLenum magFilterSetting)
	{
		switch (magFilterSetting)
		{
			case 0:
			{
				texDesc.magFilterSetting = GL_LINEAR;
				break;
			}

			case 1:
			{
				texDesc.magFilterSetting = GL_NEAREST;
				break;
			}

			case 2:
			{
				texDesc.magFilterSetting = GL_NEAREST_MIPMAP_NEAREST;
				break;
			}

			case 3:
			{
				texDesc.magFilterSetting = GL_NEAREST_MIPMAP_LINEAR;
				break;
			}

			case 4:
			{
				texDesc.magFilterSetting = GL_LINEAR_MIPMAP_NEAREST;
				break;
			}

			case 5:
			{
				texDesc.magFilterSetting = GL_LINEAR_MIPMAP_LINEAR;
				break;
			}

			default:
			{
				break;
			}
		}

		BindTexture();
		glTexParameteri(texDesc.target, GL_TEXTURE_MAG_FILTER, texDesc.magFilterSetting);
		//UnbindTexture();
	}

	virtual void SetWrapS(GLenum wrapSetting)
	{
		texDesc.wrapSSetting = wrapSetting;
		
		BindTexture();
		glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_S, texDesc.wrapSSetting);
		glFinish();
		//UnbindTexture();
	}

	virtual void SetWrapT(GLenum wrapSetting)
	{
		texDesc.wrapTSetting = wrapSetting;
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
		glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_T, texDesc.wrapTSetting);
		//UnbindTexture();
	}

	virtual void SetWrapR(GLenum wrapSetting)
	{
		texDesc.wrapRSetting = wrapSetting;
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
		glTexParameteri(texDesc.target, gl_texture_wrap_r, texDesc.wrapRSetting);
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

	GLuint64 GetResidentHandle()
	{
		return residentHandle;
	}

	std::vector<float> GetPixels()
	{
		int bytes = texDesc.dimensions.x * texDesc.dimensions.y * 2;

		GLfloat* pixels = new GLfloat[bytes];

		glGetTexImage(texDesc.target, texDesc.currentMipmapLevel, texDesc.internalFormat, texDesc.dataType, pixels);

		std::vector<float> result;
		result.assign(pixels, pixels + bytes);
		return result;
	}

	//copy another texture into itself. just 2D textures right now
	void Copy(texture* otherTexture)
	{
		glCopyImageSubData(otherTexture->handle, otherTexture->texDesc.target, otherTexture->texDesc.currentMipmapLevel, 0, 0, 0,
			handle, texDesc.target, texDesc.currentMipmapLevel, 0, 0, 0,
			texDesc.dimensions.x, texDesc.dimensions.y, texDesc.dimensions.z);
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

//protected:
	GLuint				handle;
	std::string			path;
	GLuint				uniformHandle;
	std::string			uniformName;

	textureDescriptor	texDesc;
	char*				data;

	textureType_t		texType;

	GLuint64			residentHandle;
	bool				isResident;

private:

	void stbLoad(char* data, bool reload = false)
	{
		switch (texDesc.channels)
		{
		case 1:
		{
			texDesc.format = GL_R;
			break;
		}

		case 2:
		{
			texDesc.format = gl_rg;
			break;
		}

		case 3:
		{
			texDesc.format = GL_RGB;
			break;
		}

		case 4:
		{
			texDesc.format = GL_RGBA;
			break;
		}

		default:
		{
			break;
		}
		}
		if(!reload)
		{
			glGenTextures(1, &handle);
		}
		
		glBindTexture(texDesc.target, handle);

		switch (texDesc.target)
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
			glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_S, texDesc.wrapSSetting);
			glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_T, texDesc.wrapTSetting);

			if(texDesc.isImmutable)
			{				
				glTexStorage2D(texDesc.target, texDesc.mipmapLevels, texDesc.internalFormat, texDesc.dimensions.x, texDesc.dimensions.y);
				glTextureSubImage2D(handle, texDesc.currentMipmapLevel, texDesc.xOffset, texDesc.yOffset, texDesc.dimensions.x, texDesc.dimensions.y, texDesc.format, texDesc.dataType, data);
			}

			else
			{
				glTexImage2D(texDesc.target, texDesc.currentMipmapLevel, texDesc.internalFormat, texDesc.dimensions.x, texDesc.dimensions.y, texDesc.border, texDesc.format, texDesc.dataType, data);
			}
			
			if (texDesc.mipmapLevels > 0)
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
			glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_S, texDesc.wrapSSetting);
			glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_T, texDesc.wrapTSetting);
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

		

		if (texDesc.mipmapLevels > 0)
		{
			glTexParameteri(GL_TEXTURE_2D, gl_texture_max_level, texDesc.mipmapLevels);
			glTexParameteri(texDesc.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(texDesc.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}

		else
		{			
			glTexParameteri(texDesc.target, GL_TEXTURE_MIN_FILTER, texDesc.minFilterSetting);
			glTexParameteri(texDesc.target, GL_TEXTURE_MAG_FILTER, texDesc.magFilterSetting);
		}

		float aniso = 0.0f;
		glGetFloatv(gl_max_texture_max_anisotropy, &aniso); //throws out an openGL error but works anyway. not sure how to fix
		glTexParameterf(GL_TEXTURE_2D, gl_texture_max_anisotropy, aniso);
	}

	void gliLoad(gli::texture tex)
	{
		tex = gli::flip(tex);
		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const gliFormat = GL.translate(tex.format(), tex.swizzles());
		texDesc.target = GL.translate(tex.target());
		texDesc.mipmapLevels = (GLint)tex.levels();
		texDesc.currentMipmapLevel = 0;
		texDesc.internalFormat = gliFormat.Internal;
		texDesc.format = gliFormat.External;
		texDesc.dataType = gliFormat.Type;

		glm::vec3 res = tex.extent();
		texDesc.dimensions = res;

		//data = tex.data()
		
		bool compressed = gli::is_compressed(tex.format());

		//assert(gli::is_compressed(tex.format()) && tex.target() == gli::TARGET_2D);

		glGenTextures(1, &handle);
		glBindTexture(texDesc.target, handle);

		switch (texDesc.target)
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
			glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_S, texDesc.wrapSSetting);
			glTexParameteri(texDesc.target, GL_TEXTURE_WRAP_T, texDesc.wrapTSetting);
			glTexParameteri(texDesc.target, gl_texture_base_level, 0);
			glTexParameteri(texDesc.target, gl_texture_max_level, static_cast<GLint>(tex.levels() - 1));
			glTexParameteriv(texDesc.target, gl_texture_swizzle_rgba, &gliFormat.Swizzles[0]);
			
			for (unsigned int level = 0; level < tex.levels(); level++)
			{
				glm::tvec3<GLsizei> extents(tex.extent(level));
				if(compressed)
				{
					glTexStorage2D(texDesc.target, texDesc.currentMipmapLevel, texDesc.internalFormat, texDesc.dimensions.x, texDesc.dimensions.y);
					glCompressedTexSubImage2D(
						texDesc.target, static_cast<GLint>(level), 0, 0, extents.x, extents.y,
						texDesc.internalFormat, static_cast<GLsizei>(tex.size(level)), tex.data(0, 0, level));
				}

				else
				{
					texDesc.currentMipmapLevel = 0;
					glTexImage2D(texDesc.target, texDesc.currentMipmapLevel, texDesc.internalFormat, texDesc.dimensions.x, texDesc.dimensions.y, texDesc.border, texDesc.format, texDesc.dataType, tex.data(0, 0, level));
				}
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
		glTexParameteri(texDesc.target, GL_TEXTURE_MIN_FILTER, texDesc.minFilterSetting);
		glTexParameteri(texDesc.target, GL_TEXTURE_MAG_FILTER, texDesc.magFilterSetting);

		UnbindTexture();
	}
};
#endif
