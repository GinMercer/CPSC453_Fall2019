//CPSC453 Assignment02
//Sijia Yin
//30049836
//2019-10-29
//Note: reference--here are some websites i search for some resources.
//      https://blog.csdn.net/zach_z/article/details/80153536 texture
//      https://www.cnblogs.com/icyhusky/p/10686747.html texture
//      https://www.xuebuyuan.com/3180909.html texture
//      http://www.doc88.com/p-1764884686105.html texture
//      https://www.xuebuyuan.com/2136217.html light
//      https://www.jianshu.com/p/0982a0d66b98  light
//      https://www.geeks3d.com/20111111/simple-introduction-to-geometry-shaders-glsl-opengl-tutorial-part1/  shader
//      http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/ mouse move

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
#include "obj_mesh_file_io.hpp"
#include "obj_mesh.hpp"
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
#include "texture.hpp"
#include "image.hpp"

using namespace math;
using namespace geometry;
using namespace opengl;

// GLOBAL Variables
Mat4f g_M = Mat4f::identity();
Mat4f g_V = Mat4f::identity();
Mat4f g_P = Mat4f::identity();

GLuint SCR_WIDTH = 1200, SCR_HEIGHT = 1200;
std::string object="teapot";

Vec3f lightPosition(10, 10, 10);

// function declaration
using namespace std;

void setFrameBufferSize(GLFWwindow *window, int width, int height) {
  SCR_WIDTH = width;
  SCR_HEIGHT = height;
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  g_P = perspectiveProjection(30, float(SCR_WIDTH) / SCR_HEIGHT, 0.01, 100.f);
}

void scroll_callback(GLFWwindow* window, double x, double y){
	if(y ==  1.0f){g_M = g_M*uniformScaleMatrix(1.1f);}
	if(y == -1.0f){g_M = g_M*uniformScaleMatrix(0.9f);}
}

void setKeyboard(GLFWwindow *window, int key, int scancode, int action,int mods)
{
  if (GLFW_KEY_KP_2 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutXMatrix(5.0f) * g_M;
    }
  } else if (GLFW_KEY_KP_8 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutXMatrix(-5.0f) * g_M;
    }
  } else if (GLFW_KEY_KP_5 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutYMatrix(5.0f) * g_M;
    }
  } else if (GLFW_KEY_KP_0 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutYMatrix(-5.0f) * g_M;
    }
  } else if (GLFW_KEY_KP_4 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutZMatrix(5.0f) * g_M;
    }
  } else if (GLFW_KEY_KP_6 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = rotateAboutZMatrix(-5.0f) * g_M;
    }  
  } else if (GLFW_KEY_W == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(0.0f, 0.3f, 0.0f) * g_M;
    }
  } else if (GLFW_KEY_S == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(0.0f, -0.3f, 0.0f) * g_M;
    }
  } else if (GLFW_KEY_A == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(-0.3f, 0.0f, 0.0f) * g_M;
    }
  } else if (GLFW_KEY_D == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      g_M = translateMatrix(0.3f, 0.0f, 0.0f) * g_M;
    }
  } else if (GLFW_KEY_ESCAPE == key) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }else if (GLFW_KEY_1 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      object = "teapot";
    }  
  }else if (GLFW_KEY_2 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      object = "spot";
    }
  }else if (GLFW_KEY_3 == key) {
    if (GLFW_REPEAT == action || GLFW_PRESS == action) {
      object = "Nefertiti";
    }
  }
}

void move(float x, float y, int width, int height)
{
  float xeyes = -g_V[3];
  xeyes -= x/(int) (width/2);
  if (xeyes >= 100.0f){xeyes = -100.0f;}

  float yeyes = -g_V[7];
  yeyes += y/(int) (height/2);
  if (yeyes >= 100.0f){yeyes = -100.0f;}
 
  g_V = math::lookAtMatrix( {xeyes, yeyes, 3.0f}, 
                            {xeyes, yeyes, 0.0f}, 
                            {0.0f, 1.0f, 0.0f});	
}

