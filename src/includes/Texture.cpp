#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

/*
 ***************************************************************
 * Constructors & Destructors
 *	- Default Constructor
 *		- Initializes everything to their default values
 *	- Construct Texture based on Image Source Given
 *
 *	- Destructor used to Free up Memory
 *
 ***************************************************************
 */

Texture::Texture() {
	width = height = channels = 0;
	textureID = 0;
}

Texture::Texture(const char* src) {
	// Setup STBI Settings
	stbi_set_flip_vertically_on_load(true);	// Flip Images Vertically

	// Load in the Image (RGBA Channels)
	unsigned char *data = stbi_load(src, &width, &height, &channels, 4);

	// Make sure data is Loaded Successfully
	if (data) {
		// Generate and Bind Texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);


		// Setup Texture Properties to Bound Texture
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		// Specify & Load Data of Image Texture
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,			// 8-bits (1Byte) per channel (0-255)
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data
		);

		// Generate a Mipmap for the Texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// ERROR: Texture not Loaded
	else {
		textureID = 0;
		std::cerr << "Texture: Image not Loaded in Properly!\n";
	}

	// Free up Image Data
	stbi_image_free(data);
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}


/*
 ***************************************************************
 * Texture Handling and Manipulation
 *	- Bind & Unbind Current Texture
 ***************************************************************
 */

void Texture::bind(unsigned int slot = 0) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}