// seulement si glew32s.lib
#define GLEW_STATIC 1
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#pragma region common
#include "../common/GLShader.h"
#include "../common/ObjData.h"
#include "../common/Matrix.h"
#include "../common/VertexArray.h"
#include "../common/VertexBuffer.h"
#include "../common/BufferLayout.h"
#include "../common/Texture.h"
#include "../common/UniformBuffer.h"
#include "../common/IndexBuffer.h"
#include "../common/GLObject.h"
#include "../common/TinyObjLoader.h"
#include "../common/Skybox.h"
#include "../common/Camera.h"

#pragma region imgui
#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"


#pragma region cpp_libraries
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <ctime>
#pragma endregion cpp_libraries

using namespace std;
using namespace ImGui;

vector<GLObject*> Objects;
map<string, GLShader*> Shaders;
GLShader* defaultShader;
Skybox* skybox;

int mWidth = 1280;
int mHeight = 720;

static bool randomLights = false;
static float lightDelay = 0.2f;
static float lightTimer = 0.0f;

#pragma region Camera
    #pragma region Variables

    Camera* currentCamera = nullptr;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float lastX = (float)mWidth / 2.0;
    float lastY = (float)mHeight / 2.0;
    bool firstMouse = true;
    bool registerMouse = false;

    bool rotateX = false;
    bool rotateY = false;
    bool rotateZ = false;
    float rotationSpeed = 1.0f;

    #pragma endregion Variables
    #pragma region Functions

    void mouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
        if (!registerMouse)
            return;
        float xpos = (float)x_pos;
        float ypos = (float)y_pos;

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        currentCamera->ProcessMouse(xoffset, yoffset);
    }

    void mouseInputCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
            firstMouse = true;
            registerMouse = true;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
            registerMouse = false;
        }
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        currentCamera->ProcessScroll(yoffset);
    }
    #pragma endregion Functions
#pragma endregion Camera

#pragma region ObjectFunctions
   
    float RandomNumber(float min, float max)
    {
        return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
    }
    void createObject(const string& path) {
        GLObject* obj = new GLObject(*defaultShader);
        if (TinyObjLoader::ImportObject(path, *obj)){
            Objects.push_back(obj);
            Vector3* pos = obj->GetpPosition();
            Vector3* rot = obj->GetpRotation();
            Vector3* scale = obj->GetpScale();

            *pos = Vector3(RandomNumber(-20, 20), RandomNumber(-20, 20), RandomNumber(-20, 20));
            *rot = Vector3(RandomNumber(0, 360), RandomNumber(0, 360), RandomNumber(0, 360));
        }
        else {
            cout << "Problème lors de l'import de l'objet" << endl;
            delete obj;
        }
    }

    

    void InitializeScene() {
        //Create Skybox
        skybox = new Skybox("../ressources/skybox/skybox.jpg");

        //Create Shaders

        GLShader* AmbientDiffuseSpecularShader = new GLShader("Ambient_Diffuse_Specular");
        AmbientDiffuseSpecularShader->LoadVertexShader("../shaders/Ambient_Diffuse_Specular.vs");
        AmbientDiffuseSpecularShader->LoadFragmentShader("../shaders/Ambient_Diffuse_Specular.fs");
        AmbientDiffuseSpecularShader->Create();


        GLShader* hemisphereShader = new GLShader("Hemisphere");
        hemisphereShader->LoadVertexShader("../shaders/Hemisphere.vs");
        hemisphereShader->LoadFragmentShader("../shaders/Hemisphere.fs");
        hemisphereShader->Create();

        defaultShader = AmbientDiffuseSpecularShader;
        Shaders["Ambient_Diffuse_Specular"] = AmbientDiffuseSpecularShader;
        Shaders["Hemisphere"] = hemisphereShader;

        //Create Objects

        createObject("../ressources/blue_box.obj");
        createObject("../ressources/earth.obj");
        createObject("../ressources/Only_Spider_with_Animations_Export.obj");
        createObject("../ressources/Room2.obj");


    }

    void DeinitializeScene() {
        for (auto o : Objects)
            delete o;
        for (auto s : Shaders) {
            s.second->Destroy();
            delete s.second;
        }
        Objects.clear();
        Shaders.clear();

        delete skybox;
    }

#pragma endregion ObjectFunctions

