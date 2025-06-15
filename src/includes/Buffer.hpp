#pragma once

#include <GL/glew.h>

#include <cstddef>
#include <cstring>
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
template <typename T>
struct Buffer {
  private:
    bool _is_buffer_bound = false;
    GLuint _gl_buffer_index = 0;

    Buffer() noexcept {}

  public:
    /**
     * Creates a new Buffer with the given number of elements pre-allocated
     * on the heap.
     *
     * @param num_elts number of elements to allocate.
     *
     * @return shared pointer instance to the created Buffer.
     */
    static std::shared_ptr<Buffer> create( const size_t num_elts ) noexcept {
      std::shared_ptr<Buffer> new_buffer( new Buffer() );

      new_buffer->buffer               = new T[num_elts];
      new_buffer->buffer_size_in_bytes = sizeof( T ) * num_elts;
      new_buffer->num_elts             = num_elts;

      return new_buffer;
    }

    /**
     * Creates a new Buffer by copying the given buffer data into an internal
     * buffer.
     *
     * @param buffer buffer to copy internally.
     * @param num_elts number of elements that the buffer array contains.
     *
     * @return shared pointer instance to the created Buffer.
     */
    static std::shared_ptr<Buffer> create( const T *buffer, const size_t num_elts ) noexcept {
      std::shared_ptr<Buffer> new_buffer( new Buffer() );

      new_buffer->buffer               = new T[num_elts];
      new_buffer->buffer_size_in_bytes = sizeof( T ) * num_elts;
      new_buffer->num_elts             = num_elts;

      // Copy data internally.
      memcpy(new_buffer->buffer, buffer, new_buffer->buffer_size_in_bytes);

      return new_buffer;
    }

    ~Buffer() noexcept {
      if ( this->_is_buffer_bound ) { glDeleteBuffers( 1, &this->_gl_buffer_index ); }
      delete[] buffer;
    }

    // Properties
    /**
     * Returns the number of elements in the buffer.
     */
    size_t size() { return num_elts; }

    /**
     * Returns the buffer size in bytes.
     */
    size_t size_in_bytes() { return buffer_size_in_bytes; }

    /**
     * Returns buffer data at the given index.
     */
    T at( const size_t index ) { return this->buffer[index]; }

    /**
     * Returns the internal buffer.
     */
    T *get_buffer() { return this->buffer; }

    GLuint &get_gl_buffer_index() { return this->_gl_buffer_index; }
    void set_gl_buffer_index( GLuint index ) {
      this->_is_buffer_bound = true;
      this->_gl_buffer_index = index;
    }

  protected:
    T *buffer;
    size_t buffer_size_in_bytes;
    size_t num_elts;
};
