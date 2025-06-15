#include "BufferData.h"
#include <spdlog/spdlog.h>
#include <memory>

BufferData::BufferData(
  std::shared_ptr<Buffer<GLdouble>> vertex_buffer, const size_t vertex_stride,
  std::shared_ptr<IndexBuffer> index_buffer, GLuint &vao_index, std::shared_ptr<Shader> shader
)
  : vertex_buffer_ptr( vertex_buffer ),
    vertex_stride( vertex_stride ),
    index_buffer_ptr( index_buffer ),
    vao_index( vao_index ),
    texture( nullptr ),
    solid_color( 255.0, 0.0, 0.0, 255.0 ),
    shader( shader ) {}

BufferData::~BufferData() {
  glDeleteVertexArrays(1, &this->vao_index);
  if ( this->texture ) { delete this->texture; }
}

void BufferData::update() {
  glNamedBufferSubData(
    this->vertex_buffer_ptr->get_gl_buffer_index(),
    0,
    this->vertex_buffer_ptr->size_in_bytes(),
    this->vertex_buffer_ptr->get_buffer()
  );

  for (size_t i = 0; i < this->index_buffer_ptr->buffers.size(); i++) {
    glNamedBufferSubData(
      this->index_buffer_ptr->buffers.at(i)->get_gl_buffer_index(),
      0,
      this->index_buffer_ptr->buffers.at(i)->size_in_bytes(),
      this->index_buffer_ptr->buffers.at(i)->get_buffer()
    );
  }
}

/**
 * Creates Buffer data for Verticies & Indicies provided
 *  by creating a vao_index linked to a VBO and EBO.
 * Data is configured and packaged in an Object with the
 *  reference IDs given by OpenGL and returned.
 *
 * Data is packed in an array of:
 *   [ VERTEX<vec3>   RGBA<vec4>    Texture Coordinates<vec2> ]
 *
 * @param vertex_buffer Vertex buffer data
 * @param index_data Index buffer data
 * @param shader shader instance to bind on buffers
 * @param buffer_usage GLenum type for binding buffers
 */
std::shared_ptr<BufferData> BufferData::create_float_buffer(
  std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
  std::shared_ptr<Shader> shader, GLenum buffer_usage
) {
  GLsizei vertex_stride = 9;

  /* 0. Allocate Verticies Buffer Object on GPU */
  GLuint vertex_gen_buffer_index;
  GLuint vao_index;                           // Vertex Array Object (Binds Vertex Buffer with the Attributes Specified)
  glGenVertexArrays(1, &vao_index);           // Create a VAO
  glGenBuffers(1, &vertex_gen_buffer_index);  // Create One Buffer

  vertex_data->set_gl_buffer_index( vertex_gen_buffer_index );

  /* 0.5. Bind the VAO so that the data is stored in it */
  glBindVertexArray(vao_index);


  /* 1. Specify how to Interpret the Vertex Data (Buffer Attribute) */
  // Bind Vertex Buffer Data
  glBindBuffer(GL_ARRAY_BUFFER, vertex_data->get_gl_buffer_index());  // Tell OpenGL it's an Array Buffer

  /* Send the data into the Buffer Memory to Binded Buffer
   * Docs: https://docs.gl/gl4/glBufferData
	 *  GL_STATIC_DRAW:   the data will most likely not change at all or very rarely.
	 *  GL_DYNAMIC_DRAW:  the data is likely to change a lot.
	 *  GL_STREAM_DRAW:   the data will change every time it is drawn.
	 */
  glBufferData(GL_ARRAY_BUFFER, vertex_data->size_in_bytes(), vertex_data->get_buffer(), buffer_usage);
  glEnableVertexAttribArray(0);  // Enable aPos Attribute
  glVertexAttribPointer(
    0,                                  // Which Index Attribute to Configure (At Location 0, aPos)
    3,                                  // There are Values per Vertex (x,y,z)
    GL_DOUBLE,                          // Type of Data in the Array
    GL_FALSE,                           // Normalize?
    vertex_stride * sizeof(GLdouble),   // Stride till next Vertex
    (void*)0                            // Pointer to the Beginning position in the Buffer
  );


  /* 2. Store Index Elements Data */
  for (size_t i = 0; i < index_data->buffers.size(); i++) {
    // Create Buffer for Indicies
    // Element Buffer Object that specifies order of drawing existing verticies
    GLuint indicies_gen_buffer_index;
    glGenBuffers(1, &indicies_gen_buffer_index);
    index_data->buffers.at(i)->set_gl_buffer_index(indicies_gen_buffer_index);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_data->buffers.at(i)->get_gl_buffer_index());
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      index_data->buffers.at(i)->size_in_bytes(),
      index_data->buffers.at(i)->get_buffer(),
      buffer_usage
    );
  }
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
  return std::shared_ptr<BufferData>( new BufferData( vertex_data, vertex_stride, index_data, vao_index, shader ) );
}

std::shared_ptr<BufferData> BufferData::create_static_float(
  std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
  std::shared_ptr<Shader> shader
) {
  return create_float_buffer( vertex_data, index_data, shader, GL_STATIC_DRAW );
}

std::shared_ptr<BufferData> BufferData::create_stream_float(
  std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
  std::shared_ptr<Shader> shader
) {
  return create_float_buffer( vertex_data, index_data, shader, GL_STREAM_DRAW );
}

std::shared_ptr<BufferData> BufferData::create_dynamic_float(
  std::shared_ptr<Buffer<GLdouble>> &vertex_data, std::shared_ptr<IndexBuffer> &index_data,
  std::shared_ptr<Shader> shader
) {
  return create_float_buffer( vertex_data, index_data, shader, GL_DYNAMIC_DRAW );
}
