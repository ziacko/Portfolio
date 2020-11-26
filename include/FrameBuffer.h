#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct FBODescriptor : public textureDescriptor
{
	enum class attachmentType_t
	{
		color,
		depth,
		stencil,
		depthAndStencil
	};

	FBODescriptor(GLuint sampleCount = 1, attachmentType_t attachmentType = attachmentType_t::color, textureDescriptor texDesc = textureDescriptor())
	{
		this->sampleCount = sampleCount;
		this->attachmentType = attachmentType;
		attachmentFormat = 0;
		attachmentHandle = 0;
		layers = 0;

		this->dimensions = texDesc.dimensions;
		this->channels = texDesc.channels;
		this->format = texDesc.format;
		this->bitsPerPixel = texDesc.bitsPerPixel;

		this->currentMipmapLevel = texDesc.currentMipmapLevel;
		this->mipmapLevels = texDesc.mipmapLevels;
		this->border = texDesc.border;
		this->xOffset = texDesc.xOffset;
		this->yOffset = texDesc.yOffset;

		this->internalFormat = texDesc.internalFormat;
		this->target = texDesc.target;
		this->dataType = texDesc.dataType;

		this->minFilterSetting = texDesc.minFilterSetting;
		this->magFilterSetting = texDesc.magFilterSetting;
		this->wrapSSetting = texDesc.wrapSSetting;
		this->wrapTSetting = texDesc.wrapTSetting;
		this->wrapRSetting = texDesc.wrapRSetting;
	}

	GLuint				layers;
	GLuint				sampleCount;
	GLenum				attachmentFormat;
	GLuint				attachmentHandle;
	attachmentType_t	attachmentType;
};

class frameBuffer
{
public:

	//have this inherit from texture later on!
	class attachment_t : public texture
	{
	public:

		attachment_t(std::string uniformName = "defaultTexture", FBODescriptor FBODesc = FBODescriptor())
		{
			this->uniformName = uniformName;
			this->handle = 0;
			this->texType = texType;
			isResident = false;

			this->data = nullptr;			

			attachmentHandle = 0;
			//need a better system to this redundant bullshit
			this->texDesc = (textureDescriptor)FBODesc;
			this->FBODesc = FBODesc;

			glCreateTextures(this->FBODesc.target, 1, &handle);
			glBindTexture(this->FBODesc.target, handle);
			switch (this->FBODesc.target)
			{
			case gl_texture_2d_multisample:
			{

				glTextureStorage2DMultisample(handle, this->FBODesc.sampleCount, this->FBODesc.internalFormat, this->FBODesc.dimensions.x, this->FBODesc.dimensions.y, true);
				break;
			}

			case gl_texture_2d:
			{
				//parse internal format as bits per pixel
				glTexImage2D(this->FBODesc.target, this->FBODesc.currentMipmapLevel, this->FBODesc.internalFormat, this->FBODesc.dimensions.x, this->FBODesc.dimensions.y, this->FBODesc.border, this->FBODesc.format, this->FBODesc.dataType, nullptr);
				break;
			}

			case gl_texture_2d_array:
			case gl_texture_3d:
			{
				glTexImage3D(this->FBODesc.target, this->FBODesc.currentMipmapLevel, this->FBODesc.internalFormat, this->FBODesc.dimensions.x, this->FBODesc.dimensions.y, this->FBODesc.dimensions.z, this->FBODesc.border, this->FBODesc.format, this->FBODesc.dataType, nullptr);
				break;
			}
			}

			glTexParameteri(this->FBODesc.target, GL_TEXTURE_MIN_FILTER, this->FBODesc.minFilterSetting);
			glTexParameteri(this->FBODesc.target, GL_TEXTURE_MAG_FILTER, this->FBODesc.magFilterSetting);
			glTexParameteri(this->FBODesc.target, GL_TEXTURE_WRAP_S, this->FBODesc.wrapSSetting);
			glTexParameteri(this->FBODesc.target, GL_TEXTURE_WRAP_T, this->FBODesc.wrapTSetting);
			if(this->FBODesc.mipmapLevels > 0)
			{
				glGenerateMipmap(this->FBODesc.target);
			}
			UnbindTexture();
		}

		void Initialize(GLenum attachmentFormat)
		{
			FBODesc.attachmentFormat = attachmentFormat;
			if (FBODesc.layers > 0)
			{
				for (size_t iter = 0; iter < FBODesc.layers; iter++)
				{
					glFramebufferTextureLayer(gl_framebuffer, gl_color_attachment0 + iter, handle, 0, (GLint)iter);
				}
			}
			else
			{

				glFramebufferTexture(gl_framebuffer, attachmentFormat, handle, FBODesc.currentMipmapLevel);
			}

			switch(attachmentFormat)
			{
			case gl_depth_attachment:
			{
				SetReadMode(FBODescriptor::attachmentType_t::depth);
				break;
			}
			
			case gl_stencil_attachment:
			{
				SetReadMode(FBODescriptor::attachmentType_t::stencil);
				break;
			}

			case gl_depth_stencil_attachment:
			{
				SetReadMode(FBODescriptor::attachmentType_t::stencil);
				break;
			}

			default:
			{
				break;
			}
			}
		}

