// seulement si glew32s.lib
#define GLEW_STATIC 1
#include "GL/glew.h"

#include <GLFW/glfw3.h>
#include "../common/ObjData.h"

#include "../common/GLShader.h"
#include "../common/Matrix.h"
#include "../glutilities/VertexArray.h"
#include "../glutilities/VertexBuffer.h"
#include "../glutilities/BufferLayout.h"
#include "../glutilities/Texture.h"
#include "../glutilities/IndexBuffer.h"
#include "../glutilities/GLObject.h"
#include "../common/TinyObjLoader.h"
#include "../glutilities/Skybox.h"
#include "../glutilities/UniformBuffer.h"

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
#include <vector>
#include <map>
#include <iostream>

#include <algorithm>
#include "../glutilities/Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <ctime>
static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);

}

static void GLCheckError() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseInputCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void createShaders();
static void createObject(const std::string& path);
static void createObjects();
static void randomizeObjectsPosition();
static void createSkybox();
static void initialize();
static void cleanUp();
float RandomNumber(float Min, float Max);

static void HelpMarker(const char* desc);

static std::vector<GLObject*> Objects;
static std::map<std::string, GLShader*> Shaders;
static GLShader* defaultShader;
static Skybox* skybox;

static int WINDOW_WIDTH = 1280;
static int WINDOW_HEIGHT = 960;

static Camera* currentCamera = nullptr;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static float lastX = (float)WINDOW_WIDTH / 2.0;
static float lastY = (float)WINDOW_HEIGHT / 2.0;
static bool firstMouse = true;
static bool registerMouse = false;

static bool rotateX = false;
static bool rotateY = false;
static bool rotateZ = false;
static float rotationSpeed = 1.0f;

