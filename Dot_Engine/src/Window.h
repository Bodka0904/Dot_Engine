#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Log.h"



class Window
{
public:
	Window(int width, int height, const char* title);
	virtual~Window();
	void Init();
	void Update();
	bool IsClosed();
	void Terminate();
	void ShowWindow();
	void Clear();



	

	inline int GetWidth() const;
	inline int GetHeight() const;


	GLFWwindow* GetWindow() const;
private:
	GLFWwindow * m_window;
	int m_width;
	int m_height;
	const char* m_title;
	

};