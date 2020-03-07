////////////////////////////////////////////////////////////////////////////
//
//			CPSC453 Assignment04
//			Sijia Yin
//			30049836
//			2019-12-07
//			Note: reference--here are some websites i search for some resources.
//      		https://www.glprogramming.com/red/chapter12.html
//      		https://pages.cpsc.ucalgary.ca/~eharris/cpsc453/tut23/
//      		http://www.ams.org/publicoutreach/feature-column/fcarc-bezier
//      		https://community.khronos.org/t/drawing-points-where-you-click/74769/2
//      		https://gamedev.stackexchange.com/questions/65976/how-to-select-a-vertex-by-mouse-clicking-in-opengl
//      		https://www.glfw.org/docs/3.0/group__time.html
//      
////////////////////////////////////////////////////////////////////////////   


#include <array>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cassert>	//assert

 // glad beforw glfw
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"

#include "common_matrices.hpp"
//#include "obj_mesh_file_io.hpp"
//#include "obj_mesh.hpp"
#include "mat4f.hpp"
#include "mat3f.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "shader_file_io.hpp"
#include "buffer_object.hpp"
#include "vertex_array_object.hpp"
#include "vbo_tools.hpp"
//#include "texture.hpp"
//#include "image.hpp"

using namespace math;
using namespace geometry;
using namespace opengl; 

// GLOBAL Variables
Mat4f g_M = Mat4f::identity();
Mat4f g_V = Mat4f::identity();
Mat4f g_P = Mat4f::identity();

GLuint SCR_WIDTH = 1200, SCR_HEIGHT = 600;

// function declaration
using namespace std;

float depth = 100.0f;//change cut number
int clickPoint;
double second;

std::vector<Vec3f> pottery;
std::vector<Vec3f> potterynormal;
std::vector<Vec3f> tmp;

void setFrameBufferSize(GLFWwindow *window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	g_P = perspectiveProjection(30, float(SCR_WIDTH) / SCR_HEIGHT, 0.01, 100.f);
}

void setKeyboard(GLFWwindow *window, int key, int scancode, int action,
	int mods) {
	if (GLFW_KEY_ESCAPE == key) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}else if (GLFW_KEY_C == key) {
    	if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      	g_M = rotateAboutXMatrix(-5.0f) * g_M;
    }
  }
}
//move light from assignment02
Vec3f lightMoving(string x, Vec3f pos){
  float X = pos.x;
  float Y = pos.y;  
  float Z = pos.z;
  Vec3f v;
  if(x == "up"){
    X = X - 0.1;
    Y = Y + 0.1;
  }
  if(x == "down"){
    X = X + 0.1;
    Y = Y - 0.1;
  }
  if(x == "left"){
    X = X - 0.1;
    Z = Z + 0.1;
  }
  if(x == "right"){
    X = X + 0.1;
    Z = Z - 0.1;
  }
  v = normalized(Vec3f(X, Y, Z));
  return v;
}

// user defined alias
opengl::Program createShaderProgram(std::string const &vertexShaderFile,
                                    std::string const &fragmentShaderFile) {
	using namespace opengl;
	auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
	auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

	std::cout << "[Log] compiling program " << vertexShaderFile << ' '
		<< fragmentShaderFile << '\n';
	return opengl::makeProgram(vertexShaderSource, fragmentShaderSource);
}

opengl::Program createShaderProgram(std::string const &vertexShaderFile,
                                    std::string const &geometryShaderFile,
                                    std::string const &fragmentShaderFile) {
  using namespace opengl;
  auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
  auto geometryShaderSource = loadShaderStringFromFile(geometryShaderFile);
  auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

  std::cout << "[Log] compiling program " << vertexShaderFile << ' '
            << geometryShaderFile << ' ' << fragmentShaderFile << '\n';
  return opengl::makeProgram(vertexShaderSource, geometryShaderSource,
                             fragmentShaderSource);
}

std::string glfwVersion() {
	std::ostringstream s("GLFW version: ", std::ios::in | std::ios::ate);
	// print version
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
	s << glfwMajor << '.' << glfwMinor << '.' << glfwRevision;
	return s.str();
}

