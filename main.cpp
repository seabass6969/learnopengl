#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// event listener for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width, height);
}

// input listener
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void checkShaderSuccess(unsigned int *shader){
    // check successful
    int success;
    char infoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void checkShaderProgramSuccess(unsigned int* shaderProgram){
    int success;
    char infoLog[512];
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

// graphics pipeline 
// vertex shader -> geometry shader -> shape assembly -> rasterization -> fragment shader -> test and blending

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "learnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Unable to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Unable to load GLAD" << std::endl;
        return -1;
    }
    // location of the lower left corner of the window
    // and windows size width and height
    glViewport(0,0,800,600);

    // event listener for window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // define variables for rendering 
    // all coordinate in openGL are 3d -> x,y,z
    // opengl only process 3d coordinates when they're in range between -1 to 1 in all 3 axes
    // coordinates within this is called normalized device coordinate 

    // vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    checkShaderSuccess(&vertexShader);

    // fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    checkShaderSuccess(&fragmentShader);

    // Shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    checkShaderProgramSuccess(&shaderProgram);

    // delete shader object after linked into the program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    // VBO are used to store large number of vertices in the GPU's memory
    // it can be used to send large batches of data all at once to the GPU
    // sending data to the GPU is very slow. try to send as much data at once
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);

    // has a unique ID corresponding to that buffer
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // buffer type of GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copies the previously defined vertex data into the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // target, 
    // size, 
    // data, 
    // (how the gpu manage the data 
        // (GL_STATIC_DRAW: set only once and used many times) useful for static object
        // (GL_DYNAMIC_DRAW : change a lot and used many times) useful for data that change frequently
    // )



    // link vertex attribute
    // vertex buffer data formating: 
    // (vertex 1)  (vertex 2)  (vertex 3)
    //   X Y Z       X Y Z       X Y Z
    // --stride->  --stride->  --stride->
    // each position data are stored as 4 bytes floating point values
    // each position is composed of 3 of those values
    // the values are tightly packed in the array 
    // first value in the data is at the beginning of the buffer 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // which vertex attribute we want to configure. The location of the position vertex from the vertex shader.
    // vec3 so size 3
    // type of data
    // Normalized data or not
    // stride, space between conscutive vertex attributes (0 can be set to let OpenGL determine thge stride. (only works when values are tightly packed))


    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // event loop
    while (!glfwWindowShouldClose(window)){
        // input 
        processInput(window);

        // rendering commands 
            // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

            // draw
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap buffer
        glfwSwapBuffers(window);
        // glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) 
        // that is used to render to during this render iteration and show it as output to the screen.
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