int main(void)
{
    srand(static_cast <unsigned> (time(0)));
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 16);

    window = glfwCreateWindow(mWidth, mHeight, "OpenGL Projet : Attack On Spider", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();
	
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseInputCallback);


    //ImGui
    CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    StyleColorsDark();

    InitializeScene();
	
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    UniformBuffer ubViewProj("Matrices", nullptr, 2 * sizeof(float) * 16);
    UniformBuffer ubLight("Lights", nullptr, 2 * sizeof(float) * 4);

    for (const auto& [name, shader] : Shaders) {
        auto program = shader->GetProgram();
        glUseProgram(program);
        ubViewProj.AddShader(program);
        ubLight.AddShader(program);
    }
    ubViewProj.AddShader(skybox->GetShader().GetProgram());

    Camera camera;

    currentCamera = &camera;
    
    //perspective



    Matrix4x4 projection;
    Matrix4x4 view;

    bool isOrtho = false;

    float* fov = camera.GetpFOV();
    float* aspect = camera.GetpAspectRatio();
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

	Vector3& cameraOrigin = camera.GetOrigin();
    Vector3* position = camera.GetpPosition();

    Vector3* target = camera.GetpTarget();
    Vector3 up = Vector3(0, 1, 0);

    float* azimuth = camera.GetpAzimuth();
    float* elevation = camera.GetpElevation();
    float* distance = camera.GetpDistance();
     
    float* sensitivity = camera.GetpSensitivity();
    float* speed = camera.GetpSpeed();
	

    //light
    Vector4 lightDirection = Vector4(0, -0.2, -1, 0);
    Vector3 lightColor = Vector3(1, 1, 1);
	float lightIntensity = 1.f;

	Vector3 skyColor = Vector3(1, 1, 1);
	Vector3 groundColor = Vector3(1, 1, 1);
	
   

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);
    glDepthRangef(0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {

        glfwGetWindowSize(window, &mWidth, &mHeight);
        *aspect = (float)mWidth / (float)mHeight;
        

        glViewport(0, 0, mWidth, mHeight);
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (isOrtho) {
            projection = Matrix4x4::Ortho(0, 1, 0, 1, nearPlane, farPlane);
        }
        else {
            projection = Matrix4x4::Perspective(*fov * M_PI / 180, *aspect, nearPlane, farPlane);
        }

        view = camera.LookAt(*position, *target, up);
		
        unsigned int program = Shaders["Hemisphere"]->GetProgram();
		glUseProgram(program);
		
        unsigned int skyDirLocation = glGetUniformLocation(program, "u_SkyDir");
        unsigned int skyColorLocation = glGetUniformLocation(program, "u_SkyColor");
		unsigned int groundColorLocation = glGetUniformLocation(program, "u_GroundColor");

		glUniform3fv(skyDirLocation, 1, &lightDirection[0]);
		glUniform3fv(skyColorLocation, 1, &skyColor[0]);
		glUniform3fv(groundColorLocation, 1, &groundColor[0]);

		//uniform buffers
        {
            //update matrices unifrom buffer
            BufferLayout ubMatricesLayout;
            ubMatricesLayout.Push<float>(16); // size of Matrix4x4
            ubViewProj.AddData(view.Transpose().GetData(), ubMatricesLayout, true);
            ubViewProj.AddData(projection.Transpose().GetData(), ubMatricesLayout);

            //update light uniform buffer
            ubLight.AddDataf(&lightDirection[0], 4, true);
            ubLight.AddDataf(&lightColor[0], 3);
            ubLight.AddDataf(&lightIntensity, 1);
        }

		const float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;
		//Debugger ImGui
        {
			
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

            ImGui_ImplOpenGL3_NewFrame(); 
            ImGui_ImplGlfw_NewFrame();
            NewFrame();
            
                Checkbox("Orthographic", &isOrtho);

                SliderFloat("fov", fov, 0.0f, 180.f);
                SliderFloat("nearPlane", &nearPlane, 0.0f, 100.0f);
                SliderFloat("farPlane", &farPlane, 0.0f, 10000.0f);
                
                SliderFloat3("origin", &cameraOrigin[0], -100.0f, 100.0f);
                SliderFloat3("lookat", &(*target)[0], -100.0f, 100.0f);

                SliderFloat("azimuth", azimuth, -180.0f, 180.0f);
				SliderFloat("elevation", elevation, -90.0f, 90.0f);
				SliderFloat("distance", distance, -100.0f, 100.0f);
				
				SliderFloat("speed", speed, 0.0f, 10.0f);

            Render();
        }


        for (GLObject* object : Objects) {
			Vector3* position = object->GetpPosition();
            Vector3* rotation = object->GetpRotation();
            Vector3* scale = object->GetpScale();

            object->Render();
        }
       
        skybox->Render();

        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
        
        glfwSwapBuffers(window);

        glfwPollEvents();

    } 

    DeinitializeScene();

    ImGui_ImplGlfw_Shutdown();
    DestroyContext();

    glfwTerminate();
    return 0;
}

