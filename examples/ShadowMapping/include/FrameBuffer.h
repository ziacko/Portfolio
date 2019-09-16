#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
	
class frameBuffer
{
public:

	struct attachment_t
	{
		enum class attachmentType_t
		{
			color,
			depth,
			stencil
		};

		attachment_t(attachmentType_t attachment = attachmentType_t::color, GLenum target = GL_TEXTURE_2D,
			glm::ivec2 resolution = glm::ivec2(1280, 720), GLint internalFormat = GL_RGBA,
			GLuint level = 1, 
			GLenum minfilterSetting = GL_LINEAR, GLenum magfilterSetting = GL_LINEAR,
			GLenum wrapSSetting = GL_CLAMP_TO_EDGE, GLenum wrapTSetting = GL_CLAMP_TO_EDGE, GLenum wrapRSetting = GL_CLAMP_TO_EDGE)
		{
			this->attachmentType = attachment;

			this->target = target;
			this->resolution = resolution;
			this->internalFormat = internalFormat;
			this->level = level;

			this->minFilterSetting = minfilterSetting;
			this->magFilterSetting = magfilterSetting;
			this->wrapSSetting = wrapSSetting;
			this->wrapTSetting = wrapTSetting;
			this->wrapRSetting = wrapRSetting;

			glGenTextures(1, &handle);
			glBindTexture(target, handle);
			glTexStorage2D(target, level, internalFormat, resolution.x, resolution.y);
		}

		void Initialize(GLenum bufferTarget, GLenum attachment)
		{
			this->attachment = attachment;
			glFramebufferTexture(bufferTarget, attachment, handle, level);
		}

		GLenum target;
		glm::ivec2 resolution;
		GLint internalFormat;
		GLuint level;
		GLenum attachment;

		GLenum			minFilterSetting;
		GLenum			magFilterSetting;
		GLenum			wrapSSetting;
		GLenum			wrapTSetting;
		GLenum			wrapRSetting;

		GLuint handle;
		attachmentType_t attachmentType;
	};

	frameBuffer(GLenum target = GL_FRAMEBUFFER)
	{
		this->target = target;
		glGenFramebuffers(1, &bufferHandle);
	}

	void Bind()
	{
		glBindFramebuffer(target, bufferHandle);
	}

	void AddAttachment(attachment_t* attachment)
	{
		switch (attachment->attachmentType)
		{
		case attachment_t::attachmentType_t::color:
		{
			attachment->Initialize(target, GL_COLOR_ATTACHMENT0 + numColorAttachments);
			numColorAttachments++;
			break;
		}
		case attachment_t::attachmentType_t::depth:
		{
			attachment->Initialize(target, GL_DEPTH_ATTACHMENT);
			break;
		}

		case attachment_t::attachmentType_t::stencil:
		{
			attachment->Initialize(target, GL_STENCIL_ATTACHMENT);
			break;
		}
		}

		attachments.push_back(attachment);
	}

	//ok we need a target, handle, etc.
	GLuint bufferHandle;
	GLenum target;

	GLuint numColorAttachments;
	std::vector<attachment_t*> attachments;
};



#endif