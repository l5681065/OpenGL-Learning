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


const char * pVS = "shader.vert";
const char * pFS = "shader.frag";

static void RenderSceneCB();

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	
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
	cout << vs << endl;
	cout << fs << endl;


} 

int main(int argc,char**  agrv)
{
	glutInit(&argc,agrv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL-01");
	
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	GLenum res = glewInit();
	if (GLEW_OK!=res)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}
	
	glClearColor(0.0f,1.0f,0.0f,0.0f);
	CompileShader();
	glutMainLoop();
    std::cout << "Hello World!\n"; 
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
