/*
* ΢��̫��ϵ:̫������������
*/

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

//��������Ӧ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

float screen_width = 800.0f;          //���ڿ��
float screen_height = 600.0f;          //���ڸ߶�
const GLfloat  PI = 3.14159265358979323846f;
//�������ز���

Camera camera(glm::vec3(1.0f, 0.0f,6.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;


glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float skybox_vertices[] = {   //�ǿպж�������
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f,  1.0f
};

int main() {
	// ��ʼ��GLFW
	glfwInit();                                                     // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL�汾Ϊ3.3�����ΰ汾�ž���Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ(������������)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // ���ʹ�õ���Mac OS Xϵͳ�����������
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);						    // ���ɸı䴰�ڴ�С

																	// ��������(���ߡ���������)
	auto window = glfwCreateWindow(screen_width, screen_height, "model", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to Create OpenGL Context" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ��ʼ��GLAD������OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ָ����ǰ�ӿڳߴ�(ǰ��������Ϊ���½�λ�ã���������������Ⱦ���ڿ���)
	glViewport(0, 0, screen_width, screen_height);
	//Shader shader("C:/src/res/shader/Model.vs", "C:/src/res/shader/Model.fs");//������ɫ��
	//Model ourModel("C:/src/res/model/character.obj");

	//======================================================
	//======================================================
	//���
	Shader skyShader("sky.vs", "sky.fs");
	//̫��
	Shader sunShader("sun.vs", "sun.fs");
	Model sunModel("C:/src/models/sun/13902_Earth_v1_l3.obj");
	//����
	Shader earthShader("earth.vs", "earth.fs");
	Model earthModel("C:/src/models/earth/13902_Earth_v1_l3.obj");
	//����
	Shader moonShader("earth.vs", "earth.fs");
	Model moonModel("C:/src/models/moon/13902_Earth_v1_l3.obj");
	//======================================================
	//======================================================
	glEnable(GL_DEPTH_TEST);
	////////////////////////////////////////////////////////
	GLuint skyVAO, skyVBO;//����պ�VAOVBO
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//-----------------------------------------------------
	std::vector<std::string> faces//������պ�����
	{
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg")
	
	};
	//������պ�����
	unsigned int skyTexture = Texture::load_cubemap(faces);

	//-----------------------------------------------------
	sunShader.Use();   // Transform����任����
	glm::mat4 model(1);//model���󣬾ֲ�����任����������
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
	sunShader.SetMat4("model", model);


	earthShader.Use();
	model = glm::mat4(1.0f);;//model���󣬾ֲ�����任����������
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0005f, 0.0005f, 0.0005f));
	earthShader.SetMat4("model", model);
	////////////////////////////////////////////////////////
	skyShader.Use();//ʹ��SkyBoxShader��ɫ��
	skyShader.SetInt("skybox", 0);//������պ�����
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	//��ת
	float zhuan = 0.0, mmz = 0.0;
	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {

		//����ÿ֡��ʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������ɫ�������Ȼ���

		glm::mat4 view(1);//view������������任���۲�����ϵ
		view = camera.GetViewMatrix();
		glm::mat4 projection(1);//projection����ͶӰ����
		projection = glm::perspective(glm::radians(camera.Zoom), screen_width / screen_height, 0.1f, 100.0f);

		sunShader.Use();
		
		// ����ɫ���д������		
		sunShader.SetMat4("view", view);
		sunShader.SetMat4("projection", projection);
		//sunShader.SetVec3("lightPos", lightPos);
		//sunShader.SetVec3("viewPos", camera.Position);
		
		//����
		sunModel.Draw(sunShader);


		earthShader.Use();
		earthShader.SetMat4("view", view);
		earthShader.SetMat4("projection", projection);
		earthShader.SetVec3("lightPos", lightPos);
		earthShader.SetVec3("viewPos", camera.Position);
		//�������������˶�
		float R, x, y, z;
		R = 3.0;//�뾶Ϊ4.0

		x = R * cos(zhuan*PI);
		y = 0.0;
		z = R * sin(zhuan*PI);
		zhuan += 0.0005;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x,y,z));
		model = glm::rotate(model, glm::radians((R-x)*20.0f), glm::vec3(1.0, 1.0, 1.0));
		model = glm::scale(model, glm::vec3(0.0005)); // a smaller cube
		earthShader.SetMat4("model", model);		
		earthModel.Draw(earthShader);

		//����
		moonShader.Use();
		moonShader.SetMat4("view", view);
		moonShader.SetMat4("projection", projection);
		moonShader.SetVec3("lightPos", lightPos);
		moonShader.SetVec3("viewPos", camera.Position);

		//������λ
		float r, mx, my, mz;
		r = 0.7;//�뾶Ϊ4.0
		mmz += 0.005;
		mx = r * cos(mmz*PI);
		my = 0.0;
		mz = r * sin(mmz*PI);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(mx+x, my+y, mz+z));
		//model = glm::translate(model, glm::vec3(x*0.25, y*0.25, z*0.25));

		model = glm::rotate(model, glm::radians((R - x)*10.0f), glm::vec3(0.0, 1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.0002)); // a smaller cube
		moonShader.SetMat4("model", model);
		moonModel.Draw(moonShader);

		glDepthFunc(GL_LEQUAL);//��Ȳ���  ��������ֵС�ڻ���ڲο�ֵ����ͨ��
		glDepthMask(GL_FALSE);//��ֹ����Ȼ�����д������
		skyShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyShader.SetMat4("view", view);
		skyShader.SetMat4("projection", projection);

		glBindVertexArray(skyVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);//����պ�����
		glDrawArrays(GL_TRIANGLES, 0, 36);//������պ�
		glDepthMask(GL_TRUE);//��������Ȼ�����д������
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);//��Ȳ���  ��������ֵС�ڲο�ֵ����ͨ��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &skyVAO);
	glDeleteBuffers(1, &skyVBO);
	glDeleteTextures(1, &skyTexture);
	// �������е���Դ����ȷ�˳�����
	glfwTerminate();
	return 0;
}

//��������
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

//���ڷֱ�������Ӧ
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//����ƶ���Ӧ
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}
//��������Ӧ
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