bool loadGeometryToGPU(std::vector<Vec3f> const &vertices, GLuint vboID) {
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(
		GL_ARRAY_BUFFER,                  // destination
		sizeof(Vec3f) * vertices.size(), // size (bytes) of memcopy to GPU
		vertices.data(),                  // pointer to data (contiguous)
		GL_STATIC_DRAW                    // usage patter of the GPU buffer
	);
	return true;
}

//************************************************************************
//************************************************************************
//************************************************************************
//https://www.glprogramming.com/red/chapter12.html
std::vector<Vec3f> subdividePoint(std::vector<Vec3f> const & points, float n) {
	std::vector<Vec3f> out;
	//TODO
	if(points.size() == 1){
		return points;
	}else{
		for(int i = 0; i < points.size()-1; i++){
			Vec3f point = (points[i+1] - points[i]) * n + points[i];
			out.push_back(point);
		}
	}
	return subdividePoint(out, n);
}
//https://pages.cpsc.ucalgary.ca/~eharris/cpsc453/tut23/
std::vector<Vec3f> subdivideClosedCurve(std::vector<Vec3f> const & points, float depth) {
	std::vector<Vec3f> out;
	//TODO
	if(points.size() <= 1){
		return points;
	}else if(points.size() == 2){
		out.push_back(points[0]);
		out.push_back(points[points.size()-1]);
	}else{
		out.push_back(points[0]);
		for(float i = depth; i < 1; i+=depth){
			tmp = subdividePoint(points, i);
			out.push_back(tmp[0]);
		}
		out.push_back(points[points.size()-1]);	
		out.push_back(points[0]);
	}
	return out;
}

std::vector<Vec3f> subdivideOpenCurve(std::vector<Vec3f> const & points, float depth) {
	std::vector<Vec3f> out;
	//TODO
	if(points.size() <= 1){
		return points;
	}else if(points.size() == 2){
		out.push_back(points[0]);
		out.push_back((points[1] - points[0]) * 0.5 + points[0]);
		out.push_back(points[points.size()-1]);
		out.push_back(points[0]);
	}else{
		out.push_back(points[0]);
		for(float i = depth; i < 1; i+=depth){
			tmp = subdividePoint(points, i);
			out.push_back(tmp[0]);
		}
		out.push_back(points[points.size()-1]);	
	}
	return out;
}
//https://stackoverflow.com/questions/7904281/opengl-rotate-a-curve-about-the-y-axis
Vec3f rotateCurve (Vec3f point){
	float radius = sqrt(point.x * point.x + point.z * point.z);
	float n = asin(point.z / radius) / 3.14159 * 180;
	if(point.x >= 0){
		point.z = radius * sin((n + 1) * 3.14159 / 180);
		point.x = sqrt(radius * radius - point.z * point.z);
		if(n > 90){
			point.x = -point.x;
		}
	}else{
		point.z = radius * sin((n - 1) * 3.14159 / 180);
		point.x = -sqrt(radius * radius - point.z * point.z);
		if(n < -90){
			point.x = -point.x;
		}
	}
	
	return(Vec3f(point.x, point.y, point.z));
}
//DRAW Triangle
std::vector<Vec3f> getPottery(std::vector<Vec3f> point){
	std::vector<Vec3f> out;
	tmp.clear();
	for(int i = 0; i < 360; i++){
		for(int j = 0; j < point.size(); j++){
			tmp.push_back(point[j]);
			tmp.push_back(rotateCurve(point[j]));
			point[j] = rotateCurve(point[j]);
		}
		for(int n = 0; n < tmp.size()-1; n++){
			out.push_back(tmp[n+0]);
			out.push_back(tmp[n+1]);
			out.push_back(tmp[n+2]);
			out.push_back(tmp[n+3]);
			out.push_back(tmp[n+2]);
			out.push_back(tmp[n+1]);
		}	
		tmp.clear();
	}
	return out;
} 

