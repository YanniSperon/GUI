#include "Global.h"
#include "Console.h"
#include "Input.h"
#include "Random.h"
#include "Shader.h"
#include "View.h"
#include "ImageView.h"
#include "Text.h"
#include "MathExtensions.h"
#include "ButtonView.h"

#include <glew.h>
#include <glfw3.h>
#include <vector>
#include <string>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define DGPU

#ifdef DGPU
#include <Windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

static void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		Console::FatalError("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Console::Error("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		Console::Warning("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		//Console::Info("GL NOTIFICATION:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
		break;
	}
}

static double mouseXPos = 0.0;
static double mouseYPos = 0.0;

static int windowWidth = 1920;
static int windowHeight = 1080;

static bool didMove = false;

static bool shouldRecalculate = false;

int main() {

	Global::Initialize();

	Console::Assert(glfwInit(), "Failed GLFW Initialization!");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "GUI", NULL, NULL);
	Input* input = new Input();
	input->SetShouldCaptureKeyboardInput(true);
	input->SetShouldCaptureMouseInput(true);
	Console::Assert(window, "Failed Window Creation!");
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, input);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLenum err = glewInit();
	Console::Assert(err == GLEW_OK, "Failed GLEW Initialization - %s", reinterpret_cast<char const*>(glewGetErrorString(err)));
	//////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glCullFace(GL_CCW);
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);
	}
	glfwSwapInterval(0);
	glDebugMessageCallback(GLDebugMessageCallback, 0);
	glfwSetErrorCallback([](int error, const char* description)
		{
			Console::Error("GLFW Error (%i): \"%s\"", error, description);
		}
	);
	//////////////////////////////////////////////////////////////////////////////////////////////
	glfwSetCursorPosCallback(window, [](GLFWwindow* glfwWindow, double xPos, double yPos)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->MoveMouseTo(xPos, yPos);

			mouseXPos = xPos;
			mouseYPos = yPos;

			didMove = true;
		}
	);
	glfwSetMouseButtonCallback(window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateMouseButton(button, 1);
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateMouseButton(button, 3);
				break;
			}
			}
		}
	);
	glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateKeyboardKey(key, 1);
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateKeyboardKey(key, 3);
				break;
			}
			}
		}
	);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			windowWidth = width;
			windowHeight = height;

			glViewport(0, 0, windowWidth, windowHeight);

			shouldRecalculate = true;
		}
	);
	glfwSetCharCallback(window, [](GLFWwindow* glfwWindow, unsigned int keycode)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);
		}
	);
	glfwSetScrollCallback(window, [](GLFWwindow* glfwWindow, double xOffset, double yOffset)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);
		}
	);
	glfwSetCursorEnterCallback(window, [](GLFWwindow* glfwWindow, int entered)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->SetMouseWasBlocked(true);
		}
	);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);






	Shader textShader("Resources/Shaders/TempText", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER);
	GUI::Text* text = new GUI::Text("C:\\Windows\\Fonts\\times.ttf", 72, false, 0, 500, GUI::Text::Alignment::LEFT, "FPS %.1f", 0.0f);

	GUI::Text* textTemp = new GUI::Text("C:\\Windows\\Fonts\\times.ttf", 72, false, 0, 500, GUI::Text::Alignment::LEFT, "VAVAVAVAavavawndianuiodnwuia|qk");
	textTemp->SetColor(Math::Scale255To1(glm::vec4(255.0f, 0.0f, 255.0f, 255.0f)));
	textTemp->SetTranslation(glm::vec2(960.0f, 540.0f));



	Shader shader("Resources/Shaders/2D", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER);
	shader.Bind();

	GUI::View* mainView = new GUI::View(0.0f, 0.0f, (float)windowWidth, (float)windowHeight);
	mainView->SetCornerRoundness(0.0f);
	mainView->SetTintColor(Math::Scale255To1(glm::vec4(75.0f, 75.0f, 75.0f, 255.0f)));
	mainView->SetWidthConstraint(100.0f, GUI::View::ConstraintMeasurementType::PERCENT);
	mainView->SetHeightConstraint(100.0f, GUI::View::ConstraintMeasurementType::PERCENT);
	mainView->Recalculate(0.0f, 0.0f, (float)windowWidth, (float)windowHeight);

	GUI::ImageView* subView = new GUI::ImageView(mainView);
	subView->SetTexture("Resources/Textures/sexything.png");
	subView->SetBorderWeight(10.0f);
	subView->SetBorderColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	subView->SetTintColor(Math::Scale255To1(glm::vec4(222.0f, 170.0f, 29.0f, 255.0f)));
	subView->SetWidthConstraint(500.0f, GUI::View::ConstraintMeasurementType::PIXELS);
	subView->SetHeightConstraint(500.0f, GUI::View::ConstraintMeasurementType::PIXELS);
	subView->SetXConstraint(0.0f, GUI::View::ConstraintMeasurementType::PIXELS, GUI::View::XConstraintLocation::CENTER);
	subView->SetYConstraint(0.0f, GUI::View::ConstraintMeasurementType::PIXELS, GUI::View::YConstraintLocation::CENTER);
	subView->Recalculate();

	GUI::ButtonView* subView2 = new GUI::ButtonView(subView);
	subView2->SetBorderWeight(10.0f);
	subView2->SetBorderColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	subView2->SetTintColor(Math::Scale255To1(glm::vec4(222.0f, 170.0f, 29.0f, 255.0f)));
	subView2->SetWidthConstraint(50.0f, GUI::View::ConstraintMeasurementType::PERCENT);
	subView2->SetHeightConstraint(50.0f, GUI::View::ConstraintMeasurementType::PERCENT);
	subView2->SetXConstraint(0.0f, GUI::View::ConstraintMeasurementType::PERCENT, GUI::View::XConstraintLocation::CENTER);
	subView2->SetYConstraint(0.0f, GUI::View::ConstraintMeasurementType::PERCENT, GUI::View::YConstraintLocation::CENTER);
	subView2->SetCallbackFunction([&subView2, &subView]() {
		subView2->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		subView->SetTintColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	});
	//int ammo = 10;
	//subView2->SetCallbackFunction([&ammo]() {
	//	ammo--;
	//});
	subView2->Recalculate();

	/*GUI::View* subView = new GUI::View(mainView);

	subView->SetTintColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	subView->SetWidthConstraint(40.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	subView->SetHeightConstraint(40.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	subView->SetXConstraint(0.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT, VIEW_CONSTRAINT_LOCATION_LEFT);
	subView->Recalculate();

	GUI::View* ssubView = new GUI::View(subView);
	ssubView->SetShouldClipToBounds(true);
	ssubView->SetSizeMode(VIEW_SIZE_MODE_ASPECT_FIT, 1.5f);
	ssubView->SetTintColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	ssubView->SetWidthConstraint(80.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	ssubView->SetHeightConstraint(60.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	ssubView->Recalculate();

	GUI::View* subView2 = new GUI::View(mainView);

	subView2->SetTintColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	subView2->SetWidthConstraint(40.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	subView2->SetHeightConstraint(40.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	subView2->SetXConstraint(0.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT, VIEW_CONSTRAINT_LOCATION_RIGHT);
	subView2->Recalculate();

	GUI::View* ssubView2 = new GUI::View(subView2);
	ssubView2->SetShouldClipToBounds(true);
	ssubView2->SetSizeMode(VIEW_SIZE_MODE_ASPECT_FIT, 1.5f);
	ssubView2->SetTintColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	ssubView2->SetWidthConstraint(80.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	ssubView2->SetHeightConstraint(60.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	ssubView2->Recalculate();

	GUI::ImageView* sssubView2 = new GUI::ImageView(ssubView2);
	sssubView2->SetTexture("Resources/Textures/Basic.png");
	sssubView2->SetShouldClipToBounds(true);
	sssubView2->SetSizeMode(VIEW_SIZE_MODE_ASPECT_FIT, 1.5f);
	sssubView2->SetCornerRoundness(0.5f);
	sssubView2->SetWidthConstraint(80.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	sssubView2->SetHeightConstraint(60.0f, VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT);
	sssubView2->Recalculate();*/

	glm::mat4 proj = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);

	double timeConstant = 1.0;
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	

	while (!glfwWindowShouldClose(window)) {
		currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTimeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
		lastTime = currentTime;
		double deltaTimeWithoutTimeFactor = deltaTimeNanoseconds.count() / 1000000000.0;
		double deltaTime = deltaTimeWithoutTimeFactor * timeConstant;

		glfwPollEvents();
		input->Prepare();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (shouldRecalculate) {
			mainView->Recalculate(0, 0, windowWidth, windowHeight);
			proj = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);;
		}
		text->SetText("FPS %.1f", 1.0 / deltaTime);
		if (input->GetMouseButtonPressed(AD_MOUSE_BUTTON_1)) {
			GUI::View* temp = mainView->HitTest(input->GetMousePositionX(), ((float)windowHeight) - input->GetMousePositionY());
			if (temp) {
				Console::Info("    Hit Test - Color: (%.2f, %.2f, %.2f, %.2f)", temp->GetTintColor().x, temp->GetTintColor().y, temp->GetTintColor().z, temp->GetTintColor().w);
			}
			else {
				Console::Info("    Hit Test - NO RESULT");
			}
		}
		if (input->GetMouseButtonHeld(AD_MOUSE_BUTTON_2)) {
			Console::Info("Mouse Position: (%.2f, %.2f)", input->GetMousePositionX(), input->GetMousePositionY());
		}


		mainView->Draw(proj, shader);
		text->Draw(proj, textShader);
		text->SetTranslation(glm::vec2(input->GetMousePositionX(), (float)windowHeight - input->GetMousePositionY()));
		textTemp->Draw(proj, textShader);

		glfwSwapBuffers(window);

		input->Flush();
		Global::Update();
		didMove = false;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	Global::Cleanup();





















































	/*
	Global::Initialize();

	Console::Assert(glfwInit(), "Failed GLFW Initialization!");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	Input* input = new Input();
	input->SetShouldCaptureKeyboardInput(true);
	input->SetShouldCaptureMouseInput(true);
	Console::Assert(window, "Failed Window Creation!");
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, input);
	//////////////////////////////////////////////////////////////////////////////////////////////
	GLenum err = glewInit();
	Console::Assert(err == GLEW_OK, "Failed GLEW Initialization - %s", reinterpret_cast<char const*>(glewGetErrorString(err)));
	//////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_CCW);
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, 1);
	}
	glfwSwapInterval(0);
	glDebugMessageCallback(GLDebugMessageCallback, 0);
	glfwSetErrorCallback([](int error, const char* description)
		{
			Console::Error("GLFW Error (%i): \"%s\"", error, description);
		}
	);
	//////////////////////////////////////////////////////////////////////////////////////////////
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(windowWidth, windowHeight);
	static auto font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 14);
	io.IniFilename = nullptr;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.KeyMap[ImGuiKey_Tab] = AD_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = AD_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = AD_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = AD_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = AD_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = AD_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = AD_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = AD_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = AD_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = AD_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = AD_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = AD_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = AD_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = AD_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = AD_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = AD_KEY_KP_ENTER;
	io.KeyMap[ImGuiKey_A] = AD_KEY_A;
	io.KeyMap[ImGuiKey_C] = AD_KEY_C;
	io.KeyMap[ImGuiKey_V] = AD_KEY_V;
	io.KeyMap[ImGuiKey_X] = AD_KEY_X;
	io.KeyMap[ImGuiKey_Y] = AD_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = AD_KEY_Z;
	ImGui_ImplOpenGL3_Init("#version 460");
	//////////////////////////////////////////////////////////////////////////////////////////////
	glfwSetCursorPosCallback(window, [](GLFWwindow* glfwWindow, double xPos, double yPos)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->MoveMouseTo(xPos, yPos);

			mouseXPos = xPos;
			mouseYPos = yPos;

			ImGuiIO& io = ImGui::GetIO();
			io.MousePos = ImVec2(xPos, yPos);

			didMove = true;
		}
	);
	glfwSetMouseButtonCallback(window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateMouseButton(button, 1);
				if (io.WantCaptureMouse) {
					io.MouseDown[button] = true;
				}
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateMouseButton(button, 3);
				if (io.WantCaptureMouse) {
					io.MouseDown[button] = false;
				}
				break;
			}
			}
		}
	);
	glfwSetKeyCallback(window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();

			switch (action)
			{
			case GLFW_PRESS:
			{
				input->UpdateKeyboardKey(key, 1);
				if (io.WantCaptureKeyboard) {
					io.KeysDown[key] = true;
					
					io.KeyCtrl = io.KeysDown[AD_KEY_LEFT_CONTROL] || io.KeysDown[AD_KEY_RIGHT_CONTROL];
					io.KeyShift = io.KeysDown[AD_KEY_LEFT_SHIFT] || io.KeysDown[AD_KEY_RIGHT_SHIFT];
					io.KeyAlt = io.KeysDown[AD_KEY_LEFT_ALT] || io.KeysDown[AD_KEY_RIGHT_ALT];
					io.KeySuper = io.KeysDown[AD_KEY_LEFT_SUPER] || io.KeysDown[AD_KEY_RIGHT_SUPER];
				}
				break;
			}
			case GLFW_RELEASE:
			{
				input->UpdateKeyboardKey(key, 3);
				if (io.WantCaptureKeyboard) {
					io.KeysDown[key] = false;
				}
				break;
			}
			}
		}
	);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* glfwWindow, int width, int height)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			windowWidth = width;
			windowHeight = height;

			ImGuiIO& io = ImGui::GetIO();

			io.DisplaySize = ImVec2(windowWidth, windowHeight);
			io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		}
	);
	glfwSetCharCallback(window, [](GLFWwindow* glfwWindow, unsigned int keycode)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();

			if (io.WantCaptureKeyboard) {
				if (keycode > 0 && keycode < 0x10000) {
					io.AddInputCharacter((unsigned short)keycode);
				}
			}
		}
	);
	glfwSetScrollCallback(window, [](GLFWwindow* glfwWindow, double xOffset, double yOffset)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse) {
				io.MouseWheelH += xOffset;
				io.MouseWheel += yOffset;
			}
		}
	);
	glfwSetCursorEnterCallback(window, [](GLFWwindow* glfwWindow, int entered)
		{
			Input* input = (Input*)glfwGetWindowUserPointer(glfwWindow);

			input->SetMouseWasBlocked(true);
		}
	);





	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);









	


	Renderer* renderer = new LinearRenderer();

	std::vector<Object*> objects = std::vector<Object*>();
	objects.push_back(new Object("Resources/Cube.obj", "Resources/Shaders/PBR", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER, "Resources/Rust/Rust", MATERIAL_ALBEDO_TEXTURE | MATERIAL_NORMAL_TEXTURE | MATERIAL_METALLIC_TEXTURE | MATERIAL_ROUGHNESS_TEXTURE | MATERIAL_AO_TEXTURE, false));
	objects[0]->SetTranslation(glm::vec3(0.0f, -3.0f, 0.0f));
	objects[0]->SetScale(glm::vec3(50.0f, 1.0f, 50.0f));

	Camera* camera = new Camera();
	camera->SetSkybox("Resources/Skybox/Skybox");




	double timeConstant = 1.0;
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTimeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
		lastTime = currentTime;
		double deltaTimeWithoutTimeFactor = deltaTimeNanoseconds.count() / 1000000000.0;
		double deltaTime = deltaTimeWithoutTimeFactor * timeConstant;

		

		glfwPollEvents();
		input->Prepare();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		// update and draw

		for (int i = 0; i < objects.size(); i++) {
			objects[i]->GenerateFinalTransformation(glm::mat4(1.0f));
		}

		if (input->GetKeyboardKeyHeld(AD_KEY_W)) {
			camera->MoveForward(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_A)) {
			camera->StrafeLeft(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_S)) {
			camera->MoveBackward(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_D)) {
			camera->StrafeRight(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_LEFT_CONTROL)) {
			camera->MoveDown(deltaTime);
		}
		if (input->GetKeyboardKeyHeld(AD_KEY_SPACE)) {
			camera->MoveUp(deltaTime);
		}
			
		if (didMove) {
			camera->LookAtMouse(0.2f, input->GetMousePositionX(), input->GetMousePositionY(), input->GetOldMousePositionX(), input->GetOldMousePositionY());
		}





		for (int i = 0; i < objects.size(); i++) {
			renderer->AddObject(objects[i]);
		}
		renderer->Flush(windowWidth, windowHeight, camera);











		glfwSwapBuffers(window);

		input->Flush();
		Global::Update();
		didMove = false;
	}





















	ImGui_ImplOpenGL3_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	Global::Cleanup();
	*/
}