#include <GL/glew.h >
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // same as &source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // wee test
    //int test = 5;
    //int value = *(&test);
    //

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); 
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // can't do char message [length] since length is not const. 

        //alloca allocates on the stack dynamically.

        __glewGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        return 0;
    }
    return id;
}

// params are the source code. usually we would take this in from a file.
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs); // The shaders are stored inside the program now, so these can be deleted.
    glDeleteShader(fs);

    return program;
}

int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // 2D 
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // Give OpenGL the data
    //unsigned int buffer; 
    unsigned int buffer;
    glGenBuffers(1, &buffer); //Create buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Select that buffer (bind selects that buffer) ... 
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // glBindBuffer binds the buffer across OpenGL, so that is the buffer that will be used. I can remove this by writing: glBindBuffer(GL_ARRRAY_BUFFER, 0)

    // only specifying a single attribute, so only need to call this function once.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); 

        // Draw call for the buffer
        glDrawArrays(GL_TRIANGLES, 0, 3); // something you do if you do  not have an index buffer
    
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}