//From obj_mesh.cpp
std::vector<Vec3f> getVertexNormals(std::vector<Vec3f> point, std::vector<Vec3f> curve){
	std::vector<Vec3f> out;
	tmp.clear();
	for(int i=0; i< curve.size()*360; i++){
		tmp.push_back({0,0,0});//(0,0,0)
	}
	for(int i = 0; i < tmp.size()-1; i++){
		out.push_back(tmp[i+0]);
		out.push_back(tmp[i+1]);
		out.push_back(tmp[i+2]);
		out.push_back(tmp[i+3]);
		out.push_back(tmp[i+2]);
		out.push_back(tmp[i+1]);	
	}	
	tmp.clear();
	return out;
}

//From assignment02
//https://community.khronos.org/t/drawing-points-where-you-click/74769/2
//https://gamedev.stackexchange.com/questions/65976/how-to-select-a-vertex-by-mouse-clicking-in-opengl
void movePoint(float x, float y, std::vector<Vec3f> controlPoints, float xpos, float ypos){
	xpos = xpos/SCR_WIDTH*4 - 3;
	ypos = -ypos/SCR_HEIGHT*2 + 1;	
	
	//to get the point which is used to move
	for(int i = 0; i < controlPoints.size(); i++){
		if (controlPoints[i].x -0.05 < xpos && xpos < controlPoints[i].x + 0.05){
			if (controlPoints[i].y - 0.05 < ypos && ypos < controlPoints[i].y + 0.05){
				clickPoint = i;
			}
		}
	}
	if(clickPoint != -1){
		controlPoints[clickPoint].x += x/SCR_WIDTH*4;
		controlPoints[clickPoint].y += -y/SCR_HEIGHT*2;	
		if(controlPoints[clickPoint].x < -1){
			controlPoints[clickPoint].x = -1;
		}else if(controlPoints[clickPoint].x > 1){
			controlPoints[clickPoint].x = 1;
		}
		if(controlPoints[clickPoint].y < -1){
			controlPoints[clickPoint].y = -1;
		}else if(controlPoints[clickPoint].y > 1){
			controlPoints[clickPoint].y = 1;
		}
	}
	tmp = controlPoints;
	return;
}

std::vector<Vec3f> deletPoint(GLFWwindow* window, std::vector<Vec3f> controlPoints){
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){ 
		if(controlPoints.size() > 2){
			for(int n = 0; n < clickPoint; n++){
				tmp.push_back(controlPoints[n]);
			}			
			for(int i = clickPoint+1; i < controlPoints.size(); i++){
				tmp.push_back(controlPoints[i]);
			}
			controlPoints = tmp;	
			clickPoint = -1;
		}
	}
	return controlPoints;
}

std::vector<Vec3f> addPoint(GLFWwindow* window, std::vector<Vec3f> controlPoints){
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){  
		//https://www.glfw.org/docs/3.0/group__time.html
		second = glfwGetTime() - second;//add one point per time
		if(second >= 1){			
			tmp.push_back(Vec3f(0.0f,0.0f,0.0f));
			for(int n = 0; n < controlPoints.size(); n++){
				tmp.push_back(controlPoints[n]);
			}					
			controlPoints = tmp;					
		}
		second = glfwGetTime();				    
	}
	return controlPoints;
}
//************************************************************************
//************************************************************************
//************************************************************************

void setupVAO(GLuint vaoID, GLuint vboID) {
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	
	// set up position input into vertex shader
	glEnableVertexAttribArray(0);          // match layout # in shader
	glVertexAttribPointer(                 //
		0,                                 // attribute layout # (in shader)
		3,                                 // number of coordinates per vertex
		GL_FLOAT,                          // type
		GL_FALSE,                          // normalized?
		sizeof(Vec3f),                    // stride
		0									 // array buffer offset
	);

	glBindVertexArray(0);
}

void setupVAONormal(GLuint vaoID, GLuint vboID) {
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	glEnableVertexAttribArray(0); // match layout # in vertex shader
	glVertexAttribPointer(        //
		0,                        // attribute layout # (in shader)
		3,                        // number of coordinates per vertex
		GL_FLOAT,                 // type
		GL_FALSE,                 // normalized?
		sizeof(Vec3f),      // stride
		(void *)(0)  // array buffer offset
		);

	// normals
	glEnableVertexAttribArray(1); // match layout # in vertex shader
	glVertexAttribPointer(        //
		1,                        // attribute layout # (in shader)
		3,                        // number of coordinates per vertex
		GL_FLOAT,                 // type
		GL_FALSE,                 // normalized?
		sizeof(Vec3f),      // stride
		(void *)(sizeof(Vec3f) * pottery.size())   // array buffer offset
		);	


	glBindVertexArray(0);
}

