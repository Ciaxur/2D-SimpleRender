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
  this->vertex_buffer_ptr = nullptr;
  this->index_buffer_ptr = nullptr;
}

BufferData::BufferData(GLuint& _vertBuffer, GLuint& _indBuffer, GLuint& _vao) {
  this->VAO = _vao;
  this->verticiesBuffer = _vertBuffer;
  this->indiciesBuffer = _indBuffer;
  this->texture = nullptr;
  this->shader = nullptr;
  this->vertex_buffer_ptr = nullptr;
  this->index_buffer_ptr = nullptr;
}

void BufferData::freeBufferData(BufferData* buffer) {
  glDeleteVertexArrays(1, &buffer->VAO);
  glDeleteBuffers(1, &buffer->verticiesBuffer);
  glDeleteBuffers(1, &buffer->indiciesBuffer);

  if (buffer->texture)
    delete buffer->texture;

  if (buffer->vertex_buffer_ptr)
    delete[] buffer->vertex_buffer_ptr;

  if (buffer->index_buffer_ptr)
    delete[] buffer->index_buffer_ptr;
}

void BufferData::update() {
  glNamedBufferSubData(
    this->verticiesBuffer,
    0,
    this->vertex_buffer_size_bytes,
    this->vertex_buffer_ptr
  );

  glNamedBufferSubData(
    this->indiciesBuffer,
    0,
    this->index_buffer_size_bytes,
    this->index_buffer_ptr
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
inline BufferData CreateBuffer::float_buffer(GLdouble* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader, GLenum buffer_usage) {
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
    GL_DOUBLE,                           // Type of Data in the Array
    GL_FALSE,                           // Normalize?
    vertexStride * sizeof(GLdouble),    // Stride till next Vertex
    (void*)0                            // Pointer to the Beginning position in the Buffer
  );


  /* 2. Store Index Elements Data */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indicies, buffer_usage);
  glDisableVertexAttribArray(0);  // Disable aPos Attribute


  /* 3. Configure RGB Attribute */
  GLuint aRGBA = glGetAttribLocation(shader->ID, "aRGBA");
  glEnableVertexAttribArray(aRGBA);
  glVertexAttribPointer(aRGBA, 4, GL_DOUBLE, GL_FALSE, 9 * sizeof(GLdouble), (void*)(3 * sizeof(GLdouble)));

  /* 4. Configure Texture Coordinates Attribute */
  GLuint aTextCoord = glGetAttribLocation(shader->ID, "aTextCoord");
  glEnableVertexAttribArray(aTextCoord);
  glVertexAttribPointer(aTextCoord, 2, GL_DOUBLE, GL_FALSE, 9 * sizeof(GLdouble), (void*)(7 * sizeof(GLdouble)));


  /* 5. Object is ready to be Drawn */
  BufferData data(vBuffer, iBuffer, VAO);             // Create data Reference Object
  data.indiciesElts = iSize / sizeof(indicies[0]);    // Store Number of Indicies
  data.stride = vertexStride;

  // Keep track of the applied shader so that it doesn't get deallocated while in use.
  data.shader = shader;

  // Store a copy of the data.
  data.vertex_buffer_size_bytes = vSize;
  data.vertex_buffer_ptr        = new GLdouble[data.vertex_buffer_size_bytes / sizeof(GLdouble)];
  memcpy(data.vertex_buffer_ptr, dataPack, data.vertex_buffer_size_bytes);

  data.index_buffer_size_bytes  = iSize;
  data.index_buffer_ptr         = new GLuint[data.index_buffer_size_bytes / sizeof(GLuint)];
  memcpy(data.index_buffer_ptr, indicies, data.index_buffer_size_bytes);

  return data;
}

BufferData CreateBuffer::static_float(GLdouble* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader) {
  return float_buffer(dataPack, vSize, indicies, iSize, shader, GL_STATIC_DRAW);
}

BufferData CreateBuffer::stream_float(GLdouble* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader) {
  return float_buffer(dataPack, vSize, indicies, iSize, shader, GL_STREAM_DRAW);
}

BufferData CreateBuffer::dynamic_float(GLdouble* dataPack, size_t vSize, GLuint* indicies, size_t iSize, std::shared_ptr<Shader> shader) {
  return float_buffer(dataPack, vSize, indicies, iSize, shader, GL_DYNAMIC_DRAW);
}