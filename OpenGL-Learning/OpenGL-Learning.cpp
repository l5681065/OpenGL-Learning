// OpenGL-Learning.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include <stdio.h>
#include <glew.h>
#include <freeglut.h>
#include <iostream>
using namespace std;
#include "common.h"
#include "Utils.h"
#include "math3d.h"


 
GLuint VBO;

const char * pVS = "shader.vert";
const char * pFS = "shader.frag";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);

	//用这个VBO 渲染 1.glBindBuffer 2.数据格式解析 3.渲染
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	
	//数据格式解析
	//index 与上面index保持一致   size 我们要解析的数据有几个元素构成（之前传递的一个顶点 由三个浮点数构成） type 浮点数 normalized  是否要进行归一化 
	//如果我们要传递的顶点数据是法向量 且不确定法向量是否进行了归一化 我们会设置为GL_True  因为在光照计算时  法向量为归一化 光照效果是不正确的
	//stride 两个数据之间需要跨越多少个Byte 我们这里 一个顶点 由三个浮点数构成 一个浮点数占4位  因此3*4
	//pointer   代表 偏移 offset
	//假如在一个数组中传递多种数据  一个顶点不只只是坐标 还有颜色 法线   比如说这个顶点数据中还包括法线 point 要解析的是法线数据 比如说是3个坐标3个法线值 
	//那么glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*4,3*4); 因为法线坐标的起始量 要跳过三个顶点值  所以就是3*4
	//因为这个我们就只有顶点数据 所以就填0
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
	//渲染指令 
	//mode 已什么样的拓扑关系绘制三角形 frist 从第几个开始 count 绘制几个顶点数据
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();

	
}

static void CreateVertexBuffer()
{
	Vector3f Vertices[3];
	Vertices[0] = Vector3f(-1.0f,-1.0f,0.0f);
	Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);
	//创建一个VBO
	glGenBuffers(1,&VBO); 
	//绑定缓冲区
	glBindBuffer(GL_ARRAY_BUFFER,VBO); 
	//传递给服务端 usage 渲染类型
	glBufferData (GL_ARRAY_BUFFER,sizeof(Vertices),Vertices,GL_STATIC_DRAW);

}
static void AddShader(GLuint ShaderProgram,const char * shaderText,GLenum shaderType)
{
	//创建一个shader对象
	GLuint shaderObj= glCreateShader(shaderType);
	if (shaderObj==0)
	{
		ErrorOut();
		exit(1);
	}
	const GLchar* p[1];
	p[0] = shaderText;
	GLint L[1];
	L[0] = strlen(shaderText);

	glShaderSource(shaderObj,1,p,L);

	glCompileShader(shaderObj);
	 
	GLint Success;
	glGetShaderiv(shaderObj,GL_COMPILE_STATUS,&Success);
	if (!Success)
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObj,1024,NULL,InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n ", shaderType, InfoLog); 
		exit(1);
	}
	glAttachShader(ShaderProgram,shaderObj);

	   
}

void CompileShader()
{
	string vs, fs;
	if (!ReadFile(pVS, vs))
	{
		exit(1);
	}
	if (!ReadFile(pFS, fs))
	{
		exit(1);
	}
	
	//通过OpenGL状态机分配Program 可以理解为句柄  如果多次调用glCreateProgram可以保证每次的ShaderProgram是不相同的  也像向操作系统请求内存保证每次返回的都是唯一的
	GLuint ShaderProgram = glCreateProgram();
	if (ShaderProgram==0)
	{
		ErrorOut();
		exit(1);
	}
	//绑定program
	AddShader(ShaderProgram,vs.c_str(),GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	//链接
	glLinkProgram(ShaderProgram);

	GLint Success;
	glGetShaderiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		GLchar InfoLog[1024];
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, InfoLog);
		fprintf(stderr, "Error linking shader program '%s' \n", InfoLog);
		exit(1);
	}

	// 链接
	glGetShaderiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		GLchar InfoLog[1024];
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, InfoLog);
		fprintf(stderr, "Error linking shader program '%s' \n", InfoLog);
		exit(1);
	}

	//生效
	glValidateProgram(ShaderProgram);
	glGetShaderiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		GLchar InfoLog[1024];
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, InfoLog);
		fprintf(stderr, "Error validate shader program '%s' \n", InfoLog);
		exit(1);
	}
	glUseProgram(ShaderProgram);
 
} 

static void InitializeGlutCallBack()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

int main(int argc, char**  agrv)
{
	glutInit(&argc,agrv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(1024	,768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL-01");
	
	InitializeGlutCallBack();

	GLenum res = glewInit();
	if (GLEW_OK!=res)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f,1.0f,0.0f,0.0f);
	CreateVertexBuffer();
	CompileShader();
	glutMainLoop();
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