GLFWwindow *initWindow() {
	GLFWwindow *window = nullptr;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	std::cout << glfwVersion() << '\n';

	// set opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(SCR_WIDTH,          // width
                              SCR_HEIGHT,         // height
                              "Assignment04",   // title
                              NULL,             // unused
                              NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1); // vsync
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);

	//Polygon fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// setup callbacks
	glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
	glfwSetKeyCallback(window, setKeyboard);

	return window;
}

int main() {
	GLFWwindow *window = initWindow();

	auto vao_control = makeVertexArrayObject();
	auto vbo_control = makeBufferObject();
	auto vao_curve = makeVertexArrayObject();
	auto vbo_curve = makeBufferObject();
	//pottery
	auto vao_pottery = makeVertexArrayObject();
	auto vbo_pottery = makeBufferObject();
	auto vao_potterynormal = makeVertexArrayObject();
	auto vbo_potterynormal = makeBufferObject();

	/*
	Vec3f viewPosition(0, 0, 3);
	g_V = lookAtMatrix(viewPosition,    // eye position
		{ 0.f, 0.f, 0.f }, // look at
		{ 0.f, 1.f, 0.f }  // up vector
	);
	g_P = orthographicProjection(-1, 1, 1, -1, 0.001f, 10);
	*/

	auto basicShader = createShaderProgram("../shaders/basic_vs.glsl",
										"../shaders/basic_fs.glsl");
	setupVAO(vao_control.id(), vbo_control.id());
	setupVAO(vao_curve.id(), vbo_curve.id());

	setupVAONormal(vao_pottery.id(), vbo_pottery.id());
	setupVAONormal(vao_potterynormal.id(), vbo_potterynormal.id());

	//Load control points
	std::vector<Vec3f> controlPoints;
	controlPoints.push_back({ 0.0f,	0.80f,0.0f });
	controlPoints.push_back({ 0.6f,	0.80f,0.0f });
	controlPoints.push_back({-0.9f,	0.70f,0.0f });
	controlPoints.push_back({ 0.6f,	0.10f,0.0f });
	controlPoints.push_back({ 0.9f,-0.80f,0.0f });	
	controlPoints.push_back({ 0.0f,-0.80f,0.0f });
	loadGeometryToGPU(controlPoints, vbo_control.id());
	
	//Load curve points
	std::vector<Vec3f> outCurve;
	outCurve = subdivideOpenCurve(controlPoints, 1/depth);//TODO - insert subdivision here
	loadGeometryToGPU(outCurve, vbo_curve.id());

	pottery = getPottery(outCurve);
	loadGeometryToGPU(pottery, vbo_pottery.id());
	potterynormal = getVertexNormals(pottery, outCurve);
	loadGeometryToGPU(potterynormal, vbo_potterynormal.id());
		
	Vec3f color_curve(0, 1, 1);
	Vec3f color_control(1, 0, 0);

	//From Assignment02
	double xpos; 
	double ypos;
	int width; 
	int height;

	g_V = math::lookAtMatrix({0.f, 0.f, 3.f},	//eye position
							 {0.f, 0.f, 0.f},	//look at
							 {0.f, 1.f, 0.f});	//up vector
	g_P = math::orthographicProjection(-1.f, 1.f, 1.f, -1.f, 0.1, 100.0);

	glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
	glfwSetKeyCallback(window, setKeyboard);
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, SCR_WIDTH/4, SCR_HEIGHT/2);

	double xPosition = xpos;
	double yPosition = ypos;

	float ambientStrength = 0.2;	
	float specularStrength = 0.5;

	Vec3f lightPosition(10, 10, 10);
	Vec3f viewPos(0, 0, 3);
	Vec3f lightColor(1.0,1.0,1.0);
	
	auto textureShader = createShaderProgram("../shaders/texture_vs.glsl",
										"../shaders/texture_gs.glsl",
										"../shaders/texture_fs.glsl");

	assert(textureShader && basicShader);//Check console for program compilation error
	basicShader.use();
	opengl::Program *program = &basicShader;

	second = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		program->use();				
		setUniformMat4f(program->uniformLocation("model"), g_M, true);
		setUniformMat4f(program->uniformLocation("view"), g_V, true);
		setUniformMat4f(program->uniformLocation("projection"), g_P, true);
		setUniformVec3f(textureShader.uniformLocation("lightPosition"), lightPosition);
		setUniformVec3f(textureShader.uniformLocation("lightColor"), lightColor);
		setUniformVec3f(textureShader.uniformLocation("viewPos"), viewPos);
		setUniform1f(textureShader.uniformLocation("ambientStrength"), ambientStrength);
		setUniform1f(textureShader.uniformLocation("specularStrength"), specularStrength);

		pottery = getPottery(outCurve);
		loadGeometryToGPU(pottery, vbo_pottery.id());
		potterynormal = getVertexNormals(pottery, outCurve);
		loadGeometryToGPU(potterynormal, vbo_potterynormal.id());
		
		glfwGetFramebufferSize(window, &width, &height);
		glfwGetCursorPos(window, &xpos, &ypos);
		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){  
			movePoint(xpos-xPosition, ypos-yPosition, controlPoints, xpos, ypos);
			controlPoints = tmp;
			tmp.clear();
			loadGeometryToGPU(controlPoints, vbo_control.id());
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){	
      		lightPosition = lightMoving("up", lightPosition);
  		}	
    	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ){
      		lightPosition = lightMoving("down", lightPosition);
  		} 
    	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ) {
     	 	lightPosition = lightMoving("left", lightPosition);
    	}
  		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ){	
     		lightPosition = lightMoving("right", lightPosition);
  		}

		xPosition = xpos;
		yPosition = ypos;

		controlPoints = addPoint(window, controlPoints);
		controlPoints = deletPoint(window, controlPoints);
		
		outCurve = subdivideOpenCurve(controlPoints, 1/depth);

		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS ){
			outCurve = subdivideClosedCurve(controlPoints, 1/depth);	
		}
			
		loadGeometryToGPU(outCurve, vbo_curve.id());
		loadGeometryToGPU(controlPoints, vbo_control.id());

		program->use();
			
		glViewport((float)SCR_WIDTH/2, //
					0, 					//
				   (float)SCR_WIDTH/2, //
				    SCR_HEIGHT);
		basicShader.use();	
		program = &basicShader;

		//Draw curve
		setUniformVec3f(basicShader.uniformLocation("color"), color_curve);	
		vao_curve.bind();
		glDrawArrays(GL_LINE_STRIP,   // type of drawing (rendered to back buffer)
					0,				  // offset into buffer
					outCurve.size()); // number ofyoffset vertices in buffer

		if(clickPoint != -1){
			glPointSize(10);
			setUniformVec3f(basicShader.uniformLocation("color"), color_curve);
			vao_control.bind();
			glDrawArrays(GL_POINTS, clickPoint, 1 );
		}

		//Draw control points
		setUniformVec3f(basicShader.uniformLocation("color"), color_control);
		vao_control.bind();
		glDrawArrays(GL_LINE_STRIP,   // type of drawing (rendered to back buffer)
			0,						  // offset into buffer
			controlPoints.size()	// number of vertices in buffer
		);

		glDrawArrays(GL_POINTS,   // type of drawing (rendered to back buffer)
			0,						  // offset into buffer
			controlPoints.size()	// number of vertices in buffer
		);

		//Draw control pottery
		glViewport(0, 0, (float)SCR_WIDTH / 2.f, SCR_HEIGHT);
		textureShader.use();	
		program = &textureShader;

		vao_pottery.bind();   	
		glDrawArrays(GL_TRIANGLES,  // type of drawing (rendered to back buffer)
					0,				// offset into buffer
					pottery.size()	// number of vertices in buffer
		);
		
		glfwSwapBuffers(window); 	// swaps back buffer to front for drawing to screen
		glfwPollEvents();        	// will process event queue and carry on
	}

	// cleaup window, and glfw before exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
