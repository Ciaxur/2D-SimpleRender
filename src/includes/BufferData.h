#pragma once

// Library
#include "Texture.h"
#include "Shader.h"

// Core libraries
#include <GL/glew.h>
#include <memory>

/**
 * Stores Data Objects of the There
 *  linked buffers.
 *    - Vertex Array Object     (VAO)
 *      - Binds the Vertex Attributes and Vertex Buffer
 *    - Vertex Buffer
 *    - Index Buffer
 *    - Number of Elements Indicies
 *		- Texture Object
 *
 */
class BufferData {
  public:                     // Public Variables
    GLuint VAO;               // Vertex Array Object
    GLuint verticiesBuffer;   // Vertex Buffer
    GLuint indiciesBuffer;    // Index Buffer
    Texture *texture;         // Texture Object
    size_t indiciesElts = 0;  // Number of Indicies

    // Shared pointer to a shader since there could be multiple references.
    // Bound shader program on this buffer.
    std::shared_ptr<Shader> shader;


    /* Default Constructor: Initialize everything to 0 */
    BufferData();

    /*
    * Construct Data based on Given Index Values
    *	@param _vertBuffer - Vertex Buffer that holds all Verticies
    *	@param _indBuffer - Index Buffer for the Verticies
    *	@param _vao - Vertex Array Object that is bound to the Attributes
    *		as well as the Vertex Buffer
    */
    BufferData(GLuint &, GLuint &, GLuint &);

    /* Method that frees up used Memory */
    static void freeBufferData(BufferData *);
};

namespace CreateBuffer {
  /* Creates a StaticDraw Float Buffer */
	BufferData static_float(GLfloat *dataPack, size_t vSize, GLuint *indicies, size_t iSize, std::shared_ptr<Shader> shader);
};