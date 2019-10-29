#pragma once
#include <GL/glew.h>
#include <iostream>


/*
 *********************
 * 2D Texture Class
 *********************
 */
class Texture {
  private:	// Texture Data
	  int width, height, channels;
	  GLuint textureID;

  public:
	  /* 
	   * Default Constructor
	   *	Initialize Texture data to their Default Values
	   */
	  Texture();

	  /*
	   * Loads Texture from Source Given
	   *	@param src - Location of Texture Image
	   */
	  Texture(const char*);

	  /*
	   * Frees up Used Memory
	   */
	  ~Texture();

	  /*
	   * Binds current Texture
	   *	@param slot - Texture Slot to Bind to (Default is 0)
	   */
	  void bind(unsigned int);

	  /* 
	   * Unbinds current Texture
	   */
	  void unbind();
};