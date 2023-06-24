//
// OPENGL 204 Dragon + illumination de Blinn-Phong + structure Material
// 

// GLEW_STATIC indique qu'on utilise la lib glew32s.lib (notez le s pour static)
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../common/ObjData.h"

#include "../common/GLShader.h"
#include "../common/Matrix.h"
#include "../common/VertexArray.h"
#include "../common/VertexBuffer.h"
#include "../common/BufferLayout.h"
#include "../common/Texture.h"
#include "../common/IndexBuffer.h"
#include "../common/GLObject.h"
#include "../common/TinyObjLoader.h"
#include "../common/Skybox.h"
#include "../common/UniformBuffer.h"

#include <vector>
#include <map>
#include <iostream>

#include <algorithm>
#include "../common/Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <ctime>

#if _MSC_VER
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT  
#endif

extern "C" {
    DLL_EXPORT uint32_t NvOptimusEnablement = 0x00000001;
    DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

#include "../common/GLShader.h"

#include "Vertex.h"
#include "Material.h"

#include "DragonData.h"

// attention cmath ne defini pas M_PI !
// ...mais math.h non plus a moins d'ajouter ce define
#define _USE_MATH_DEFINES
#include <math.h>

// attention, ce define ne doit etre specifie que dans 1 seul fichier cpp
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"

static void createObject(const std::string& path);
static void createObjects();

struct Application
{
    GLShader m_textureShader;

    int m_width;
    int m_height;

    GLuint VBO;
    GLuint IBO;
    GLuint VAO;
    GLuint TexID;

    Material m_material;

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


        m_textureShader.LoadVertexShader("transform.vs.glsl");
        m_textureShader.LoadFragmentShader("texture_material_blinnphong.fs.glsl");
        // va creer un objet OpenGL type "program"
        m_textureShader.Create();

        //      +y
        //       ^
        //       |
        //-x  <--+--> +x

                // on indique quel Shader Program on 
                // souhaite utiliser (0 = default)
        uint32_t program = GetProgram();
        //glUseProgram(program);
        // le premier parametre est un "canal" (OpenGL utilise le terme "location")
        // si le contenu des donnees est un tableau, il faut le dire a OpenGL avec glEnableVertexAttribArray()

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _countof(DragonVertices), DragonVertices, GL_STATIC_DRAW);
        // pas obligatoire:
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * _countof(DragonIndices), DragonIndices, GL_STATIC_DRAW);
        // pas obligatoire:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        // les IBO/VBO sont maintenant referencable par le VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        const int32_t POSITION = glGetAttribLocation(program, "a_Position");
        const int32_t NORMAL = glGetAttribLocation(program, "a_Normal");
        const int32_t UV = glGetAttribLocation(program, "a_TexCoords");

        glEnableVertexAttribArray(POSITION);
        // la fonction suivante decrit UN sommet
        // stride (avant-dernier param) est la distance en octets entre deux sommets
        // offset (dernier param) est l'adresse (ou position) de depart de la lecture des donnees
        // ici on souhaite ne pas prendre en compte le 3eme float de chaque sommet
        glVertexAttribPointer(POSITION
            , 3 /*nombre de composantes*/, /*type*/GL_FLOAT, GL_FALSE
            , sizeof(Vertex), (void*)offsetof(Vertex, position));

        // TODO: NORMAL
        glEnableVertexAttribArray(NORMAL);
        glVertexAttribPointer(NORMAL
            , 3 /*nombre de composantes*/, /*type*/GL_FLOAT, GL_FALSE
            , sizeof(Vertex), (void*)offsetof(Vertex, color));

        glEnableVertexAttribArray(UV);
        glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE
            , sizeof(Vertex), (void*)offsetof(Vertex, uv));


        // ATTENTION ! 
        // toujours reinitialiser le VAO a 0 avant de reinitialiser 
        // (ou changer de) VBO / IBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // texture

        glGenTextures(1, &TexID);
        glBindTexture(GL_TEXTURE_2D, TexID);
        int w, h;
        uint8_t* data = stbi_load("dragon.png", &w, &h, nullptr, STBI_rgb_alpha);
        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8
                , w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // filtre bilineaire
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // filtre trilineaire (necessite mipmap)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        // un exemple de materiau pour notre objet
        // 
        m_material.diffuseColor = { 1.0f, 0.5f, 0.31f };
        m_material.specularColor = { 0.5f, 0.25f, 0.0f };
        m_material.shininess = 256.0f;

        return true;
    }

    void Deinitialize()
    {
        glDeleteTextures(1, &TexID);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);

        m_textureShader.Destroy();
    }

    inline uint32_t GetProgram()
    {
        return m_textureShader.GetProgram();
    }

    void Render()
    {
        // portion bas gauche
        // (x0, y0) = origine, (width, height) = dimensions
        glViewport(0, 0, m_width, m_height);

        // les parametres sont R(ouge) V(ert) B(leu) A(lpha)
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);


        // Rendu 3D
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LEQUAL); // GL_LESS par defaut
        glEnable(GL_CULL_FACE); // active la suppression des faces cachees (par defaut sens "horaire")
        //glFrontFace(GL_CCW); // CCW par defaut = counter-clockwise (anti-horaire)


        uint32_t program = GetProgram();
        glUseProgram(program);
        // une variable uniforme est constante pour un "draw"
        // elle est partagee par l'ensemble du program courant
        float temps = static_cast<float>(glfwGetTime());
        const int32_t TIME = glGetUniformLocation(program
            , "u_Time");
        glUniform1f(TIME, temps);

        // TODO: multiplier Translation, Rotation, Scale en une seule matrice
        float transform_homogene4D[] = {
                                cosf(temps), 0.f, -sinf(temps), 0.0f,
                                0.f, 1.f, 0.f, 0.f,
                                sinf(temps), 0.f, cosf(temps), 0.f,
                                0.f, -3.f, -12.f, 1.f
        };

        const int32_t WORLD = glGetUniformLocation(program, "u_WorldMatrix");
        glUniformMatrix4fv(WORLD, 1, false, transform_homogene4D);

        const float zNear = 0.1f;
        const float zFar = 100.0f;
        const float aspect = float(m_width) / float(m_height);
        const float fov = 45.f * M_PI / 180.f;
        const float f = 1.0f / tanf(fov / 2.f);
        float projection[] = {
            f / aspect, 0.f, 0.f, 0.f,
            0.f, f, 0.f, 0.f,
            0.f, 0.f, (zNear + zFar) / (zNear - zFar), -1.f,
            0.f, 0.f, 2.f * zNear * zFar / (zNear - zFar), 0.f
        };

        const int32_t PROJECTION = glGetUniformLocation(program, "u_ProjectionMatrix");
        glUniformMatrix4fv(PROJECTION, 1, false, projection);

        const int32_t TEXTURE = glGetUniformLocation(program, "u_Sampler");
        glUniform1i(TEXTURE, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);

        // Blinn-Phong --
        // L'algorithme d'illumination necessite la position de la camera
        // ainsi que la position de (ou direction VERS) la lumiere
        // afin de calculer un vecteur directeur pour la reflexion speculaire
        const int32_t LIGHT_DIR = glGetUniformLocation(program, "u_LightDirection");
        // attention le repere est main droite, donc le Z positif pointe vers nous !
        vec3 lightDir = { 0.f, -1.f,  0.f };
        vec3 invLightDir = { -lightDir.x, -lightDir.y, -lightDir.z };
        glUniform3fv(LIGHT_DIR, 1, &invLightDir.x);

        const int32_t VIEW_POS = glGetUniformLocation(program, "u_ViewPosition");
        vec3 viewPosition = { 0.f, 0.f,  0.f };
        glUniform3fv(VIEW_POS, 1, &viewPosition.x);

        const int32_t MAT_DIFFUSE_COLOR = glGetUniformLocation(program, "u_Material.DiffuseColor");
        glUniform3fv(MAT_DIFFUSE_COLOR, 1, &m_material.diffuseColor.x);
        const int32_t MAT_SPECULAR_COLOR = glGetUniformLocation(program, "u_Material.SpecularColor");
        glUniform3fv(MAT_SPECULAR_COLOR, 1, &m_material.specularColor.x);
        const int32_t MAT_SHININESS = glGetUniformLocation(program, "u_Material.Shininess");
        glUniform1f(MAT_SHININESS, m_material.shininess);

        // ---

        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, _countof(DragonIndices), GL_UNSIGNED_SHORT, 0);

        // pas obligatoire de retablir le shader par defaut
        // mais c'est un exemple d'utilisation
        glUseProgram(0);
    }
};

int main(void)
{
    Application app;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Dragon final", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // IMPORTANT: a appeler APRES le MakeContextCurrent
    GLenum error = glewInit();

    // A partir de maintenant, on peut utiliser des commandes OpenGL
    app.Initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // gere le dimensionnement de la fenetre
        // alternativement glfwSetWindowSizeCallback
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        app.setSize(width, height);

        // rendu
        app.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    app.Deinitialize();

    glfwTerminate();
    return 0;
}