void rotate(float x, float y){
  if (x < 0.0f){g_M = g_M*rotateAboutYMatrix(-5.0f);}   
  if (x > 0.0f){g_M = g_M*rotateAboutYMatrix(5.0f);}
  if (y < 0.0f){g_M = g_M*rotateAboutXMatrix(-5.0f);}
  if (y > 0.0f){g_M = g_M*rotateAboutXMatrix(5.0f);}
}

math::Mat4f perspect(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS ){
		g_P = math::orthographicProjection(-1.0f, 1.0f, 1.0f, -1.0f, 0.1f, 100.0f);
		g_V = math::lookAtMatrix( {0.0f, 0.0f, 3.0f}, 
                              {0.0f, 0.0f, 0.0f}, 
                              {0.0f, 1.0f, 0.0f});}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){	
    g_P = math::perspectiveProjection(30.0f, 1.0f, 0.1f, 100.0f);
    g_V = math::lookAtMatrix( {0.0f, 0.0f, 3.0f}, 
                              {0.0f, 0.0f, 0.0f}, 
                              {0.0f, 1.0f, 0.0f});}	           
  return g_P, g_V;
};

void wireFrame(GLFWwindow* window){
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS ){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );}
  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS ){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){}

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

  window = glfwCreateWindow(SCR_WIDTH,           // width
                            SCR_HEIGHT,           // height
                            "Assignment02", // title
                            NULL,           // unused
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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  //Polygon fill mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // setup callbacks
  glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
  glfwSetKeyCallback(window, setKeyboard);

  return window;
}

void bindTexture(opengl::Texture const &texture, opengl::Program const &program, std::string s, GLuint activeTex) {
	assert(activeTex < 32);
	glActiveTexture(GL_TEXTURE0 + activeTex); // Activate texture in position "activeTex"
	texture.bind();		//Bind texture object to active texture
	
	//Set bound texture to sampler2D uniform in shader program in position "activeTex" 
	program.use();
	glUniform1i(program.uniformLocation(s), activeTex);	
}

int main() {
  GLFWwindow *window = initWindow();

  GLuint totalIndices = 0;

//teapot**********************************************************************
  auto teapotvao = makeVertexArrayObject();
  auto teapotindexBuffer = makeBufferObject();
  auto teapotvertexBuffer = makeBufferObject();
  GLuint teapottotalIndices = 0;

  {
    geometry::OBJMesh meshData;
    std::string filePath = "../models/teapot/teapot/teapot_triangulated.obj";

    if (!geometry::loadOBJMeshFromFile(filePath, meshData)) {
      std::cerr << "[Error] Cannot load .obj\n";
    }

	  //Calculate vertex normals if they are not included in .obj file
	  if (meshData.normals.size() == 0) {
		  auto normals =
			  geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);
		
		  auto vboData = opengl::makeConsistentVertexNormalIndices(meshData, normals);

		  teapottotalIndices =
			  opengl::setup_vao_and_buffers(teapotvao, teapotindexBuffer, teapotvertexBuffer, vboData);
	  }

	  //Pass in vertex normals if they are already included in .obj file
	  else {		
		  auto vboData = opengl::makeConsistentVertexNormalIndices(meshData);

		  teapottotalIndices =
		  	opengl::setup_vao_and_buffers(teapotvao, teapotindexBuffer, teapotvertexBuffer, vboData);
	  }
  }
//***************************************************************************

