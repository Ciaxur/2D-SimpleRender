#include "BufferData.h"

/*
 ***************************************************************
 * Constructors & Destructors
 *	- Default Constructor
 *		- Initializes everything to their default values
 *	- Construct Data based on Given Index Values
 *
 *	- Destructor used to Free up Memory
 *
 ***************************************************************
 */
BufferData::BufferData(): verticiesBuffer(0), indiciesBuffer(0), VAO(0), textureID(0) {}

BufferData::BufferData(GLuint& _vertBuffer, GLuint& _indBuffer, GLuint& _vao)
	: verticiesBuffer(_vertBuffer), indiciesBuffer(_indBuffer), VAO(_vao), textureID(0) {}

BufferData::~BufferData() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &verticiesBuffer);
	glDeleteBuffers(1, &indiciesBuffer);
}