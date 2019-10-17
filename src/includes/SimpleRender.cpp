#include "SimpleRender.h"

/**
 ***********************************************************
 * Private Static Methods and Callbacks
 *  - Key Presses, Mouse Button, Cursor Movements, and
 *      Mouse Scrolling Callbacks
 *
 *  - Initialization of Backend Functionallity
 *      - Shaders
 *      - Buffers
 ***********************************************************
 */

void SimpleRender::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Output Key Pressed
    printf("KEY: Key[%d], ScanCode[%d], Action[%d], Mods[%d]\n", key, scancode, action, mods);

    // Close window on 'Q' Press
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void SimpleRender::mouseBtn_callback(GLFWwindow *window, int button, int action, int mods) {
    // Output Key Pressed
    printf("MOUSE: Button[%d], Action[%d], Mods[%d]\n", button, action, mods);
}

void SimpleRender::cursorPos_callback(GLFWwindow *window, double xPos, double yPos) {
    // Output Mouse Cursor Position
    printf("CURSOR: X[%.2f], Y[%.2f]\n", xPos, yPos);
}

void SimpleRender::mouseScroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    // Output Mouse Cursor Position
    printf("SCROLL: X-off[%.2f], Y-off[%.2f]\n", xOffset, yOffset);
}

void SimpleRender::error_callback(int error, const char *description) {
    // Output any Errors
    fprintf(stderr, "Error[%d]: %s\n", error, description);
}

GLuint SimpleRender::InitShader(std::string srcFile, GLenum shaderType) {
    // Load in Source Code
    std::ifstream in(srcFile);
    if (!in.is_open()) //
        fprintf(stderr, "Shader Initialize: Source Code %s could not be loaded\n", srcFile.c_str());
    std::string vertSrc((std::istreambuf_iterator<char>(in)), //
                        std::istreambuf_iterator<char>());
    in.close();

    const char *c_str = vertSrc.c_str();

    // Compile and Store Shader
    GLuint shaderID = glCreateShader(shaderType); // Stores Reference ID
    glShaderSource(shaderID, 1, &c_str, NULL);
    glCompileShader(shaderID);


    // Check for Errors
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "Initialize Shaders: Error in Compiling Shader Source!\n" << infoLog;
        return -1;
    }

    // Return the Shader Reference ID Created
    return shaderID;
}

BufferData SimpleRender::createBuffer(GLfloat *verticies, size_t vSize, GLuint *indicies, size_t iSize) {
    /* 0. Allocate Verticies Buffer Object on GPU */
    GLuint VAO;                 // Vertex Buffer Objects References store in Vertex Array Object
    GLuint VBO;                 // All Data and Configurations stored in Vertex Buffer Object
    GLuint EBO;                 // Element BUffer Object that specifies Order of drawing existing verticies
    glGenVertexArrays(1, &VAO); // Create a VAO
    glGenBuffers(1, &VBO);      // Create One Buffer
    glGenBuffers(1, &EBO);      // Create Buffer for EBO


    // Bind VAO & VBO Data
    glBindVertexArray(VAO);             // Bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Tell OpenGL it's an Array Buffer

    /* Send the data into the Buffer Memory to Binded Buffer
     *  GL_STATIC_DRAW:     the data will most likely not change at all or very rarely.
     *  GL_DYNAMIC_DRAW:    the data is likely to change a lot.
     *  GL_STREAM_DRAW:     the data will change every time it is drawn.
     */
    glBufferData(GL_ARRAY_BUFFER, vSize, verticies, GL_STATIC_DRAW);


    // Bind EBO Data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indicies, GL_STATIC_DRAW);



    /* 1. Specify how to Interpret the Data */
    glVertexAttribPointer( //
        0,                 // Which Index Attribute to Configure (At Location 0)
        3,                 // There are Values per Vertex (x,y,z)
        GL_FLOAT,          // Type of Data in the Array
        GL_FALSE,          // Normalize?
        3 * sizeof(float), // Stride till next Vertex, 3 Value of size Float per Vertix
        (void *)0          // Pointer to the Beginning position in the Buffer
    );
    glEnableVertexAttribArray(0); // Enable the Attribute of Index for VAO



    /* 2. Object is ready to be Drawn */
    BufferData data(VAO, VBO, EBO);                  // Create data Reference Object
    data.indiciesElts = iSize / sizeof(indicies[0]); // Store Number of Indicies

    return data;
}



/**
 ***********************************************************
 * Private Helper Functions
 *
 *  - Draw Loop
 *  - Preload Function
 *      - Is Called prior to Draw
 *  - Process Input Function
 *      - Handles User Input from window
 *      - Function is Called Prior to Draw Loop
 ***********************************************************
 */

