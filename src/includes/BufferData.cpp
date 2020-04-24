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


/**
 * CreateBuffer NAMESPACE
 * 
 * Creates Buffer data for Verticies & Indicies provided
 *  by creating a VAO linked to a VBO and EBO.
 * Data is configured and packaged in an Object with the
 *  reference IDs given by OpenGL and returned.
 * 
 * Data is packed in an array of:
 *   [ VERTEX<vec3>     RGBA<vec4>      Texture Coordinates<vec2> ]
 *
 * @param dataPack - Data Pack for Buffer
 * @param vSize     - Size of the array in Bytes (sizeof(verticies))
 * @param indicies - The Indicies Array, specifying the order of Vertex to be drawn
 * @param iSize     - Size of the array in Bytes (sizeof(indicies))
 * @param programID - Program ID of Compiled Shaders
 * @return BufferData Object with the Object Reference IDs stored
 */

BufferData CreateBuffer::static_float(GLfloat* dataPack, size_t vSize, GLuint* indicies, size_t iSize, GLuint programID) {
    /* 0. Allocate Verticies Buffer Object on GPU */
    GLuint VAO;                  // Vertex Array Object (Binds Vertex Buffer with the Attributes Specified)
    GLuint vBuffer;              // Vertex Buffer
    GLuint iBuffer;              // Element BUffer Object that specifies Order of drawing existing verticies
    glGenVertexArrays(1, &VAO);  // Create a VAO
    glGenBuffers(1, &vBuffer);   // Create One Buffer
    glGenBuffers(1, &iBuffer);   // Create Buffer for Indicies


    /* 0.5. Bind the VAO so that the data is stored in it */
    glBindVertexArray(VAO);


    /* 1. Specify how to Interpret the Vertex Data (Buffer Attribute) */
    // Bind Vertex Buffer Data
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);  // Tell OpenGL it's an Array Buffer

    /* Send the data into the Buffer Memory to Binded Buffer
	 *  GL_STATIC_DRAW:     the data will most likely not change at all or very rarely.
	 *  GL_DYNAMIC_DRAW:    the data is likely to change a lot.
	 *  GL_STREAM_DRAW:     the data will change every time it is drawn.
	 */
    glBufferData(GL_ARRAY_BUFFER, vSize, dataPack, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  // Enable aPos Attribute
    glVertexAttribPointer(         //
        0,                         // Which Index Attribute to Configure (At Location 0, aPos)
        3,                         // There are Values per Vertex (x,y,z)
        GL_FLOAT,                  // Type of Data in the Array
        GL_FALSE,                  // Normalize?
        9 * sizeof(GL_FLOAT),      // Stride till next Vertex
        (void*)0                   // Pointer to the Beginning position in the Buffer
    );


    /* 2. Store Index Elements Data */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indicies, GL_STATIC_DRAW);
    glDisableVertexAttribArray(0);  // Disable aPos Attribute


    /* 3. Configure RGB Attribute */
    GLuint aRGBA = glGetAttribLocation(programID, "aRGBA");
    glEnableVertexAttribArray(aRGBA);
    glVertexAttribPointer(aRGBA, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

    /* 4. Configure Texture Coordinates Attribute */
    GLuint aTextCoord = glGetAttribLocation(programID, "aTextCoord");
    glEnableVertexAttribArray(aTextCoord);
    glVertexAttribPointer(aTextCoord, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));


    /* 5. Object is ready to be Drawn */
    BufferData data(vBuffer, iBuffer, VAO);           // Create data Reference Object
    data.indiciesElts = iSize / sizeof(indicies[0]);  // Store Number of Indicies

    return data;
}
