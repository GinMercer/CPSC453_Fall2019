//CPSC453 Assignment01
//Traditioanl OpenGL
//Sijia Yin
//30049836
//2019-10-03
//Note: As a new c++ user, a new opengl learner, a new English speaker. This a assignment is super hard to do. There is some websites I learn a lot from
//		and i will show as followed.
//		https://www.cnblogs.com/cs-lcy/p4590930.html draw a square
//		https://blog.csdn.net/ynsenyu/article/details/39423493 the input
//		https://learnopengl-cn.github.io create window
//		https://baike.baidu.com/item/glClearColor/9516326?fr=aladdin glClearColor
//		https://baike.baidu.com/item/glew/5495893?fr=aladdin glfw
//		https://www.jianshu.com/p/4672f38f6c1e context
//		https://blog.csdn.net/zjz520yy/article/details/83000081 glfw


#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* window;
GLint width,height;
GLfloat ratio=1.0f;

//Decide the size of the window
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;

//********************************************************DRAW THE CUBE*********************************************************************
void Cube(float x, float y, float z, float side)
{
	//front
	glBegin(GL_QUADS);
	glColor3f (0.41f, 0.54f, 0.41f);
	glVertex3f( side/2+x, -side/2+y, side/2+z);
	glVertex3f(-side/2+x, -side/2+y, side/2+z);
	glVertex3f(-side/2+x,  side/2+y, side/2+z);
	glVertex3f( side/2+x,  side/2+y, side/2+z);
	glEnd();
    
	//bottom
	glBegin(GL_QUADS);
	glColor3f (1.0f, 0.96f, 0.93f);	
	glVertex3f( side/2+x,  -side/2+y,  side/2+z);
	glVertex3f(-side/2+x,  -side/2+y,  side/2+z);
	glVertex3f(-side/2+x,  -side/2+y, -side/2+z);
	glVertex3f( side/2+x,  -side/2+y, -side/2+z);
	glEnd();
    
	//left
	glBegin(GL_QUADS);
	glColor3f (0.74f, 0.74f, 0.74f);
	glVertex3f(-side/2+x,  side/2+y,  side/2+z);
	glVertex3f(-side/2+x, -side/2+y,  side/2+z);
	glVertex3f(-side/2+x, -side/2+y, -side/2+z);
	glVertex3f(-side/2+x,  side/2+y, -side/2+z);
	glEnd();
    
	//behind
	glBegin(GL_QUADS);
	glColor3f (0.75f, 1.0f, 0.75f);
	glVertex3f( side/2+x, -side/2+y, -side/2+z);
	glVertex3f(-side/2+x, -side/2+y, -side/2+z);
	glVertex3f(-side/2+x,  side/2+y, -side/2+z);
	glVertex3f( side/2+x,  side/2+y, -side/2+z);
	glEnd();
    
	//right
	glBegin(GL_QUADS);
	glColor3f (0.94f, 1.0f, 1.0f);
	glVertex3f(side/2+x,  side/2+y,	 side/2+z);
	glVertex3f(side/2+x, -side/2+y,  side/2+z);
	glVertex3f(side/2+x, -side/2+y, -side/2+z);
	glVertex3f(side/2+x,  side/2+y, -side/2+z);
	glEnd();

    //top
	glBegin(GL_QUADS);
	glColor3f (0.32f, 0.54f, 0.32f);
	glVertex3f( side/2+x, side/2+y,	side/2+z);
	glVertex3f(-side/2+x, side/2+y,	side/2+z);
	glVertex3f(-side/2+x, side/2+y,-side/2+z);
	glVertex3f( side/2+x, side/2+y,-side/2+z);
	glEnd();
}

