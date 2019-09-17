// OpenGL_03.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include<glut.h>
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glu.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

GLUquadricObj* quadric;													// The Quadric Object
void init()
{
	quadric = gluNewQuadric();											// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadric, GLU_SMOOTH);								// Create Smooth Normals 
	gluQuadricTexture(quadric, GL_TRUE);
}
void display()
{
	//画分割线 ，分成四个视图区域
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glViewport(0, 0, 400, 400);
	glBegin(GL_LINES);
	glVertex2f(-1.0, 0);
	glVertex2f(1.0, 0);
	glVertex2f(0, -1.0);
	glVertex2f(0, 1.0);
	glEnd();


	//定义在左上角的区域
	glColor3f(1.0, 0.0, 0.0);
	glViewport(0, 200, 200, 200);
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();
	//定义在右下角的区域
	glColor3f(1.0, 1.0, 1.0);
	glViewport(200, 0, 200, 200);
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();


	//定义左下角区域
	glColor3f(0.0, 1.0, 0.0);
	glViewport(0, 0, 200, 200);
	glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
	glLoadIdentity();											// Reset The Projection Matrix
	// Set Up Perspective Mode To Fit 1/4 The Screen (Size Of A Viewport)
	gluPerspective(45.0, 1, 0.1f, 500.0);
	glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -14.0f);								// Move 14 Units Into The Screen
	gluSphere(quadric, 4.0f, 32, 32);								// Draw A Sphere

	//定义右上角区域
	glViewport(200, 200, 200, 200);
	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();							// 重置投影矩阵
	gluPerspective(45.0f, 1, 0.1f, 100.0f);
	glColor3f(0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -7.0f);				// 移入屏幕
	glBegin(GL_QUADS);					// 开始绘制
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.8f, 0.0f);
	glVertex3f(1.0f, 0.8f, 0.0f);
	glEnd();
	
	glFlush();
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("glViewport");
	init();
	glutDisplayFunc(display);
	//0100;
	glutMainLoop();
	return 0;
}