		void Resize(glm::ivec3 newSize, bool unbind = true)
		{
			texDesc.dimensions = newSize;
			FBODesc.dimensions = newSize;

			switch (FBODesc.target)
			{
			case gl_texture_2d_multisample:
			{
				BindTexture();
				glDeleteTextures(1, &handle);
				glCreateTextures(FBODesc.target, 1, &handle);
				BindTexture();
				glTextureStorage2DMultisample(handle, this->FBODesc.sampleCount, this->FBODesc.internalFormat, this->FBODesc.dimensions.x, this->FBODesc.dimensions.y, true);
				UnbindTexture();
				break;
			}

			case gl_texture_2d:
			{
				BindTexture();
				glTexImage2D(FBODesc.target, FBODesc.currentMipmapLevel, FBODesc.internalFormat, FBODesc.dimensions.x, FBODesc.dimensions.y, FBODesc.border, FBODesc.format, FBODesc.dataType, nullptr);
				UnbindTexture();
				break;
			}

			case gl_texture_3d:
			case gl_texture_2d_array:
			{
				BindTexture();
				glTexImage3D(this->FBODesc.target, this->FBODesc.currentMipmapLevel, this->FBODesc.internalFormat, this->FBODesc.dimensions.x, this->FBODesc.dimensions.y, this->FBODesc.dimensions.z, this->FBODesc.border, this->FBODesc.format, this->FBODesc.dataType, nullptr);
				UnbindTexture();
				break;
			}
			}
		}

		void SetReadMode(FBODescriptor::attachmentType_t attachmentType)
		{
			BindTexture();
			FBODesc.attachmentType = attachmentType;

			switch (FBODesc.attachmentType)
			{
			case FBODescriptor::attachmentType_t::depth:
			{
				glTexParameteri(FBODesc.target, gl_depth_texture_mode, GL_LUMINANCE);
			}

			case FBODescriptor::attachmentType_t::stencil:
			{
				glTexParameteri(FBODesc.target, gl_depth_stencil_texture_mode, GL_STENCIL_INDEX);
			}

			case FBODescriptor::attachmentType_t::depthAndStencil:
			{
				glTexParameteri(FBODesc.target, gl_depth_stencil_texture_mode, GL_STENCIL_INDEX);
			}
			}
			
			UnbindTexture();
		}

		void Draw()
		{
			//if the current framebuffer is not this one then bind it
			switch (FBODesc.attachmentType)
			{
			case FBODescriptor::attachmentType_t::stencil:
			case FBODescriptor::attachmentType_t::depth:
			case FBODescriptor::attachmentType_t::depthAndStencil:
			{
				//if non-color, draw to GL_NONE
				GLenum attachment = GL_NONE;
				glDrawBuffers(1, &attachment);
				break;
			}

			default:
			{
				glDrawBuffers(1, &FBODesc.attachmentFormat);
				break;
			}
			}
		}

	public:
		GLuint			attachmentHandle;
		FBODescriptor	FBODesc;
	};

	frameBuffer()
	{
		bufferHandle = 0;	
	}

	void Initialize()
	{
		glGenFramebuffers(1, &bufferHandle);
		//glBindFramebuffer(gl_framebuffer, bufferHandle);
	}

	void Bind(GLenum target = gl_framebuffer)
	{
		glBindFramebuffer(target, bufferHandle);
	}

	static void Unbind(GLenum target = gl_framebuffer)
	{
		glBindFramebuffer(target, 0);
	}

	void DrawAll()
	{
		std::vector<GLenum> allImages;
		for (auto iter : attachments)
		{
			switch (iter->FBODesc.attachmentType)
			{
			case FBODescriptor::attachmentType_t::stencil:
			case FBODescriptor::attachmentType_t::depth:
			case FBODescriptor::attachmentType_t::depthAndStencil:
				{
					//if non-color, draw to GL_NONE
					allImages.push_back(GL_NONE);
					break;
				}

				default:
				{
					allImages.push_back(iter->FBODesc.attachmentFormat);
					break;
				}
			}
		}

		glDrawBuffers(allImages.size(), allImages.data());
	}

	void DrawDepth()
	{
		GLuint test = gl_depth_attachment;
		glDrawBuffers(1, &test);
	}

	void DrawMultiple(const char* name)
	{

	}

