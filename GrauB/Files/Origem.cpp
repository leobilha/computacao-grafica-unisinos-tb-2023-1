#define STB_IMAGE_IMPLEMENTATION

#include <Windows.h>
#include "stb_image.h"
#include <stdio.h>
#include <vector>

#include "../Headers/Includes.h"
#include "../Headers/Mesh.h"
#include "../Headers/Material.h"
#include "../Headers/ObjectPlanet.h"
#include "../Headers/Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processarEntradaTecladoObjeto(GLFWwindow* window);
void processarEntradaTecladoObjeto(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale, int idObjeto);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
vector<ObjectPlanet*> listaPlanetas;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camera
float camSpeed = 0.01f;
float dirSpeed = 0.01f;

glm::vec3	cameraPos, 
			cameraFront, 
			cameraUp;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;

float fov = 90.f;
float nearPlane = 0.1f;
float farPlane = 1000.f;

int planetSelecionado = 2;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GB - Computacao Grafica - Unisinos", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	glewInit();

	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//read shaders
	GLuint shaderProgram = LoadShader("Shaders/Phong.vs", "Shaders/Phong.fs");
	glUseProgram(shaderProgram);

	glm::mat4 ProjectionMatrix(1.f);
	glm::mat4 ModelMatrix(1.f);

	cameraPos = glm::vec3(0.0, 0.0, 3.0);
	cameraFront = glm::vec3(0.0, 0.0, -1.0); 
	cameraUp = glm::vec3(0.0, 1.0, 0.0);

	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

	GLint viewPosLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	ObjectPlanet* objTerra = new ObjectPlanet();
	objTerra->Inicializar();
	objTerra->position.x = 12.0f;
	Mesh* mesh = objTerra->processObj("bolaTerra.obj");
	objTerra->mesh = mesh;
	vector<Material*> materials = objTerra->getMat();
	objTerra->materials = materials;
	listaPlanetas.push_back(objTerra);

	ObjectPlanet* objSol = new ObjectPlanet();
	objSol->Inicializar();
	objSol->scale = glm::vec3(5.0, 5.0, 5.0);
	Mesh* mesh2 = objSol->processObj("bolaSol.obj");
	objSol->mesh = mesh2;
	vector<Material*> materials2 = objSol->getMat();
	objSol->materials = materials2;
	listaPlanetas.push_back(objSol);

	while (!glfwWindowShouldClose(window))
	{

		processarEntradaTecladoObjeto(window);
		processarEntradaTecladoObjeto(window, listaPlanetas[planetSelecionado - 1]->position, listaPlanetas[planetSelecionado - 1]->rotation, listaPlanetas[planetSelecionado - 1]->scale, planetSelecionado);

		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glm::mat4 view = glm::mat4(1);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewPosLoc, 1, FALSE, glm::value_ptr(view));
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		ProjectionMatrix = glm::mat4(1.0f);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		for (int i = 0; i < listaPlanetas.size(); i++)
		{
			listaPlanetas[i]->transform();
			GLuint texture;
			for (Group* g : listaPlanetas[i]->mesh->groups) {
				for (Material* m : listaPlanetas[i]->materials) {
					texture = m->texture;
					glUniform3f(glGetUniformLocation(shaderProgram, "Ka"), m->ka->r, m->ka->g, m->ka->b);
					glUniform3f(glGetUniformLocation(shaderProgram, "Kd"), m->kd->r, m->kd->g, m->kd->b);
					glUniform3f(glGetUniformLocation(shaderProgram, "Ks"), m->ks->r, m->ks->g, m->ks->b);
					glUniform1f(glGetUniformLocation(shaderProgram, "Ns"), m->ns);
				}

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);
				glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);

				glBindVertexArray(g->vao);

				glUniform1i((glGetUniformLocation(shaderProgram, "planetSelecionado")), planetSelecionado == i + 1);
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(listaPlanetas[i]->ModelMatrix));

				glDrawArrays(GL_TRIANGLES, 0, g->faces.size() * 6);
			}
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

//Método responsável por processar a entrada do teclado e atualizar as variáveis de posição, rotação e escala de um objeto específico, com base nas teclas pressionadas.
void processarEntradaTecladoObjeto(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

//Método responsável por processar a entrada do teclado e atualizar as variáveis de posição, rotação e escala de um objeto específico, com base nas teclas pressionadas.
void processarEntradaTecladoObjeto(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale, int idObjeto)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		planetSelecionado = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		planetSelecionado = 2;
	}

	if (planetSelecionado == idObjeto)
	{

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position.y += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position.y += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position.x -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position.y -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position.x += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			rotation.y -= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			rotation.y += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			rotation.x -= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			rotation.x += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
			rotation.z -= 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) {
			rotation.z += 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			scale += 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			scale -= 0.001f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += camSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= camSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * dirSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * dirSpeed;

	}

}

//Método acionado sempre que o tamanho do framebuffer da janela é alterado.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Método callback para a função mouse_callback do GLFW, que é acionado sempre que há um movimento do mouse registrado na janela do aplicativo.
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}