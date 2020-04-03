/*
* 微型太阳系:太阳、地球、月亮
*/

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

//鼠标键盘响应函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

float screen_width = 800.0f;          //窗口宽度
float screen_height = 600.0f;          //窗口高度
const GLfloat  PI = 3.14159265358979323846f;
//摄像机相关参数

Camera camera(glm::vec3(1.0f, 0.0f,6.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;


glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float skybox_vertices[] = {   //星空盒顶点数组
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
	// 初始化GLFW
	glfwInit();                                                     // 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL版本为3.3，主次版本号均设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式(无需向后兼容性)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 如果使用的是Mac OS X系统，需加上这行
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);						    // 不可改变窗口大小

																	// 创建窗口(宽、高、窗口名称)
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

	// 初始化GLAD，加载OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 指定当前视口尺寸(前两个参数为左下角位置，后两个参数是渲染窗口宽、高)
	glViewport(0, 0, screen_width, screen_height);
	//Shader shader("C:/src/res/shader/Model.vs", "C:/src/res/shader/Model.fs");//加载着色器
	//Model ourModel("C:/src/res/model/character.obj");

	//======================================================
	//======================================================
	//天空
	Shader skyShader("sky.vs", "sky.fs");
	//太阳
	Shader sunShader("sun.vs", "sun.fs");
	Model sunModel("C:/src/models/sun/13902_Earth_v1_l3.obj");
	//地球
	Shader earthShader("earth.vs", "earth.fs");
	Model earthModel("C:/src/models/earth/13902_Earth_v1_l3.obj");
	//月亮
	Shader moonShader("earth.vs", "earth.fs");
	Model moonModel("C:/src/models/moon/13902_Earth_v1_l3.obj");
	//======================================================
	//======================================================
	glEnable(GL_DEPTH_TEST);
	////////////////////////////////////////////////////////
	GLuint skyVAO, skyVBO;//绑定天空盒VAOVBO
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//-----------------------------------------------------
	std::vector<std::string> faces//加载天空盒纹理
	{
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg"),
		  ("data/sky/sky.jpg")
	
	};
	//加载天空盒纹理
	unsigned int skyTexture = Texture::load_cubemap(faces);

	//-----------------------------------------------------
	sunShader.Use();   // Transform坐标变换矩阵
	glm::mat4 model(1);//model矩阵，局部坐标变换至世界坐标
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
	sunShader.SetMat4("model", model);


	earthShader.Use();
	model = glm::mat4(1.0f);;//model矩阵，局部坐标变换至世界坐标
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0005f, 0.0005f, 0.0005f));
	earthShader.SetMat4("model", model);
	////////////////////////////////////////////////////////
	skyShader.Use();//使用SkyBoxShader着色器
	skyShader.SetInt("skybox", 0);//传入天空盒纹理
	////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////
	//旋转
	float zhuan = 0.0, mmz = 0.0;
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {

		//计算每帧的时间差
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清理颜色缓冲和深度缓冲

		glm::mat4 view(1);//view矩阵，世界坐标变换至观察坐标系
		view = camera.GetViewMatrix();
		glm::mat4 projection(1);//projection矩阵，投影矩阵
		projection = glm::perspective(glm::radians(camera.Zoom), screen_width / screen_height, 0.1f, 100.0f);

		sunShader.Use();
		
		// 向着色器中传入参数		
		sunShader.SetMat4("view", view);
		sunShader.SetMat4("projection", projection);
		//sunShader.SetVec3("lightPos", lightPos);
		//sunShader.SetVec3("viewPos", camera.Position);
		
		//绘制
		sunModel.Draw(sunShader);


		earthShader.Use();
		earthShader.SetMat4("view", view);
		earthShader.SetMat4("projection", projection);
		earthShader.SetVec3("lightPos", lightPos);
		earthShader.SetVec3("viewPos", camera.Position);
		//做出地球绕日运动
		float R, x, y, z;
		R = 3.0;//半径为4.0

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

		//月亮
		moonShader.Use();
		moonShader.SetMat4("view", view);
		moonShader.SetMat4("projection", projection);
		moonShader.SetVec3("lightPos", lightPos);
		moonShader.SetVec3("viewPos", camera.Position);

		//月亮移位
		float r, mx, my, mz;
		r = 0.7;//半径为4.0
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

		glDepthFunc(GL_LEQUAL);//深度测试  输入的深度值小于或等于参考值，则通过
		glDepthMask(GL_FALSE);//禁止向深度缓冲区写入数据
		skyShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyShader.SetMat4("view", view);
		skyShader.SetMat4("projection", projection);

		glBindVertexArray(skyVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);//绑定天空盒纹理
		glDrawArrays(GL_TRIANGLES, 0, 36);//绘制天空盒
		glDepthMask(GL_TRUE);//允许向深度缓冲区写入数据
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);//深度测试  输入的深度值小于参考值，则通过
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &skyVAO);
	glDeleteBuffers(1, &skyVBO);
	glDeleteTextures(1, &skyTexture);
	// 清理所有的资源并正确退出程序
	glfwTerminate();
	return 0;
}

//键盘输入
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

//窗口分辨率自适应
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//鼠标移动响应
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
//鼠标滚轮响应
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




