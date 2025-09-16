#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height) //Allows for resizing the window
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //If escape was presses close the window
        glfwSetWindowShouldClose(window, true);
}

//Source code for the Vertices Shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//Source code for the Fragment Shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    if (!glfwInit()) //Initalzie glfw
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Hint what type of window is used as well as the version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Vertices (Points) Cordinates for the triangle
    GLfloat vertices[] = 
    {
        -0.5f, -0.5f, 0.0f, //Lower Left corner
        0.5f, -0.5f, 0.0f, //Lower right corner
        0.0f,  0.5f, 0.0f //Upper corner
        -0.5f / 2, -0.5f, 0.0f, //Lower Left corner
        0.5f / 2, -0.5f, 0.0f, //Lower right corner
        0.0f,  0.5f * 2, 0.0f //Upper corner
    };

    GLuint indices[] =
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1,
    };

    GLFWwindow* window = glfwCreateWindow(800, 600, "Physics_SIM", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //Initilazie Glad
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Create the vertext object and get the refrence of what is being made
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Set the shader for the vertices to the object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //Turn the vertex shaders into machine code
    glCompileShader(vertexShader);

    //Create the fragment object and get the refrence of what is being made
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Set the shaders for the freagments to the object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //Turn the fragment shaders into machine code
    glCompileShader(fragmentShader);

    //Create a program object that will store all the information
    GLuint shaderProgram = glCreateProgram();
    //Set the vertex shader to the program
    glAttachShader(shaderProgram, vertexShader);
    //Set the fragement shader to the program
    glAttachShader(shaderProgram, fragmentShader);
    //Link everything up
    glLinkProgram(shaderProgram);

    //Delete the made vertex and fragment object since they were already used
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //Create container to hold VAO and VBO
    GLuint VAO, VBO, EBO;

    //Set the amount of object a VAO will hold and bind it to current Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Set the amount of object VBO will hold and bind it to current Array Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Add the vertieces to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Add attributes of the verteices so OpenGL knows what to do
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //Enable those attributes
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Bind VBO and VAO to 0 so we don't modify the created VAO and VBO 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while(!glfwWindowShouldClose(window)) //Render Looop/Check if the window is still open
    {
        processInput(window); //Check for esc input
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Specific which program to use
        glUseProgram(shaderProgram);
        //Bind the AVO so OpenGL knows that to do
        glBindVertexArray(VAO);
        //Draw the triangle us the GL_trangles
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        
        glfwPollEvents(); //Checks for any even triggers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}