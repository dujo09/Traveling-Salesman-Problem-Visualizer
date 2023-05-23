#include "Shader.h"
#include "TravelingSalesmanSolver.h"
#include "Point2D.h"
#include "SolverColors.h"

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

struct LineVertex
{
	glm::vec2 position;
	glm::vec3 color;
};

struct PointVertex
{
	glm::vec2 position;
	glm::vec3 color;
	glm::vec2 localPosition; // position local to the object rendered
};



static void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static PointVertex* createRectangle(PointVertex* target, float lowerLeftX, float lowerLeftY, float width, float height, glm::vec3 color);
static LineVertex* createLine(LineVertex* target, float xStart, float yStart, int xEnd, int yEnd, glm::vec3 color);
static float mapNumberToRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd);

const float INITIAL_SCREEN_WIDTH = 1280.0f;
const float INITIAL_SCREEN_HEIGHT = 720.0f;

const unsigned int MAX_POINT_COUNT = 1000;

const float INITIAL_POINT_RADIUS = 15.0f;
const float MAX_POINT_RADIUS = 30.0f;
const float MIN_POINT_RADIUS = 1.0f;

const float INITIAL_LINE_WIDTH = 10.0f;
const float MIN_LINE_WIDTH = 1.0f;


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
	const unsigned int MAX_LINES_VERTEX_COUNT = MAX_POINT_COUNT * 2;

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

	unsigned int pointsVAO, pointsVBO, pointsEBO;

	glGenVertexArrays(1, &pointsVAO);
	glGenBuffers(1, &pointsVBO);
	glGenBuffers(1, &pointsEBO);

	glBindVertexArray(pointsVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_POINTS_VERTEX_COUNT * sizeof(PointVertex), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pointsIndices), pointsIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (const void*)offsetof(PointVertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (const void*)offsetof(PointVertex, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (const void*)offsetof(PointVertex, localPosition));
	glEnableVertexAttribArray(2);


	unsigned int linesVAO, linesVBO;	

	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);

	glBindVertexArray(linesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_LINES_VERTEX_COUNT * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, color));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader pointShader("shaders/PointShader.vert", "shaders/PointShader.frag");
	Shader lineShader("shaders/LineShader.vert", "shaders/LineShader.geo", "shaders/LineShader.frag");

	TravelingSalesmanSolver solver(10, 0, 10000, 0, 10000);

	float pointRadius = INITIAL_POINT_RADIUS;
	float lineWidth = INITIAL_LINE_WIDTH;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int displayWidth, displayHeight;
		glfwGetFramebufferSize(window, &displayWidth, &displayHeight);


		unsigned int pointIndexCount = 0;

		{
			const std::vector<Point2D>& points = solver.getPoints();

			std::array<PointVertex, MAX_POINTS_VERTEX_COUNT> pointVertices;
			PointVertex* buffer = pointVertices.data();

			for (const Point2D& point : points)
			{
				const float pointCenterXOnScreen = mapNumberToRange(point.getX(),
					solver.getXMin(), solver.getXMax(), pointRadius, displayWidth - pointRadius);
				const float pointCenterYOnScreen = mapNumberToRange(point.getY(),
					solver.getYMin(), solver.getYMax(), pointRadius, displayHeight - pointRadius);

				const float pointLowerLeftXOnScreen = pointCenterXOnScreen - pointRadius;
				const float pointLowerLeftYOnScreen = pointCenterYOnScreen - pointRadius;

				buffer = createRectangle(buffer, pointLowerLeftXOnScreen, pointLowerLeftYOnScreen,
					pointRadius * 2, pointRadius * 2, point.getColor());
				pointIndexCount += 6;
			}

			glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, pointVertices.size() * sizeof(PointVertex), pointVertices.data());
		}


		unsigned int routeVertexCount = 0;

		{
			const std::vector<Point2D*>& route = solver.getRoute();
			int routeLength = route.size();

			std::array<LineVertex, MAX_LINES_VERTEX_COUNT> routeVertices;
			LineVertex* buffer = routeVertices.data();

			for (int i = 0; i < routeLength - 1; ++i)
			{
				const int lineXStartOnScreen = mapNumberToRange(route.at(i)->getX(),
					solver.getXMin(), solver.getXMax(), pointRadius, displayWidth - pointRadius);
				const int lineYStartOnScreen = mapNumberToRange(route.at(i)->getY(),
					solver.getYMin(), solver.getYMax(), pointRadius, displayHeight - pointRadius);

				const int lineXEndOnScreen = mapNumberToRange(route.at(i + 1)->getX(),
					solver.getXMin(), solver.getXMax(), pointRadius, displayWidth - pointRadius);
				const int lineYEndOnScreen = mapNumberToRange(route.at(i + 1)->getY(),
					solver.getYMin(), solver.getYMax(), pointRadius, displayHeight - pointRadius);

				buffer = createLine(buffer, lineXStartOnScreen, lineYStartOnScreen, 
					lineXEndOnScreen, lineYEndOnScreen, route.at(i)->getOutgoingLineColor());
				routeVertexCount += 2;
			}

			glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, routeVertices.size() * sizeof(LineVertex), routeVertices.data());
		}

		glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)displayWidth, 0.0f, (float)displayHeight, -1.0f, 1.0f);

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
			if (ImGui::Button("Regenerate points"))
			{
				solver.generatePoints(numberOfPoints);
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop solving"))
			{
				solver.interruptSolving();
			}

			ImGui::NewLine();

			if (ImGui::SliderInt("Time step (milliseconds)", &timeStepMilliseconds, 0.0f, TravelingSalesmanSolver::MAX_TIME_STEP_MILLISECONDS))
			{
				solver.setTimeStep(timeStepMilliseconds);
			}

			if (ImGui::InputInt("Number of points", &numberOfPoints))
			{
				if (numberOfPoints > 0 && numberOfPoints < MAX_POINT_COUNT)
				{
					solver.generatePoints(numberOfPoints);
				}
				
			}


			ImGui::NewLine();

			if (ImGui::SliderFloat("Point radius", &pointRadius, MIN_POINT_RADIUS, MAX_POINT_RADIUS, "%.1f"))
			{
				lineWidth = std::min(lineWidth, pointRadius * 2.0f);
			}
			ImGui::SliderFloat("Line width", &lineWidth, MIN_LINE_WIDTH, pointRadius * 2.0f, "%.1f");

			ImGui::NewLine();

			if (ImGui::RadioButton("Random algorithm", &selectedAlgorithmIndex, SolvingAlgorithm::RANDOM) ||
				ImGui::RadioButton("Greedy algorithm", &selectedAlgorithmIndex, SolvingAlgorithm::GREEDY) ||
				ImGui::RadioButton("2-Opt algorithm", &selectedAlgorithmIndex, SolvingAlgorithm::TWO_OPT))
			{
				solver.setSolvingAlgorithm(SolvingAlgorithm(selectedAlgorithmIndex));
			}

			ImGui::NewLine();

			ImGui::Text("Route length: %.3f", solver.getRouteLength());

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}
		
		ImGui::Render();
		glViewport(0, 0, displayWidth, displayHeight);

		glClearColor(SolverColors::BACKGROUND_COLOR.x, SolverColors::BACKGROUND_COLOR.y, SolverColors::BACKGROUND_COLOR.z, 0.0f);

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
		lineShader.setVec2("viewportSize", (float)displayWidth, (float)displayHeight);
		lineShader.setFloat("lineWidth", lineWidth);

		glBindVertexArray(linesVAO);
		glDrawArrays(GL_LINES, 0, routeVertexCount);


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