//spot***********************************************************************
  auto spotvao = makeVertexArrayObject();
  auto spotindexBuffer = makeBufferObject();
  auto spotvertexBuffer = makeBufferObject();
  GLuint spottotalIndices = 0;

  auto spottex = generateTexture();
  {
    geometry::OBJMesh meshData;
  
    //std::string filePath = "../models/teapot/teapot/teapot_triangulated.obj";
    std::string filePath = "../models/spot/spot/spot_triangulated.obj";
    //std::string filePath = "../models/Nefertiti_High/Nefertiti_High/Nefertiti_High.obj";

    if (!geometry::loadOBJMeshFromFile(filePath, meshData)) {
      std::cerr << "[Error] Cannot load .obj\n";
    }

	  //Calculate vertex normals if they are not included in .obj file
	  if (meshData.normals.size() == 0) {
		  auto normals =
			  geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);
		
	  	auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData, normals);

		  spottotalIndices =
			  opengl::setup_vao_and_buffers(spotvao, spotindexBuffer, spotvertexBuffer, vboData);
	  }

	  //Pass in vertex normals if they are already included in .obj file
	  else {		
		  if (meshData.textureCoords.empty()) {
			  auto vboData = opengl::makeConsistentVertexNormalIndices(meshData);

			  spottotalIndices =
				  opengl::setup_vao_and_buffers(spotvao, spotindexBuffer, spotvertexBuffer, vboData);
		  }
		  else {
			  auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData);

		  	spottotalIndices =
				  opengl::setup_vao_and_buffers(spotvao, spotindexBuffer, spotvertexBuffer, vboData);
		  } 
    }
  } // ... mesh is deconstruced here (freeing memory)

  {
	  auto image = raster::read_image_from_file(
		  "../models/spot/spot/spot_texture.png");

	  glActiveTexture(GL_TEXTURE0);	
	  spottex.bind();
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	  auto channels = GL_RGBA;
	  if (image.channels() == 3)
		  channels = GL_RGB;
	  
	  glTexImage2D(GL_TEXTURE_2D,  //target
		0,                 		//level
		channels,         		//internalformat
		image.width(),    		//width
		image.height(),   		//height
		0,                		//border
		channels,           	//format
		GL_UNSIGNED_BYTE, 		//type
		image.data());			//data
	  
	  glGenerateMipmap(GL_TEXTURE_2D);
  }
//***************************************************************************

//Nefertiti//****************************************************************
  auto Nefertitivao = makeVertexArrayObject();
  auto NefertitiindexBuffer = makeBufferObject();
  auto NefertitivertexBuffer = makeBufferObject();
  GLuint NefertititotalIndices = 0;

  auto Nefertititex = generateTexture();
   {
    geometry::OBJMesh meshData;
  
    //std::string filePath = "../models/teapot/teapot/teapot_triangulated.obj";
    //std::string filePath = "../models/spot/spot/spot_triangulated.obj";
    std::string filePath = "../models/Nefertiti_High/Nefertiti_High/Nefertiti_High.obj";

    if (!geometry::loadOBJMeshFromFile(filePath, meshData)) {
      std::cerr << "[Error] Cannot load .obj\n";
    }

	  //Calculate vertex normals if they are not included in .obj file
	  if (meshData.normals.size() == 0) {
		  auto normals =
			  geometry::calculateVertexNormals(meshData.triangles, meshData.vertices);
		
		  auto vboData = opengl::makeConsistentVertexNormalIndices(meshData, normals);

		  NefertititotalIndices =
		  	opengl::setup_vao_and_buffers(Nefertitivao, NefertitiindexBuffer, NefertitivertexBuffer, vboData);
	  }

	  //Pass in vertex normals if they are already included in .obj file
	  else {		
		  if (meshData.textureCoords.empty()) {
			  auto vboData = opengl::makeConsistentVertexNormalIndices(meshData);

		    NefertititotalIndices =
			    opengl::setup_vao_and_buffers(Nefertitivao, NefertitiindexBuffer, NefertitivertexBuffer, vboData);
		  }
		  else {
			  auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData);

			  NefertititotalIndices =
				  opengl::setup_vao_and_buffers(Nefertitivao, NefertitiindexBuffer, NefertitivertexBuffer, vboData);
		  } 
	  }
  } // ... mesh is deconstruced here (freeing memory)

  {
	  auto image = raster::read_image_from_file(
		  "../models/Nefertiti_High/Nefertiti_High/COLOR_High.jpg");

	  glActiveTexture(GL_TEXTURE0);	
	  Nefertititex.bind();
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	  auto channels = GL_RGBA;
	  if (image.channels() == 3)
		  channels = GL_RGB;
	  
	  glTexImage2D(GL_TEXTURE_2D,  //target
		0,                 		//level
		channels,         		//internalformat
		image.width(),    		//width
		image.height(),   		//height
		0,                		//border
		channels,           	//format
		GL_UNSIGNED_BYTE, 		//type
		image.data());			//data
	  
	  glGenerateMipmap(GL_TEXTURE_2D);
  }
