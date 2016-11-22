#include <gli/gli.hpp>

template <typename texture, typename genType>
int test_texture
(
	texture const & Texture,
	genType const & ClearColor,
	genType const & FirstColor
)
{
	int Error(0);

	texture TextureA(gli::duplicate(Texture));
	TextureA.template clear<genType>(ClearColor);
	*TextureA.template data<genType>() = FirstColor;

	texture TextureB = gli::flip(TextureA);
	Error += TextureA != TextureB ? 0 : 1;

	texture TextureC = gli::flip(TextureB);
	Error += TextureC == TextureA ? 0 : 1;

	return Error;
}

int main()
{
	int Error(0);

	gli::texture2d::extent_type const TextureSize(32);
	gli::size_t const Levels = gli::levels(TextureSize);

	Error += test_texture(
		gli::texture2d(gli::FORMAT_R8_UNORM_PACK8, TextureSize, Levels),
		glm::uint8(255), glm::uint8(0));

	Error += test_texture(
		gli::texture2d(gli::FORMAT_RGB8_UNORM_PACK8, TextureSize, Levels),
		glm::u8vec3(255, 128, 0), glm::u8vec3(0, 128, 255));

	Error += test_texture(
		gli::texture2d(gli::FORMAT_RGBA8_UNORM_PACK8, TextureSize, Levels),
		glm::u8vec4(255, 128, 0, 255), glm::u8vec4(0, 128, 255, 255));

	Error += test_texture(
		gli::texture2d(gli::FORMAT_RGBA32_SFLOAT_PACK32, TextureSize, Levels),
		glm::f32vec4(1.0, 0.5, 0.0, 1.0), glm::f32vec4(0.0, 0.5, 1.0, 1.0));

	Error += test_texture(
		gli::texture2d_array(gli::FORMAT_RGBA8_UNORM_PACK8, TextureSize, 4, Levels),
		glm::u8vec4(255, 128, 0, 255), glm::u8vec4(0, 128, 255, 255));

	Error += test_texture(
		gli::texture2d_array(gli::FORMAT_RGBA32_SFLOAT_PACK32, TextureSize, 4, Levels),
		glm::f32vec4(1.0, 0.5, 0.0, 1.0), glm::f32vec4(0.0, 0.5, 1.0, 1.0));

	return Error;
}