void glfw_error_callback(int error, const char* description)
{
	std::cout << "GLFW Error " << error << ": " << description << "\n";
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static PointVertex* createRectangle(PointVertex* target, float lowerLeftX, float lowerLeftY, float width, float height, glm::vec3 color)
{
	// Lower left
	target->position = glm::vec2(lowerLeftX, lowerLeftY);
	target->color = color;
	target->localPosition = glm::vec2(-0.5f, -0.5f);
	++target;

	// Lower right
	target->position = glm::vec2(lowerLeftX + width, lowerLeftY);
	target->color = color;
	target->localPosition = glm::vec2(0.5f, -0.5f);
	++target;

	// Upper right
	target->position = glm::vec2(lowerLeftX + width, lowerLeftY + height);
	target->color = color;
	target->localPosition = glm::vec2(0.5f, 0.5f);
	++target;

	// Upper left
	target->position = glm::vec2(lowerLeftX, lowerLeftY + height);
	target->color = color;
	target->localPosition = glm::vec2(-0.5f, 0.5f);
	++target;

	return target;
}

static LineVertex* createLine(LineVertex* target, float xStart, float yStart, int xEnd, int yEnd, glm::vec3 color)
{
	target->position = glm::vec2(xStart, yStart);
	target->color = color;
	++target;

	target->position = glm::vec2(xEnd, yEnd);
	target->color = color;
	++target;

	return target;
}

static float mapNumberToRange(float input, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	// Formula: https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
	float slope = (outputEnd - outputStart) / (inputEnd - inputStart);
	return outputStart + slope * (input - inputStart);
}