void cubePositions(float x, float y, float z, float side)
{
	Cube(-side+x, 0.0f+y, -side+z, side);
	Cube(-side+x, 0.0f+y,  side+z, side);
	Cube( side+x, 0.0f+y, -side+z, side);
	Cube( side+x, 0.0f+y,  side+z, side);
	
	Cube(-side+x, side+y,  side+z, side);
	Cube(-side+x, side+y, -side+z, side);
	Cube(-side+x, side+y,  0.0f+z, side);
	Cube( 0.0f+x, side+y,  side+z, side);
	Cube( 0.0f+x, side+y, -side+z, side);
	Cube( side+x, side+y,  side+z, side);
	Cube( side+x, side+y, -side+z, side);
	Cube( side+x, side+y,  0.0f+z, side);
	
	Cube(-side+x, -side+y,  side+z,	side);
	Cube(-side+x, -side+y, -side+z,	side);
	Cube(-side+x, -side+y, -0.0f+z,	side);
	Cube( 0.0f+x, -side+y, -side+z,	side);
	Cube( 0.0f+x, -side+y,	side+z,	side);
	Cube( side+x, -side+y,	side+z,	side);
	Cube( side+x, -side+y, -side+z,	side);
	Cube( side+x, -side+y,	0.0f+z,	side);
}
void bigerCube(float x, float y, float z, float side)
{
	side=side*3;
	cubePositions(-side+x, 0.0f+y, -side+z,	side/3);
	cubePositions(-side+x, 0.0f+y,	side+z,	side/3);
	cubePositions( side+x, 0.0f+y, -side+z,	side/3);
	cubePositions( side+x, 0.0f+y,	side+z,	side/3);
	
	cubePositions(-side+x, side+y,	side+z,	side/3);
	cubePositions(-side+x, side+y, -side+z,	side/3);
	cubePositions(-side+x, side+y,  0.0f+z,	side/3);
	cubePositions( 0.0f+x, side+y,  side+z,	side/3);
	cubePositions( 0.0f+x, side+y, -side+z,	side/3);
	cubePositions( side+x, side+y,	side+z,	side/3);
	cubePositions( side+x, side+y, -side+z,	side/3);
	cubePositions( side+x, side+y,  0.0f+z,	side/3);

	cubePositions(-side+x, -side+y,	 side+z, side/3);
	cubePositions(-side+x, -side+y,	-side+z, side/3);
	cubePositions(-side+x, -side+y,	 0.0f+z, side/3);
	cubePositions( 0.0f+x, -side+y,	-side+z, side/3);
	cubePositions( 0.0f+x, -side+y,	 side+z, side/3);
	cubePositions( side+x, -side+y,  side+z, side/3);
	cubePositions( side+x, -side+y,	-side+z, side/3);
	cubePositions( side+x, -side+y,	-0.0f+z, side/3);
}

void bigestCube(float x, float y, float z, float side)
{
	side = side*9;
	bigerCube(-side+x, 0.0f+y, -side+z,	side/9);
	bigerCube(-side+x, 0.0f+y,	side+z,	side/9);
	bigerCube( side+x, 0.0f+y, -side+z,	side/9);
	bigerCube( side+x, 0.0f+y,	side+z,	side/9);
	
	bigerCube(-side+x, side+y,	side+z,	side/9);
	bigerCube(-side+x, side+y, -side+z,	side/9);
	bigerCube(-side+x, side+y, 	0.0f+z,	side/9);
	bigerCube( 0.0f+x, side+y,  side+z,	side/9);
	bigerCube( 0.0f+x, side+y, -side+z,	side/9);
	bigerCube( side+x, side+y,  side+z,	side/9);
	bigerCube( side+x, side+y, -side+z,	side/9);
	bigerCube( side+x, side+y, 	0.0f+z,	side/9);

	bigerCube(-side+x, -side+y,	 side+z, side/9);
	bigerCube(-side+x, -side+y,	-side+z, side/9);
	bigerCube(-side+x, -side+y,	 0.0f+z, side/9);
	bigerCube( 0.0f+x, -side+y,	-side+z, side/9);
	bigerCube( 0.0f+x, -side+y,	 side+z, side/9);
	bigerCube( side+x, -side+y,	 side+z, side/9);
	bigerCube( side+x, -side+y,	-side+z, side/9);
	bigerCube( side+x, -side+y,	 0.0f+z, side/9);	
}
//*****************************************************return;*************************************************************************************

