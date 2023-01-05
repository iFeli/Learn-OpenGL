#include "UserInterface.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <string>

/*
*
* Constructor & Destructor
*
*/
Pink::UserInterface::UserInterface(Settings* const settings, GLFWwindow* const window) :
	settings(settings), window(window)
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
* Private Methods
* 
*/
void Pink::UserInterface::drawInfo()
{
	int glfwWidth;
	int glfwHeight;
	glfwGetWindowSize(window, &glfwWidth, &glfwHeight);

	float imGuiWidth = 300;
	float imGuiHeight = 80;

	ImGui::SetNextWindowPos(ImVec2(glfwWidth - imGuiWidth, 0));
	ImGui::SetNextWindowSize(ImVec2(imGuiWidth, 80));
	ImGui::SetNextWindowBgAlpha(0.6f);

	if (ImGui::Begin("Info"))
	{
		// Framerate.
		ImGui::Text("Framerate");

		// Maximum Vertex Attributes.
		std::string maximumVertexAttributes = "Maximum Vertex Attributes: " + std::to_string(settings->maximumVertexAttributes);
		ImGui::Text(maximumVertexAttributes.c_str());
	}

	ImGui::End();
}

void Pink::UserInterface::drawOpenGLSettings()
{
	float imGuiWidth = 300;
	float imGuiHeight = 80;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(imGuiWidth, imGuiHeight));
	ImGui::SetNextWindowBgAlpha(0.6f);

	if (ImGui::Begin("OpenGL Settings"))
	{
		ImGui::Checkbox("Wireframe Mode", &(settings->wireframeMode));

		ImGui::ColorEdit4("Clear Color", (float *)&(settings->clearColor));
	}

	ImGui::End();
}

/*
*
* Public Methods
*
*/
void Pink::UserInterface::draw()
{
	drawInfo();
	drawOpenGLSettings();
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

void Pink::UserInterface::style()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->Colors[ImGuiCol_TitleBg] = ImColor(100, 0, 200, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(125, 0, 255, 255);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(50, 0, 100, 255);

	style->Colors[ImGuiCol_WindowBg] = ImColor(50, 0, 100);

	style->Colors[ImGuiCol_FrameBg] = ImColor(100, 0, 200, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(125, 0, 255, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 0, 100, 255);

	style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);

	//style->WindowBorderSize = 0;
	//style->WindowTitleAlign = ImVec2(0.5, 0.5);
	//style->WindowMinSize = ImVec2(900, 430);

	//style->FramePadding = ImVec2(8, 6);

	//style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	//style->Colors[ImGuiCol_ButtonActive] = ImColor(31, 30, 31, 255);
	//style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

	//style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	//style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	//style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	//style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	//style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	//style->Colors[ImGuiCol_HeaderHovered] = ImColor(46, 46, 46, 255);
}