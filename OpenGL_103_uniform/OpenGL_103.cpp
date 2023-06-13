//
// OPENGL 103 uniform
// 

// GLEW_STATIC indique qu'on utilise la lib glew32s.lib (notez le s pour static)
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#if _MSC_VER
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT  
#endif

extern "C" {
    DLL_EXPORT uint32_t NvOptimusEnablement = 0x00000001;
    //DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

#include "../common/GLShader.h"

struct vec2
{
    float x; // offset (ou adresse relative) = 0
    float y; // offset = sizeof(x) = 4
};

struct vec3
{
    float x; // offset = 0
    float y; // offset = sizeof(x) = 4
    float z; // offset = sizeof(x) + sizeof(y) = 8
};
using Color = vec3; // equivalent de typedef vec3 color;

struct Vertex                                       // sizeof(Vertex) = 
{
    vec2 position;  // offset = 0                   // sizeof(vec2) +
    Color color;    // offset = sizeof(position)    // sizeof(vec3)
};



struct Application
{
    GLShader m_basicShader;

    int m_width;
    int m_height;

    inline void setSize(int width, int height) {
        m_width = width;
        m_height = height;
    }

    bool Initialize()
    {
        std::cout << "Carte Graphique : " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version OpenGL : " << glGetString(GL_VERSION) << std::endl;
        std::cout << "version GLSL : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        //std::cout << "Extensions disponibles : " << glGetString(GL_EXTENSIONS) << std::endl;

        int numExtensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        for (int i = 0; i < numExtensions; i++) {
            std::cout << "Extensions[" << i << "] " << glGetStringi(GL_EXTENSIONS, i) << std::endl;
        }


        m_basicShader.LoadVertexShader("basic_uniform.vs.glsl");
        m_basicShader.LoadFragmentShader("basic_uniform.fs.glsl");
        // va creer un objet OpenGL type "program"
        m_basicShader.Create();

        return true;
    }

    void Deinitialize()
    {
        m_basicShader.Destroy();
    }

    inline uint32_t GetBasicProgram()
    {
        return m_basicShader.GetProgram();
    }

    void Render()
    {
        glEnable(GL_CULL_FACE); // active la suppression des faces cachees (par defaut sens "horaire")
        //glFrontFace(GL_CCW); // CCW par defaut = counter-clockwise (anti-horaire)

        // portion bas gauche
        // (x0, y0) = origine, (width, height) = dimensions
        glViewport(0, 0, m_width, m_height);

        // les parametres sont R(ouge) V(ert) B(leu) A(lpha)
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //      +y
        //       ^
        //       |
        //-x  <--+--> +x
        //
        // en pratique il n'est pas utile ici d'indiquer explicitement
        // le nom du constructeur (Vertex, vec2, Color) car
        // le compilateur peut le deduire automatiquement grace aux {}
        const Vertex triangle[] = {
            Vertex{vec2{-0.8f, +0.8f}, Color{1.0f, 0.0f, 0.0f}},// sommet #0
            Vertex{vec2{+0.0f, -0.8f}, Color{0.0f, 1.0f, 0.0f}},// sommet #1
            Vertex{vec2{+0.8f, +0.8f}, Color{0.0f, 0.0f, 1.0f}} // sommet #2
        };


        // on indique quel Shader Program on 
        // souhaite utiliser (0 = default)
        uint32_t program = GetBasicProgram();
        glUseProgram(program);
        // le premier parametre est un "canal" (OpenGL utilise le terme "location")
        // si le contenu des donnees est un tableau, il faut le dire a OpenGL avec glEnableVertexAttribArray()
        
        // une variable uniforme est constante pour un "draw"
        // elle est partagee par l'ensemble du program courant
        float temps = static_cast<float>(glfwGetTime());
        const int32_t TIME = glGetUniformLocation(program
                                                , "u_Time");
        glUniform1f(TIME, temps);

        const int32_t POSITION = glGetAttribLocation(program, "a_Position");
        const int32_t COLOR = glGetAttribLocation(program, "a_Color");
        
        glEnableVertexAttribArray(POSITION);
        // la fonction suivante decrit UN sommet
        // stride (avant-dernier param) est la distance en octets entre deux sommets
        // offset (dernier param) est l'adresse (ou position) de depart de la lecture des donnees
        // ici on souhaite ne pas prendre en compte le 3eme float de chaque sommet
        glVertexAttribPointer(POSITION
            , 2 /*nombre de composantes*/, /*type*/GL_FLOAT, GL_FALSE
            , sizeof(Vertex), &triangle->position);
        // stride = vec2 + vec3 = sizeof(Vertex)
        glEnableVertexAttribArray(COLOR);
        glVertexAttribPointer(COLOR
            , 3 /*nombre de composantes*/, /*type*/GL_FLOAT, GL_FALSE
            , sizeof(Vertex), &triangle->color);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // pas obligatoire de retablir le shader par defaut
        // mais c'est un exemple d'utilisation
        glUseProgram(0);
    }
};
int main(void)
{
    GLFWwindow* window;
    Application application;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // A faire après glfwMakeContextCurrent
    GLenum error = glewInit();

    // A partir de maintenant on peut utiliser des commandes openGL
    application.Initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // gere le dimensionnement de la fenêtre
        // alternativement glfwSetWindowSizeCallback
        int width, height;

        glfwGetWindowSize(window, &width, &height);
        application.setSize(width, height);
        application.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    application.Deinitialize();

    glfwTerminate();
    return 0;
}