static bool randomLights = false;
static float lightDelay = 0.2f;
static float lightTimer = 0.0f;
int main(void)
{
    srand(static_cast <unsigned> (time(0)));
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    glfwWindowHint(GLFW_SAMPLES, 16);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();
	
    glfwSetKeyCallback(window, inputCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseInputCallback);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    initialize();
	
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    UniformBuffer ub_view_projection("Matrices", nullptr, 2 * sizeof(float) * 16);
    UniformBuffer ub_light("Lights", nullptr, 2 * sizeof(float) * 4);

    for (const auto& [name, shader] : Shaders) {
        auto program = shader->GetProgram();
        glUseProgram(program);
        ub_view_projection.AddShader(program);
        ub_light.AddShader(program);
    }
    ub_view_projection.AddShader(skybox->GetShader().GetProgram());


    Camera camera;

    currentCamera = &camera;
    
    //persp

    float* fov = camera.GetpFOV();
    float* aspect = camera.GetpAspectRatio();
    float near_plane = 0.1f;
    float far_plane = 100.0f;

	static Vector3& cameraOrigin = camera.GetOrigin();
    Vector3* position = camera.GetpPosition();

    static Vector3* target = camera.GetpTarget();
    Vector3 up = Vector3(0, 1, 0);

    float* azimuth = camera.GetpAzimuth();
    float* elevation = camera.GetpElevation();
    float* distance = camera.GetpDistance();
     
    float* sensitivity = camera.GetpSensitivity();
    float* speed = camera.GetpSpeed();
	
    Vector4 light_direction = Vector4(0, -0.2, -1, 0);
    Vector3 light_color = Vector3(0.4, 0.4, 0.4);
	float light_intensity = 1.f;

	Vector3 skyColor = Vector3(0.5, 0.5, 1);
	Vector3 groundColor = Vector3(0.5, 1, 0.5);
	
    bool isOrtho = false;

    Matrix4x4 projection;
    Matrix4x4 view;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);
    glDepthRangef(0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {

        glfwGetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
        *aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        


        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = isOrtho ? Matrix4x4::Ortho(0, 1, 0, 1, near_plane, far_plane) : Matrix4x4::Perspective(*fov * M_PI / 180, *aspect, near_plane, far_plane);
        view = camera.LookAt(*position, *target, up);
		
        unsigned int program = Shaders["Hemisphere"]->GetProgram();
		glUseProgram(program);
		
        unsigned int skyDirLocation = glGetUniformLocation(program, "u_SkyDir");
        unsigned int skyColorLocation = glGetUniformLocation(program, "u_SkyColor");
		unsigned int groundColorLocation = glGetUniformLocation(program, "u_GroundColor");

		glUniform3fv(skyDirLocation, 1, &light_direction[0]);
		glUniform3fv(skyColorLocation, 1, &skyColor[0]);
		glUniform3fv(groundColorLocation, 1, &groundColor[0]);

		//uniform buffers
        {
            //update matrices unifrom buffer
            BufferLayout ub_Matrices_layout;
            ub_Matrices_layout.Push<float>(16); // size of Matrix4x4
            ub_view_projection.AddData(view.Transpose().GetData(), ub_Matrices_layout, true);
            ub_view_projection.AddData(projection.Transpose().GetData(), ub_Matrices_layout);

            //update light uniform buffer
            ub_light.AddDataf(&light_direction[0], 4, true);
            ub_light.AddDataf(&light_color[0], 3);
            ub_light.AddDataf(&light_intensity, 1);
        }

		const float time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;
		//imgui
        {
			
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

            ImGui_ImplOpenGL3_NewFrame(); 
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            if (ImGui::CollapsingHeader("View")) {

                ImGui::Checkbox("Orthographic", &isOrtho);

                ImGui::SliderFloat("fov", fov, 0.0f, 180.f);
                ImGui::SliderFloat("near_plane", &near_plane, 0.0f, 100.0f);
                ImGui::SliderFloat("far_plane", &far_plane, 0.0f, 10000.0f);
                
                ImGui::SliderFloat3("origin", &cameraOrigin[0], -100.0f, 100.0f);
                ImGui::SliderFloat3("position", &(*position)[0], -100.0f, 100.0f);
                ImGui::SliderFloat3("lookat", &(*target)[0], -100.0f, 100.0f);
                ImGui::SliderFloat3("up", &up[0], -1.0f, 1.0f);

                ImGui::SliderFloat("azimuth", azimuth, -180.0f, 180.0f);
				ImGui::SliderFloat("elevation", elevation, -90.0f, 90.0f);
				ImGui::SliderFloat("distance", distance, -100.0f, 100.0f);
				
				ImGui::SliderFloat("sensitivity", sensitivity, 0.0f, 10.0f);
				ImGui::SliderFloat("speed", speed, 0.0f, 10.0f);

            }


            if (ImGui::CollapsingHeader("Objects")) {
	
                static char str0[128] = "";
                ImGui::InputText("file path", str0, IM_ARRAYSIZE(str0));
                ImGui::SameLine(); HelpMarker(
                    "enter file path to load obj\n"
                    "example ../resources/untitled.obj"
                );
                if (ImGui::Button("Load .obj")) {
                    createObject(str0);
                }
				
                static int current_obj_index = 0;
                if (Objects.size() > 0) {

                    std::string objectName = std::to_string(current_obj_index) + " " + Objects[current_obj_index]->GetName();

                    if (ImGui::BeginCombo("Objects combo", objectName.c_str())) {
                        for (int i = 0; i < Objects.size(); i++) {
                            bool is_selected = (current_obj_index == i);
                            std::string s_name = std::to_string(i) + " " + Objects[i]->GetName();
                            const char* name = s_name.c_str();
                            if (ImGui::Selectable(name, is_selected))
                                current_obj_index = i;
                            if (is_selected) {
                                ImGui::SetItemDefaultFocus();
                            }

                        }
                        ImGui::EndCombo();
                    }

                    std::string objectShaderName = Objects[current_obj_index]->GetShaderName();
					
                    if (ImGui::BeginCombo("Shader combo", objectShaderName.c_str())) {

                        for(const auto& [name, shader] : Shaders) {
							bool is_selected = (objectShaderName == name);
							std::string shaderName = shader->GetName();
							if (ImGui::Selectable(shaderName.c_str(), is_selected))
								Objects[current_obj_index]->SetShader(*shader);
							if (is_selected) {
								ImGui::SetItemDefaultFocus();
							}
						}
                        ImGui::EndCombo();
                    }

                    Vector3* obj_position = Objects[current_obj_index]->GetpPosition();
                    Vector3* obj_rotation = Objects[current_obj_index]->GetpRotation();
                    Vector3* obj_scale    = Objects[current_obj_index]->GetpScale();

                    if (ImGui::Button("Randomize positions")) {
                        randomizeObjectsPosition();
                    }

                    if (ImGui::Button("Set Focus")) {
                        *target = *obj_position;
						currentCamera->GetOrigin() = *obj_position;
						currentCamera->GetTarget() = *obj_position;
                    }

                    ImGui::SliderFloat3("position", &(*obj_position)[0], -100.0f, 100.0f);
                    ImGui::SliderFloat3("rotation", &(*obj_rotation)[0], -180.0f, 180.0f);
                    ImGui::SliderFloat3("scale", &(*obj_scale)[0], -100.0f, 100.0f);

					if(ImGui::Button("RotationX")) rotateX = !rotateX;
					if(ImGui::Button("RotationY")) rotateY = !rotateY;
					if(ImGui::Button("RotationZ")) rotateZ = !rotateZ;
                    ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.0f, 10.0f);

                    if (ImGui::Button("Random lights")) {
                        randomLights = !randomLights;
                    }
                    ImGui::SliderFloat("Light delay", &lightDelay, 0.0f, 10.0f);
                }					
            }

             
            if (ImGui::CollapsingHeader("Lights")) {
                ImGui::SliderFloat3("light direction", &light_direction[0], -1.0f, 1.0f);
                ImGui::ColorEdit3("light color", &light_color[0], ImGuiColorEditFlags_Float);
				ImGui::SliderFloat("light intensity", &light_intensity, 0.0f, 1.0f);
            }
            
            if (ImGui::CollapsingHeader("Hemisphere")) {
				ImGui::SliderFloat3("sky direction", &light_direction[0], -1.0f, 1.0f);
				ImGui::ColorEdit3("sky color", &skyColor[0], ImGuiColorEditFlags_Float);
				ImGui::ColorEdit3("ground color", &groundColor[0], ImGuiColorEditFlags_Float);
            }
			

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            
            ImGui::Render();
        }
        
        lightTimer -= deltaTime;
        if (randomLights && lightTimer <= 0) {
            lightTimer = lightDelay;

            light_direction = Vector4(RandomNumber(-1, 1), RandomNumber(-1, 1), RandomNumber(-1, 1), 0);
            light_color     = Vector3(RandomNumber(0, 1), RandomNumber(0, 1), RandomNumber(0, 1));
            light_intensity = RandomNumber(0, 1);

            skyColor    = Vector3(RandomNumber(0, 1), RandomNumber(0, 1), RandomNumber(0, 1));
            groundColor = Vector3(RandomNumber(0, 1), RandomNumber(0, 1), RandomNumber(0, 1));
        }		
        
        for (GLObject* o : Objects) {
			Vector3* position = o->GetpPosition();
            Vector3* rotation = o->GetpRotation();
			Vector3* scale    = o->GetpScale();
			
            if(rotateX || rotateY || rotateZ) {				
                Vector3 new_rotation(0, 0, 0);
                if (rotateX) new_rotation[0] = time * 360 * rotationSpeed;
                if (rotateY) new_rotation[1] = time * 360 * rotationSpeed;
                if (rotateZ) new_rotation[2] = time * 360 * rotationSpeed;
                *rotation = new_rotation;
            }
            o->Render();
        }
		
        if (!isOrtho)
            //skybox->Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);

        glfwPollEvents();

    } 

    cleanUp();

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        currentCamera->ProcessKeyboard(deltaTime, FORWARD);
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		currentCamera->ProcessKeyboard(deltaTime, BACKWARD);
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		currentCamera->ProcessKeyboard(deltaTime, LEFT);
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		currentCamera->ProcessKeyboard(deltaTime, RIGHT);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		std::cout << "Camera position: " << currentCamera->GetOrigin()[0] << ", " << currentCamera->GetOrigin()[1] << ", " << currentCamera->GetOrigin()[2] << std::endl;
        firstMouse = true;
        registerMouse = true;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		registerMouse = false;
    }
}

void mouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
	if(! registerMouse)
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

static void createShaders() {
	GLShader* AmbientDiffuseSpecularShader = new GLShader("Ambient_Diffuse_Specular");
    AmbientDiffuseSpecularShader->LoadVertexShader("../shaders/Ambient_Diffuse_Specular.vs");
    AmbientDiffuseSpecularShader->LoadFragmentShader("../shaders/Ambient_Diffuse_Specular.fs");
    AmbientDiffuseSpecularShader->Create();

    GLShader* AmbientDiffuseShader = new GLShader("Ambient_Diffuse");
    AmbientDiffuseShader->LoadVertexShader("../shaders/Ambient_Diffuse.vs");
    AmbientDiffuseShader->LoadFragmentShader("../shaders/Ambient_Diffuse.fs");
    AmbientDiffuseShader->Create();

    GLShader* AmbientShader = new GLShader("Ambient");
    AmbientShader->LoadVertexShader("../shaders/Ambient.vs");
    AmbientShader->LoadFragmentShader("../shaders/Ambient.fs");
    AmbientShader->Create();
	
    GLShader* hemisphereShader = new GLShader("Hemisphere");
    hemisphereShader->LoadVertexShader("../shaders/Hemisphere.vs");
    hemisphereShader->LoadFragmentShader("../shaders/Hemisphere.fs");
    hemisphereShader->Create();

	defaultShader = AmbientDiffuseSpecularShader;
	Shaders["Ambient_Diffuse_Specular"] = AmbientDiffuseSpecularShader;
	Shaders["Ambient_Diffuse"] = AmbientDiffuseShader;
	Shaders["Ambient"] = AmbientShader;
	Shaders["Hemisphere"] = hemisphereShader;
}

