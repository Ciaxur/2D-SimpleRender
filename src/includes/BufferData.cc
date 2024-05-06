#include "BufferData.h"
#include <spdlog/spdlog.h>

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
BufferData::BufferData() {
  this->VAO = 0;
  this->verticiesBuffer = 0;
  this->indiciesBuffer = 0;
  this->texture = nullptr;
  this->shader = nullptr;
  this->data_ptr = nullptr;
}

BufferData::BufferData(GLuint& _vertBuffer, GLuint& _indBuffer, GLuint& _vao) {
  this->VAO = _vao;
  this->verticiesBuffer = _vertBuffer;
  this->indiciesBuffer = _indBuffer;
  this->texture = nullptr;
  this->shader = nullptr;
  this->data_ptr = nullptr;
}

void BufferData::freeBufferData(BufferData* buffer) {
  glDeleteVertexArrays(1, &buffer->VAO);
  glDeleteBuffers(1, &buffer->verticiesBuffer);
  glDeleteBuffers(1, &buffer->indiciesBuffer);

  if (buffer->texture)
    delete buffer->texture;

  if (buffer->data_ptr)
    delete[] buffer->data_ptr;
}

void BufferData::update() {
  glNamedBufferSubData(
    this->verticiesBuffer,
    0,
    this->data_size_bytes,
    this->data_ptr
  );
}


/**
 * CreateBuffer NAMESPACE
 *
 * Creates Buffer data for Verticies & Indicies provided
 *  by creating a VAO linked to a VBO and EBO.
 * Data is configured and packaged in an Object with the
 *  reference IDs given by OpenGL and returned.
 *
 * Data is packed in an array of:
 *   [ VERTEX<vec3>   RGBA<vec4>    Texture Coordinates<vec2> ]
 *
 * @param dataPack - Data Pack for Buffer
 * @param vSize   - Size of the array in Bytes (sizeof(verticies))
 * @param indicies - The Indicies Array, specifying the order of Vertex to be drawn
 * @param iSize   - Size of the array in Bytes (sizeof(indicies))
 * @param programID - Program ID of Compiled Shaders
 * @return BufferData Object with the Object Reference IDs stored
 */
inline BufferData CreateBuffer::float_buffer(GLfloat* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader, GLenum buffer_usage) {
  GLsizei vertexStride = 9;

  /* 0. Allocate Verticies Buffer Object on GPU */
  GLuint VAO;                  // Vertex Array Object (Binds Vertex Buffer with the Attributes Specified)
  GLuint vBuffer;              // Vertex Buffer
  GLuint iBuffer;              // Element Buffer Object that specifies Order of drawing existing verticies
  glGenVertexArrays(1, &VAO);  // Create a VAO
  glGenBuffers(1, &vBuffer);   // Create One Buffer
  glGenBuffers(1, &iBuffer);   // Create Buffer for Indicies


  /* 0.5. Bind the VAO so that the data is stored in it */
  glBindVertexArray(VAO);


  /* 1. Specify how to Interpret the Vertex Data (Buffer Attribute) */
  // Bind Vertex Buffer Data
  glBindBuffer(GL_ARRAY_BUFFER, vBuffer);  // Tell OpenGL it's an Array Buffer

  /* Send the data into the Buffer Memory to Binded Buffer
   * Docs: https://docs.gl/gl4/glBufferData
	 *  GL_STATIC_DRAW:   the data will most likely not change at all or very rarely.
	 *  GL_DYNAMIC_DRAW:  the data is likely to change a lot.
	 *  GL_STREAM_DRAW:   the data will change every time it is drawn.
	 */
  glBufferData(GL_ARRAY_BUFFER, vSize, dataPack, buffer_usage);
  glEnableVertexAttribArray(0);  // Enable aPos Attribute
  glVertexAttribPointer(
    0,                                  // Which Index Attribute to Configure (At Location 0, aPos)
    3,                                  // There are Values per Vertex (x,y,z)
    GL_FLOAT,                           // Type of Data in the Array
    GL_FALSE,                           // Normalize?
    vertexStride * sizeof(GL_FLOAT),    // Stride till next Vertex
    (void*)0                            // Pointer to the Beginning position in the Buffer
  );


  /* 2. Store Index Elements Data */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indicies, buffer_usage);
  glDisableVertexAttribArray(0);  // Disable aPos Attribute


  /* 3. Configure RGB Attribute */
  GLuint aRGBA = glGetAttribLocation(shader->ID, "aRGBA");
  glEnableVertexAttribArray(aRGBA);
  glVertexAttribPointer(aRGBA, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

  /* 4. Configure Texture Coordinates Attribute */
  GLuint aTextCoord = glGetAttribLocation(shader->ID, "aTextCoord");
  glEnableVertexAttribArray(aTextCoord);
  glVertexAttribPointer(aTextCoord, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));


  /* 5. Object is ready to be Drawn */
  BufferData data(vBuffer, iBuffer, VAO);             // Create data Reference Object
  data.indiciesElts = iSize / sizeof(indicies[0]);    // Store Number of Indicies
  data.stride = vertexStride;

  // Keep track of the applied shader so that it doesn't get deallocated while in use.
  data.shader = shader;

  // Store a copy of the data.
  data.data_size_bytes = vSize;
  data.data_ptr = new GLfloat[data.data_size_bytes / sizeof(GL_FLOAT)];
  memcpy(data.data_ptr, dataPack, data.data_size_bytes);

  return data;
}

BufferData CreateBuffer::static_float(GLfloat* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader) {
  return float_buffer(dataPack, vSize, indicies, iSize, shader, GL_STATIC_DRAW);
}

BufferData CreateBuffer::stream_float(GLfloat* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader) {
  return float_buffer(dataPack, vSize, indicies, iSize, shader, GL_STREAM_DRAW);
}

BufferData CreateBuffer::dynamic_float(GLfloat* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader) {
  return float_buffer(dataPack, vSize, indicies, iSize, shader, GL_DYNAMIC_DRAW);
}