#include "UserInterface.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

/*
*
* Constructor & Destructor
*
*/
Pink::UserInterface::UserInterface(GLFWwindow* const window) :
	wireframeMode(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

Pink::UserInterface::~UserInterface()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/*
*
* Public Methods
*
*/
bool Pink::UserInterface::getWireframeMode()
{
	return wireframeMode;
}

void Pink::UserInterface::draw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(150, 80));

	if (ImGui::Begin("OpenGL Settings"))
	{
		ImGui::Checkbox("Wireframe Mode", &wireframeMode);
	}
	
	ImGui::End();
}

void Pink::UserInterface::newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Pink::UserInterface::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}