//********************************************************************************
  //auto tex = generateTexture();

  double xpos;
  double ypos;
  int width;
  int height;

  g_P = math::perspectiveProjection(30.0f, 1.0f, 0.1f, 100.0f);
  g_V = math::lookAtMatrix( {0.0f, 0.0f, 3.0f}, 
                            {0.0f, 0.0f, 0.0f}, 
                            {0.0f, 1.0f, 0.0f});
	
  glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
  glfwSetScrollCallback(window, scroll_callback);	
  glfwSetKeyCallback(window, setKeyboard);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
  glfwSetCursorPos(window, SCR_WIDTH/2, SCR_HEIGHT/2);

  glfwSetScrollCallback(window, scroll_callback);

  double xPosition = xpos;
  double yPosition = ypos;

  Vec3f lightPosition(1.0, 0.0, 0.0);
  Vec3f viewPosition(0, 0, 3);

  g_V = lookAtMatrix(viewPosition,    // eye position
                     {0.f, 0.f, 0.f}, // look at
                     {0.f, 1.f, 0.f}  // up vector
                     );
  g_P = perspectiveProjection(30.,  // fov
                              1.f,  // ratio
                              0.1f, // distance to near plane
                              100.f // distance to far plane
                              );

  auto depthShader = createShaderProgram("../shaders/depth_vs.glsl",
										"../shaders/depth_fs.glsl");
  auto normalShader = createShaderProgram("../shaders/normal_vs.glsl",
                                        "../shaders/normal_fs.glsl");
  auto textureShader = createShaderProgram("../shaders/texture_vs.glsl",
										"../shaders/texture_gs.glsl",
										"../shaders/texture_fs.glsl");

  assert(depthShader && normalShader && textureShader);	//Check console for program compilation error

  depthShader.use();
  setUniformVec3f(depthShader.uniformLocation("lightPosition"), lightPosition);
  normalShader.use();
  setUniformVec3f(normalShader.uniformLocation("lightPosition"), lightPosition);
  textureShader.use();
  setUniformVec3f(textureShader.uniformLocation("lightPosition"), lightPosition);
  
  //Set to one shader program 
  //opengl::Program *program = &depthShader;
  //opengl::Program *program = &normalShader;
  opengl::Program *program = &textureShader;

  //Load texture to OpenGL
  bindTexture(spottex, *program, "tex", 0);
  bindTexture(Nefertititex, *program, "tex", 0);

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glfwGetFramebufferSize(window, &width, &height);
    wireFrame(window);
    glfwGetCursorPos(window, &xpos, &ypos);
    
    g_P, g_V = perspect(window); 


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){  
      move(xpos-xPosition, ypos-yPosition, width, height);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
      rotate(xpos-xPosition, ypos-yPosition);
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

    program->use();

    if(object == "teapot"){
        teapotvao.bind();
        totalIndices = teapottotalIndices;}
    if(object == "spot"){
        spotvao.bind();
        spottex.bind();
        totalIndices = spottotalIndices;}
    if(object == "Nefertiti"){
        Nefertitivao.bind();
        Nefertititex.bind();
        totalIndices = NefertititotalIndices;}
  
    setUniformMat4f(program->uniformLocation("model"), g_M, true);
    setUniformMat4f(program->uniformLocation("view"), g_V, true);
    setUniformMat4f(program->uniformLocation("projection"), g_P, true);
    setUniformVec3f(textureShader.uniformLocation("lightPosition"), lightPosition);
  
    glDrawElements(GL_TRIANGLES,
                   totalIndices,    // # of triangles * 3
                   GL_UNSIGNED_INT, // type of indices
                   (void *)0        // offset
                   );

    glfwSwapBuffers(window); // swaps back buffer to front for drawing to screen
    glfwPollEvents();        // will process event queue and carry on
  }

  // cleaup window, and glfw before exit
  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