//************************************************************INPUT*************************************************************************
void processInput(GLFWwindow* window) //use esc to quit the window and the up and down rotate
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) glRotatef(1.0f,0.0f,0.0f,1.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) glRotatef(-1.0f,0.0f,0.0f,1.0f);
}
int keyInput(GLFWwindow* window, int times) //use 1 2 3 4 to choose the level of cube you want
{
   	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) times = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) times = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) times = 3;
   	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) times = 4;
	return times;
}
void mouseRotate(GLFWwindow* window)//use mouse to control the left and right rotate
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) glRotatef(-1.0f,0.0f,1.0f,0.0f);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) glRotatef(1.0f,0.0f,1.0f,0.0f);
}
void scroll_callback(GLFWwindow* window, double x, double y)//use scroll to control the bigger or smaller
{
	if(y==1.0){glScalef(1.1f,1.1f,1.1f);}
	else if (y ==-1.0){glScalef(0.9f,0.9f,0.9f);}
}
void myPerspective(GLdouble fov, GLdouble aspectRatio, GLdouble zNear,GLdouble zFar)//replace the gluPerspective
{
	GLdouble rFov = fov *3.14159265/180.0;
	glFrustum(-zNear*tan(rFov/2.0)*aspectRatio,zNear*tan(rFov/2.0)*aspectRatio,-zNear*tan(rFov/2.0),zNear*tan(rFov/2.0),zNear,zFar);
}
void framebuffer_size_callback(GLFWwindow* window, int w, int h)//to do the perspective projection
{
	if(height == 0){height=1;}
	if(height>0){ratio =(float)w/(float)h;}
	glViewport(0,0,w, h);
	width=w;
	height=h;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	myPerspective(20.0f,ratio,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-5.0f);
	glRotatef (60,0.0f,1.0f,0.0f);//set the orignal cube's angle, let the cube is good looking

}
//******************************************************************************************************************************************

int main()
{
	if(!glfwInit())//glfw initialize
	{
		std::cout<<"Failed to initialize GLFW!\n"<<std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);//set the major version number
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);//set the minor version number
    
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A1", nullptr, nullptr);//set the window
	if(window == nullptr)//check the window
	{
		std::cout <<"Failed to create GLFW window"<<std::endl;
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(window);//set current context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//initialize glad before calling the opengl function
	{
		std::cout <<"Failed to initialize GLAD"<<std::endl;
		return -1;
	}
	float side = 1.0f;//set the cube side length as 1
	int times = 0;//set the orignal showing without cube, just the window, and then you can input the level number. I set 4 levels for my cube.
	
    glEnable(GL_DEPTH_TEST);// depth test

	//render loop
    while (!glfwWindowShouldClose(window))
    {
     	glClearColor (0.2f, 0.2f, 0.2f, 1.0f);//set the backgroud color
		glClearDepth(1.0);//specifies the clear value of the depth buffer
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//____________________________call inputs______________________________________________
		processInput(window);
		
		times=keyInput(window, times);

		if(times==1)
		{
			Cube(0.0f,0.0f,0.0f,side);
		}
		if(times==2)
		{
			cubePositions(0.0f,0.0f,0.0f,side/3);
		}
		if(times==3)
		{
			bigerCube(0.0f,0.0f,0.0f,side/9);
		}
		if(times==4)
		{
			bigestCube(0.0f,0.0f,0.0f,side/27);
		}
	
		mouseRotate(window);
		glfwSetScrollCallback(window,scroll_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//_____________________________________________________________________________________

        glfwSwapBuffers(window);//exchange the buffer
        glfwPollEvents();//the events processing
    };
	glfwDestroyWindow(window);//destory the window
	glfwTerminate();//stop
	return 0;
}