	void Resize(glm::ivec3 newSize/*, bool unbind = true*/)
	{
		//delete the framebuffer
		//glDeleteFramebuffers(1, &bufferHandle);

		//resize the buffers
		for (size_t iter = 0; iter < attachments.size(); iter++)
		{
			//if its the last one, unbind textures
			//if (iter == attachments.size() - 1)
			//{
				attachments[iter]->Resize(newSize);
			//}

			/*else if(unbind)
			{
				attachments[iter]->Resize(newSize, unbind);							   				 			  
			}*/
		}
		
		//recreate the framebuffer and re-attach the textures
		/*glGenFramebuffers(1, &bufferHandle);


		for (auto iter : attachments)
		{
			iter->Initialize(iter->attachmentFormat);
		}*/

		//unbind that framebuffers
		//Unbind();
	}

	void ClearTexture(attachment_t* attachment, float clearColor[4])
	{
		switch (attachment->FBODesc.attachmentType)
		{
		case FBODescriptor::attachmentType_t::color:
		{	
			glClearBufferfv(GL_COLOR, attachment->attachmentHandle, clearColor);
			break;
		}

		case FBODescriptor::attachmentType_t::depth:
		{
			glClearBufferfv(GL_DEPTH, 0, clearColor);
			break;
		}

		case FBODescriptor::attachmentType_t::stencil:
		{
			glClearBufferiv(GL_STENCIL, 0, (GLint*)&clearColor[0]);
			break;
		}

		case FBODescriptor::attachmentType_t::depthAndStencil:
		{
			//glClearBufferfv(GL_STENCIL, attachment->attachmentHandle, clearColor);
			glClearBufferfi(GL_DEPTH, attachment->attachmentHandle, clearColor[0], (GLint)clearColor[1]);
			break;
		}
		}
	}

	void AddAttachment(attachment_t* attachment)
	{
		//if the current framebuffer is not this one then bind it
		int currentBuffer = 0;
		glGetIntegerv(gl_framebuffer_binding, &currentBuffer);
		if (currentBuffer != bufferHandle)
		{
			Bind();
		}

		switch (attachment->FBODesc.attachmentType)
		{
		case FBODescriptor::attachmentType_t::color:
		{
			attachment->attachmentHandle = colorAttachmentNum;
			attachment->Initialize(gl_color_attachment0 + colorAttachmentNum);
			colorAttachmentNum++;
			break;
		}

		case FBODescriptor::attachmentType_t::depth:
		{
			attachment->attachmentHandle = gl_depth_attachment;
			attachment->Initialize(gl_depth_attachment);
			break;
		}

		case FBODescriptor::attachmentType_t::stencil:
		{
			attachment->attachmentHandle = gl_stencil_attachment;
			attachment->Initialize(gl_stencil_attachment);
			break;
		}

		case FBODescriptor::attachmentType_t::depthAndStencil:
		{
			attachment->attachmentHandle = gl_depth_stencil_attachment;
			attachment->Initialize(gl_depth_stencil_attachment);
			break;
		}
		}

		attachments.push_back(attachment);
		CheckStatus();
	}

	/*void AddDepth(glm::vec2 size)
	{
		glGenRenderbuffers(1, &depthHandle);
		glBindRenderbuffer(gl_renderbuffer, depthHandle);
		glRenderbufferStorage(gl_renderbuffer, GL_DEPTH_COMPONENT,size.x, size.y);
		glFramebufferRenderbuffer(gl_framebuffer, gl_depth_attachment, gl_renderbuffer, depthHandle);
		glBindRenderbuffer(gl_renderbuffer, 0);
	}*/

	bool CheckStatus()
	{
		//if the current framebuffer is not this one then bind it
		glFinish();
		int currentBuffer = 0;
		glGetIntegerv(gl_framebuffer_binding, &currentBuffer);
		if (currentBuffer != bufferHandle)
		{
			Bind();
		}
		GLenum err = glCheckFramebufferStatus(gl_draw_framebuffer);
		if (err != gl_framebuffer_complete)
		{
			switch (err)
			{
			case gl_framebuffer_undefined:
			{
				printf("framebuffer undefined \n");
				break;
			}

			case gl_framebuffer_incomplete_attachment:
			{
				printf("framebuffer incomplete attachment \n");
				break;
			}

			case gl_framebuffer_incomplete_missing_attachment:
			{
				printf("framebuffer missing attachment \n");
				break;
			}

			case gl_framebuffer_incomplete_draw_buffer:
			{
				printf("framebuffer incomplete draw buffer \n");
				break;
			}

			case gl_framebuffer_incomplete_read_buffer:
			{
				printf("framebuffer incomplete read buffer \n");
				break;
			}

			case gl_framebuffer_unsupported:
			{
				printf("framebuffer unsupported \n");
				break;
			}

			case gl_framebuffer_incomplete_multisample:
			{
				printf("framebuffer incomplete multisample \n");
				break;
			}

			case gl_framebuffer_incomplete_layer_targets:
			{
				printf("framebuffer incomplete layer targets \n");
				break;
			}
			}
			return false;
		}
		return true;
	}

	//ok we need a target, handle, etc.
	GLuint							bufferHandle;
	std::vector<attachment_t*>		attachments;
	GLuint							colorAttachmentNum = 0;
	GLuint							depthHandle = 0;
};
#endif