/**
 * @file gl.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#include <iostream>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;

/**
 * @brief glfw error callback function
 * 
 * @param err	error number
 * @param msg 	description
 */
void err_callback(int err, const char *msg){
	cout << "err:" << err << msg << endl;
}

static void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

/**
 * @brief main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char ** argv){
	// set error callback
	glfwSetErrorCallback(err_callback);

	// initialize
	if(!glfwInit()){
		cout << "init error, exit" << endl;
	}

	// create a window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
	if(!pWd){
		cout << "create window failed!" << endl;
	}

	// set key callback
	glfwSetKeyCallback(pWd, key_callback);

	// make opengl context
	glfwMakeContextCurrent(pWd);

	// set viewport
	int height=0, width=0;
	glfwGetFramebufferSize(pWd, &width, &height);
	glViewport(0, 0, width, height);

	// get time
	double t = glfwGetTime();

	// buffer swapping setting
	glfwSwapInterval(1);

	// running until exit
	while(!glfwWindowShouldClose(pWd)){
		glfwSwapBuffers(pWd);
		glfwPollEvents();
	}

	cout << "user request to close this window!" << endl;


	// destroy window
	glfwDestroyWindow(pWd);

	// terminate
	glfwTerminate();
}
