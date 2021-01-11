#pragma once

class FFTransform_t
{
	glm::ivec2 workGroups;
	glm::ivec2 size;
	unsigned int fftProgram;
	attachment_t preCompData;

	FFTransform_t()
	{
		FBODescriptor desc;
		desc.dimensions = glm::ivec3(size.x, size.y, 1);
		desc.format = GL_RG;
		desc.internalFormat = gl_rg16f;

		preCompData = attachment_t("preCompData", desc);



	}
};

