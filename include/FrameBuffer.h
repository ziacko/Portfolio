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

		this->width = texDesc.width;
		this->height = texDesc.height;
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

		attachment_t(std::string uniformName = "defaultTexture", glm::vec2 resolution = glm::vec2(1280, 720),
			FBODescriptor FBODesc = FBODescriptor())
		{
			this->uniformName = uniformName;
			this->handle = 0;
			this->texType = texType;
			isResident = false;

			this->data = nullptr;			

			attachmentHandle = 0;
			this->FBODesc = FBODesc;
			this->FBODesc.width = resolution.x;
			this->FBODesc.height = resolution.y;
			this->texDesc = FBODesc;


			glCreateTextures(this->FBODesc.target, 1, &handle);
			glBindTexture(this->FBODesc.target, handle);
			switch (this->FBODesc.target)
			{
			case gl_texture_2d_multisample:
			{

				glTextureStorage2DMultisample(handle, this->FBODesc.sampleCount, this->FBODesc.internalFormat, this->FBODesc.width, this->FBODesc.height, true);
				break;
			}

			case gl_texture_2d:
			{
				//parse internal format as bits per pixel
				glTexImage2D(this->FBODesc.target, this->FBODesc.currentMipmapLevel, this->FBODesc.internalFormat, this->FBODesc.width, this->FBODesc.height, this->FBODesc.border, this->FBODesc.format, this->FBODesc.dataType, nullptr);
				break;
			}

			case gl_texture_2d_array:
			case gl_texture_3d:
			{
				glTexImage3D(this->FBODesc.target, this->FBODesc.currentMipmapLevel, this->FBODesc.internalFormat, this->FBODesc.width, this->FBODesc.height, this->FBODesc.depth, this->FBODesc.border, this->FBODesc.format, this->FBODesc.dataType, nullptr);
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
					glFramebufferTextureLayer(gl_framebuffer, gl_color_attachment0 + iter, handle, 0, iter);
				}
			}
			else
			{

				glFramebufferTexture(gl_framebuffer, attachmentFormat, handle, FBODesc.currentMipmapLevel);
			}
		}

		void Resize(glm::vec2 newSize, bool unbind = true)
		{
			FBODesc.width = newSize.x;
			FBODesc.height = newSize.y;

			switch (FBODesc.target)
			{
			case gl_texture_2d_multisample:
			{
				BindTexture();
				glDeleteTextures(1, &handle);
				glCreateTextures(FBODesc.target, 1, &handle);
				BindTexture();
				glTextureStorage2DMultisample(handle, this->FBODesc.sampleCount, this->FBODesc.internalFormat, this->FBODesc.width, this->FBODesc.height, true);
				UnbindTexture();
				break;
			}

			case gl_texture_2d:
			{
				BindTexture();
				glTexImage2D(FBODesc.target, FBODesc.currentMipmapLevel, FBODesc.internalFormat, FBODesc.width, FBODesc.height, FBODesc.border, FBODesc.format, FBODesc.dataType, nullptr);
				UnbindTexture();
				break;
			}

			case gl_texture_3d:
			case gl_texture_2d_array:
			{
				BindTexture();
				glTexImage3D(this->FBODesc.target, this->FBODesc.currentMipmapLevel, this->FBODesc.internalFormat, this->FBODesc.width, this->FBODesc.height, this->FBODesc.depth, this->FBODesc.border, this->FBODesc.format, this->FBODesc.dataType, nullptr);
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
				glTexParameteri(FBODesc.target, gl_depth_texture_mode, GL_DEPTH_COMPONENT);
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

	void Draw(unsigned int renderTextureHandle)
	{
		//if the current framebuffer is not this one then bind it
		glDrawBuffers(1, &attachments[renderTextureHandle]->FBODesc.attachmentFormat);
	}

	void DrawAll()
	{
		std::vector<GLenum> allImages;
		for (auto iter : attachments)
		{
			allImages.push_back(iter->FBODesc.format);
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

	void Resize(glm::vec2 newSize/*, bool unbind = true*/)
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