void SimpleRender::Draw() {
    // Draw the Object
    glUseProgram(programID);			// Use Default Program

    for (BufferData &bd : bufferData) {
        glBindVertexArray(bd.VAO);
        /**
         * First Arg  - Mode to Draw in
         * Second Arg - Number of Indicies to use
         * Third Arg  - Data Type data is in
         * Fourth Arg - Offsets in the data
         */
        glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void SimpleRender::Preload() {
    // Create the Data
    // clang-format off
        GLfloat verticies[] = {
            -0.4f, -0.2f, 0.0f,     // Bottom-Left
            -0.2f, -0.2f, 0.0f,     // Bottom-Right
            -0.4f,  0.2f, 0.0f,     // Top-Left
            -0.2f,  0.2f, 0.0f      // Top-Right
        };

        GLuint indicies[] = {
            0, 1, 2,                // First Triangle
            1, 2, 3                 // Second Triangle
        };
    // clang-format on

    // Create and Bind Data to Buffer
    bufferData.push_back(                                                      //
        createBuffer(verticies, sizeof(verticies), indicies, sizeof(indicies)) //
    );


    // clang-format off
        GLfloat verticies2[] = {
             0.0f,  0.3f, 0.0f,     // Bottom-Left
             0.4f,  0.3f, 0.0f,     // Bottom-Right
             0.0f, -0.3f, 0.0f,     // Top-Left
             0.4f, -0.3f, 0.0f      // Top-Right
        };
    // clang-format on
    bufferData.push_back(                                                        //
        createBuffer(verticies2, sizeof(verticies2), indicies, sizeof(indicies)) //
    );
}

void SimpleRender::processInput(GLFWwindow *window) {
    // Handle Key Input
}



/**
 ***********************************************************
 * Main User-End Methods
 *
 *  - Constructor for Creating the Object :)
 *  - Destructor for Cleaning Up
 *  - Inizializing & Starting the Engine once the User
 *      is ready!
 ***********************************************************
 */

SimpleRender::SimpleRender(unsigned int  w, unsigned int  h, const char *title) : WIDTH(w), HEIGHT(h) {
    this->title = title;
    InitRender();
}

SimpleRender::~SimpleRender() {
    /* Destroy Resources */
    printf("\nExiting, cleaning up first...\n");
    glfwDestroyWindow(window);
    glfwTerminate();
}

void SimpleRender::InitRender() {
    /* Configure OpenGL Properties */
    glfwWindowHint(GLFW_SAMPLES, 4);                               // 4x Antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Enables Forward Compatiblity
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL Profile used
}

int SimpleRender::run() {
    /* Initialize GLFW */
    glewExperimental = true; // Needed for Core Profile
    if (!glfwInit()) {
        fprintf(stderr, "Failed to Initialize GLFW\n");
        glfwTerminate();
        return -1;
    }


    /* Open a Window with OpenGL Context */
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
	glViewport(0, 0, WIDTH, HEIGHT);		// Set Rendering Dimensions

    if (!window) {
        fprintf(stderr, "Failed to open GLFW window!");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW


    /* Setup Callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouseBtn_callback);
    glfwSetCursorPosCallback(window, cursorPos_callback);
    glfwSetScrollCallback(window, mouseScroll_callback);
    glfwSetErrorCallback(error_callback);

    /* Setup GLFW Properties */
    glfwSwapInterval(1); // Default is 0, this is to prevent Tearing


    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to Initalize GLEW\n");
        glfwTerminate();
        return -1;
    }

    /** Initialize the Shaders from File Input
     *      - Compile Shaders
     *      - Attach their Reference IDs to programID
     *      - Link the Program
     */
    GLuint vertShader = InitShader("./Shaders/shader.vert", GL_VERTEX_SHADER);
    GLuint fragShader = InitShader("./Shaders/shader.frag", GL_FRAGMENT_SHADER);

    // Attach & Link Shaders
    if (vertShader != -1 && fragShader != -1) {
        programID = glCreateProgram();

        glAttachShader(programID, vertShader);
        glAttachShader(programID, fragShader);
        glLinkProgram(programID);

        // Check for Linking Errors
        char infoLog[512];
        int success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programID, 512, NULL, infoLog);
            std::cerr << "Program Linking ERROR: Failed to link\n" << infoLog;
        }
    }

    // Free up Space (No need for the shaders anymore)
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);


    /* Run Pre-Start Function */
    Preload();

    /* Keep Window open until 'Q' key is pressed */
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do {
        // Clear the Screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Process User Input
        processInput(window);

        // Draw here...
        Draw();


        // Swap Buffers
        glfwSwapBuffers(window);

        // Wait for Polling Events
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window)); // Keep Window Open util Window should Closed

    // No Issues
    return 0;
}