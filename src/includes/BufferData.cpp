#include "BufferData.h"

/*
 ***************************************************************
 * Constructors & Destructors
 *	- Default Constructor
 *		- Initializes everything to their default values
 *	- Construct Data based on Given Index Values
 *
 *	- Method used to Free up Memory
 *
 ***************************************************************
 */
BufferData::BufferData() : VAO(0), verticiesBuffer(0), indiciesBuffer(0), texture(nullptr) {}

BufferData::BufferData(GLuint& _vertBuffer, GLuint& _indBuffer, GLuint& _vao)
    : VAO(_vao), verticiesBuffer(_vertBuffer), indiciesBuffer(_indBuffer), texture(nullptr) {}

void BufferData::freeBufferData(BufferData* buffer) {
    glDeleteVertexArrays(1, &buffer->VAO);
    glDeleteBuffers(1, &buffer->verticiesBuffer);
    glDeleteBuffers(1, &buffer->indiciesBuffer);
    delete buffer->texture;
}