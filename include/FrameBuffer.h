#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
	
class frameBuffer
{
public:

	//have this inherit from texture later on!
	class attachment_t : public texture
	{
	public:
		enum class attachmentType_t
		{
			color,
			depth,
			stencil,
			depthAndStencil
		};

		attachment_t(attachmentType_t attachment = attachmentType_t::color,
			std::string uniformName = "defaultTexture", glm::vec2 resolution = glm::vec2(1280, 720),
			GLint format = GL_RGBA, GLenum target = GL_TEXTURE_2D,
			GLenum dataType = GL_FLOAT,	GLenum internalFormat = GL_RGBA16,
			GLuint samples = 0, GLint currentMipmapLevel = 0, GLint mipmapLevels = 1, 
			GLint border = 0, GLint xOffset = 0, GLint yOffset = 0,
			GLenum minFilterSetting = GL_LINEAR, GLenum magFilterSetting = GL_LINEAR,
			GLenum wrapSSetting = GL_REPEAT, GLenum wrapTSetting = GL_REPEAT, GLenum wrapRSetting = gl_clamp_to_edge)
		{
			this->uniformName = uniformName;

			this->attachmentType = attachment;

			this->uniformName = uniformName;
			this->width = resolution.x;
			this->height = resolution.y;
			this->channels = 0;
			this->format = format;
			this->target = target;
			this->sampleCount = samples;
			this->currentMipmapLevel = currentMipmapLevel;
			this->mipmapLevels = mipmapLevels;
			this->border = border;
			this->dataType = dataType;
			this->internalDataType = internalDataType;
			this->internalFormat = internalFormat;
			this->xOffset = xOffset;
			this->yOffset = yOffset;

			this->minFilterSetting = minFilterSetting;
			this->magFilterSetting = magFilterSetting;
			this->wrapSSetting = wrapSSetting;
			this->wrapTSetting = wrapTSetting;
			this->wrapRSetting = wrapRSetting;
			this->bitsPerPixel = 0;
			this->uniformHandle = 0;
			this->data = nullptr;

			this->handle = 0;

			this->texType = texType;

			attachmentHandle = 0;

			glGenTextures(1, &handle);
			glBindTexture(target, handle);

			//parse internal format as bits per pixel

			/*if (samples > 0 && target == gl_texture_2d_multisample)
			{
				glTexStorage2DMultisample(target, sampleCount, internalFormat, width, height, true);				
			}
			else
			{*/
				glTexImage2D(target, currentMipmapLevel, internalFormat, width, height, border, format, dataType, nullptr);
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilterSetting);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilterSetting);
				glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapSSetting);
				glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapTSetting);
			//}			

			UnbindTexture();
		}

		void Initialize(GLenum attachmentFormat)
		{
			this->attachmentFormat = attachmentFormat;
			glFramebufferTexture2D(gl_framebuffer, this->attachmentFormat, target, handle, currentMipmapLevel);
		}

		void Resize(glm::vec2 newSize, bool unbind = true)
		{
			width = newSize.x;
			height = newSize.y;
			BindTexture();

			glTexImage2D(target, currentMipmapLevel, internalFormat, width, height, border, format, dataType, nullptr);

			//if (unbind)
			//{
				UnbindTexture();
			//}
		}

	public:

		GLuint					sampleCount;
		GLenum					attachmentFormat;
		GLuint					attachmentHandle;
		attachmentType_t		attachmentType;
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

	void Bind()
	{
		glBindFramebuffer(gl_framebuffer, bufferHandle);
	}

	static void Unbind()
	{
		glBindFramebuffer(gl_framebuffer, 0);
	}

	void Draw(unsigned int renderTextureHandle)
	{
		//if the current framebuffer is not this one then bind it
		glDrawBuffers(1, &attachments[renderTextureHandle]->attachmentFormat);
	}

	void DrawAll()
	{
		std::vector<GLenum> allImages;
		for (auto iter : attachments)
		{
			allImages.push_back(iter->attachmentFormat);
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
		switch (attachment->attachmentType)
		{
		case attachment_t::attachmentType_t::color:
		{	
			glClearBufferfv(GL_COLOR, attachment->attachmentHandle, clearColor);
			break;
		}

		case attachment_t::attachmentType_t::depth:
		{
			glClearBufferfv(GL_DEPTH, 0, clearColor);
			break;
		}

		case attachment_t::attachmentType_t::stencil:
		{
			glClearBufferfv(GL_STENCIL, 0, clearColor);
			break;
		}

		case attachment_t::attachmentType_t::depthAndStencil:
		{
			glClearBufferfv(GL_STENCIL, attachment->attachmentHandle, clearColor);
			glClearBufferfv(GL_DEPTH, attachment->attachmentHandle, clearColor);
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

		switch (attachment->attachmentType)
		{
		case attachment_t::attachmentType_t::color:
		{
			attachment->attachmentHandle = colorAttachmentNum;
			attachment->Initialize(gl_color_attachment0 + colorAttachmentNum);
			colorAttachmentNum++;
			break;
		}

		case attachment_t::attachmentType_t::depth:
		{
			attachment->attachmentHandle = gl_depth_attachment;
			attachment->Initialize(gl_depth_attachment);
			break;
		}

		case attachment_t::attachmentType_t::stencil:
		{
			attachment->attachmentHandle = gl_stencil_attachment;
			attachment->Initialize(gl_stencil_attachment);
			break;
		}

		case attachment_t::attachmentType_t::depthAndStencil:
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
			printf("framebuffer creation failed \n");
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