static void createObject(const std::string& path) {
	GLObject* obj = new GLObject(*defaultShader);
    if(TinyObjLoader::ImportObj(path, *obj))
	    Objects.push_back(obj);
    else {
		std::cout << "Error loading object" << std::endl;
		delete obj;
	}
}

static void createObjects() { 

    createObject("../resources/Bear.obj");
    createObject("../resources/Bear1.obj");
    createObject("../resources/Fidget_Spinner.obj");
    createObject("../resources/Room.obj");
    createObject("../resources/TRAIN.obj");

}

static void randomizeObjectsPosition() {
	for(auto& obj : Objects) {
        Vector3* pos = obj->GetpPosition();
		Vector3* rot = obj->GetpRotation();
		Vector3* scale = obj->GetpScale();
		
		*pos = Vector3(RandomNumber(-10, 10), RandomNumber(-10, 10), RandomNumber(-10, 10));
	}
}

static void createSkybox() {
    skybox = new Skybox("../resources/envmaps/pisa.jpg");
}

static void initialize() {
    createShaders();
    createObjects();
    randomizeObjectsPosition();
	createSkybox();
}

static void cleanUp() {
	for(auto o : Objects) 
		delete o;
    for (auto s : Shaders) {
        s.second->Destroy();
		delete s.second;
    }
	Objects.clear();
	Shaders.clear();
	
    delete skybox;
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}