#pragma once

#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <sstream>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Include this header for glm::value_ptr
#include <glm/gtx/string_cast.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Camera.h"
#include "Plane.h"
#include "Light.h"
#include "sphere_scene.cpp"

using namespace glm;

struct Vector3
{
	float x, y, z;
};

struct Triangle
{
	unsigned int indices[3];
};

extern std::vector<vec3> gPositions;
extern std::vector<vec3> gNormals;
extern std::vector<unsigned int> gTriangles;

void load_mesh(std::string fileName); // �Լ� ����


extern float gTotalTimeElapsed;
extern int 	gTotalFrames;
void init_timer(); // Ÿ�̸� �ʱ�ȭ �Լ� ����
void start_timing(); // Ÿ�̸� ���� �Լ� ����
float stop_timing(); // Ÿ�̸� ���� �Լ� ����
void display();

// -------------------------------------------------
// Global Variables
// -------------------------------------------------
int Width = 1280;
int Height = 1280;
std::vector<float> OutputImage;
GLuint shaderProgram;
GLuint VAO;
int indexSize;
// -------------------------------------------------

GLuint LoadShaders(const string& vertex_file_path, const string& fragment_file_path) {
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path.c_str(), ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossile to open %s", vertex_file_path.c_str());
		getchar();
		return 0;
	}

	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path.c_str(), ios::in);
	if (FragmentShaderStream.is_open()) {
		stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", vertex_file_path.c_str());
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling shader : %s\n", fragment_file_path.c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Update_mesh(const GLuint& VAO, const vector<GLuint>& GLBuffers, vector<vec3>& Positions,
	vector<vec3>& Normals, vector<vec3>& Colors, vector<unsigned int>& Indices) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(Positions[0]), &Positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Colors[0]), &Colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffers[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices[0]), &Indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//helped by ChatGPT
void setUniformVec3(GLuint program, const std::string& name, const glm::vec3& value) {
	GLint loc = glGetUniformLocation(program, name.c_str());
	glUniform3fv(loc, 1, glm::value_ptr(value));
}
//helped by ChatGPT
void setUniformFloat(GLuint program, const std::string& name, float value) {
	GLint loc = glGetUniformLocation(program, name.c_str());
	glUniform1f(loc, value);
}
//helped by ChatGPT
void setUniformMat4(GLuint program, const std::string& name, const glm::mat4& mat) {
	GLint loc = glGetUniformLocation(program, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

//helped by ChatGPT
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);  // back-face culling OFF
	glEnable(GL_NORMALIZE);

	glUseProgram(shaderProgram);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, -1.0f, -1.5f));
	model = glm::scale(model, glm::vec3(10.0f));

	glm::vec3 eye = glm::vec3(0, 0, 0);
	glm::vec3 center = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::mat4 view = glm::lookAt(eye, center, up);

	glm::mat4 projection = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

	glm::mat4 mvp = projection * view * model;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

	// Set uniforms
	// 재질 세팅
	setUniformVec3(shaderProgram, "ka", glm::vec3(1.0f));
	setUniformVec3(shaderProgram, "kd", glm::vec3(1.0f));
	setUniformVec3(shaderProgram, "ks", glm::vec3(0.0f));
	setUniformFloat(shaderProgram, "shininess", 0.0f);

	// 조명 세팅
	setUniformVec3(shaderProgram, "Ia", glm::vec3(0.2f));             // 전역 ambient
	setUniformVec3(shaderProgram, "lightAmbient", glm::vec3(0.0f));  // directional light ambient
	setUniformVec3(shaderProgram, "lightDiffuse", glm::vec3(1.0f));  // directional light diffuse
	setUniformVec3(shaderProgram, "lightSpecular", glm::vec3(0.0f)); // directional light specular

	// 방향성 조명 벡터 (정규화 필요)
	glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	setUniformVec3(shaderProgram, "lightDir", lightDirection);

	// 카메라 위치 (필요하지만 specular 없어서 사실상 영향 없음)
	setUniformVec3(shaderProgram, "viewPos", glm::vec3(0.0f, 0.0f, 0.0f));

	setUniformMat4(shaderProgram, "model", model);
	setUniformMat4(shaderProgram, "view", view);
	setUniformMat4(shaderProgram, "projection", projection);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexSize), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderFunc() {
	render();
}

void reshape(int w, int h)
{
	glutPostRedisplay();

}

void resize_callback(GLFWwindow* window, int nw, int nh)
{
	Width = nw;
	Height = nh;
	glViewport(0, 0, Width, Height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Q2");

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n";
		return -1;
	}


	shaderProgram = LoadShaders("../Phong.vert", "../Phong.frag");
	load_mesh("../bunny.obj");

	glGenVertexArrays(1, &VAO);
	vector<GLuint> GLBuffers(4, 0);
	glGenBuffers(4, &GLBuffers[0]);
	vector<vec3> gColorBuffer(gPositions.size(), vec3(1, 1, 1));
	Update_mesh(VAO, GLBuffers, gPositions, gNormals, gColorBuffer, gTriangles);
	indexSize = gTriangles.size();

	init_timer();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}