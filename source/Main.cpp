#include "Shader.h"
#include "TravelingSalesmanSolver.h"
#include "Point2D.h"

#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;
};



void processInput(GLFWwindow*& window, TravelingSalesmanSolver& solver);
static void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static Vertex* createRectangle(Vertex* target, float lowerLeftX, float lowerLeftY, float width, float height, glm::vec3 color);
static Vertex* createPointOnRoute(Vertex* target, float x, float y, glm::vec3 color);
static float mapNumberToRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd);

const float INITIAL_SCREEN_WIDTH = 1280.0f;
const float INITIAL_SCREEN_HEIGHT = 720.0f;

const unsigned int MAX_POINT_COUNT = 1000;

const float POINT_RADIUS = 20.0f;
const float PADDING = 0.0f;

int main()
{
	// GLFW setup
	// ----------
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Traveling Salesman Problem Visualizer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// ImGUI setup
	// -----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// glad setup
	// -----------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD function\n";
		glfwTerminate();
		return -1;
	}

	const unsigned int MAX_POINTS_VERTEX_COUNT = MAX_POINT_COUNT * 4;
	const unsigned int MAX_POINTS_INDEX_COUNT = MAX_POINT_COUNT * 6;
	const unsigned int MAX_LINES_VERTEX_COUNT = MAX_POINT_COUNT + 1;
	const unsigned int MAX_LINES_INDEX_COUNT = MAX_POINT_COUNT * 2;

	unsigned int pointsIndices[MAX_POINTS_INDEX_COUNT];

	int offset = 0;
	for (int i = 0; i < MAX_POINTS_INDEX_COUNT; i += 6)
	{
		pointsIndices[i + 0] = 0 + offset;
		pointsIndices[i + 1] = 1 + offset;
		pointsIndices[i + 2] = 2 + offset;

		pointsIndices[i + 3] = 2 + offset;
		pointsIndices[i + 4] = 3 + offset;
		pointsIndices[i + 5] = 0 + offset;

		offset += 4;
	}


	unsigned int routeIndices[MAX_LINES_INDEX_COUNT];

	offset = 0;
	for (int i = 0; i < MAX_LINES_INDEX_COUNT; i += 2)
	{
		routeIndices[i + 0] = 0 + offset;
		routeIndices[i + 1] = 1 + offset;

		offset += 1;
	}

	unsigned int pointsVAO, pointsVBO, pointsEBO;

	glGenVertexArrays(1, &pointsVAO);
	glGenBuffers(1, &pointsVBO);
	glGenBuffers(1, &pointsEBO);

	glBindVertexArray(pointsVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_POINTS_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pointsIndices), pointsIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);


	unsigned int linesVAO, linesVBO, linesEBO;	

	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);
	glGenBuffers(1, &linesEBO);

	glBindVertexArray(linesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_LINES_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(routeIndices), routeIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader pointShader("shaders/DefaultShader.vert", "shaders/PointShader.frag");
	Shader lineShader("shaders/DefaultShader.vert", "shaders/LineShader.frag");

	TravelingSalesmanSolver solver(10, 0, 10000, 0, 10000);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window, solver);

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);


		unsigned int pointIndexCount = 0;

		{
			const std::vector<Point2D>& points = solver.getPoints();

			std::array<Vertex, MAX_POINTS_VERTEX_COUNT> pointVertices;
			Vertex* buffer = pointVertices.data();

			for (const Point2D& point : points)
			{
				const float pointLowerLeftXOnScreen = mapNumberToRange(point.getX() - POINT_RADIUS,
					solver.getXMin(), solver.getXMax(), 0 + PADDING, screenWidth - PADDING);
				const float pointLowerLeftYOnScreen = mapNumberToRange(point.getY() - POINT_RADIUS,
					solver.getYMin(), solver.getYMax(), 0 + PADDING, screenHeight - PADDING);

				buffer = createRectangle(buffer, pointLowerLeftXOnScreen, pointLowerLeftYOnScreen, 
					POINT_RADIUS * 2, POINT_RADIUS * 2, point.getColor());
				pointIndexCount += 6;
			}

			glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, pointVertices.size() * sizeof(Vertex), pointVertices.data());
		}


		unsigned int lineIndexCount = 0;

		{
			const std::vector<Point2D*>& route = solver.getRoute();

			std::array<Vertex, MAX_LINES_VERTEX_COUNT> routeVertices;
			Vertex* buffer = routeVertices.data();

			for (const Point2D* point : route)
			{
				const int pointCenterXOnScreen = mapNumberToRange(point->getX(),
					solver.getXMin(), solver.getXMax(), 0 + PADDING, screenWidth - PADDING);
				const int pointCenterYOnScreen = mapNumberToRange(point->getY(),
					solver.getYMin(), solver.getYMax(), 0 + PADDING, screenHeight - PADDING);

				buffer = createPointOnRoute(buffer, pointCenterXOnScreen, pointCenterYOnScreen, point->getColor());
				lineIndexCount += 2;
			}
			lineIndexCount -= 2;

			glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, routeVertices.size() * sizeof(Vertex), routeVertices.data());
		}

		glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			int timeStepMilliseconds = solver.getTimeStep();
			int numberOfPoints = solver.getNumberOfPoints();
			int selectedAlgorithmIndex = solver.getSelectedAlgorithm();

			ImGui::Begin("Control window");

			if (ImGui::Button("Start solving"))
			{
				solver.startSolving();
			}
			ImGui::SameLine();
			if (ImGui::Button("Interrupt solving"))
			{
				solver.interruptSolving();
			}

			ImGui::Text("Configure solver");
			if (ImGui::SliderInt("Time step (milliseconds)", &timeStepMilliseconds, 0.0f, TravelingSalesmanSolver::MAX_TIME_STEP_MILLISECONDS))
			{
				solver.setTimeStep(timeStepMilliseconds);
			}

			if (ImGui::SliderInt("Number of points", &numberOfPoints, 3, MAX_POINT_COUNT))
			{
				if (solver.isSolving())
				{
					break;
				}
				solver.generatePoints(numberOfPoints);
			}

			ImGui::Text("Choose algorithm");
			if (ImGui::RadioButton("Random algorithm", &selectedAlgorithmIndex, SolvingAlgorithm::RANDOM) ||
				ImGui::RadioButton("Greedy algorithm", &selectedAlgorithmIndex, SolvingAlgorithm::GREEDY) ||
				ImGui::RadioButton("2-Opt algorithm", &selectedAlgorithmIndex, SolvingAlgorithm::TWO_OPT))
			{
				if (solver.isSolving())
				{
					break;
				}
				solver.setSolvingAlgorithm(SolvingAlgorithm(selectedAlgorithmIndex));
			}

			ImGui::Text("Results");
			ImGui::Text("Route length: %d", solver.getRouteLength());

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}
		
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		

		lineShader.use();
		lineShader.setMat4("projectionMatrix", projectionMatrix);

		glBindVertexArray(linesVAO);
		glDrawElements(GL_LINES, lineIndexCount, GL_UNSIGNED_INT, nullptr);


		pointShader.use();
		pointShader.setMat4("projectionMatrix", projectionMatrix);

		glBindVertexArray(pointsVAO);
		glDrawElements(GL_TRIANGLES, pointIndexCount, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow*& window, TravelingSalesmanSolver& solver)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		solver.interruptSolving();
	}
}

void glfw_error_callback(int error, const char* description)
{
	std::cout << "GLFW Error " << error << ": " << description << "\n";
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static Vertex* createRectangle(Vertex* target, float lowerLeftX, float lowerLeftY, float width, float height, glm::vec3 color)
{
	target->position = glm::vec2(lowerLeftX, lowerLeftY);
	target->color = color;
	++target;

	target->position = glm::vec2(lowerLeftX + width, lowerLeftY);
	target->color = color;
	++target;

	target->position = glm::vec2(lowerLeftX + width, lowerLeftY + height);
	target->color = color;
	++target;

	target->position = glm::vec2(lowerLeftX, lowerLeftY + height);
	target->color = color;
	++target;

	return target;
}

static Vertex* createPointOnRoute(Vertex* target, float x, float y, glm::vec3 color)
{
	target->position = glm::vec2(x, y);
	target->color = color;
	++target;

	return target;
}

static float mapNumberToRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	// https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
	float slope = (outputEnd - outputStart) / (inputEnd - inputStart);
	return outputStart + slope * (input - inputStart);
}