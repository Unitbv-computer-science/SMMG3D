// Lab5 - Load Mesh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Lab5Engine.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#pragma comment (lib, "Engine3D.lib")

int main()
{
   Lab5Engine renderEngine("Lab5", WINDOW_WIDTH, WINDOW_HEIGHT);

   renderEngine.Init();

   renderEngine.Run();

   renderEngine.Exit();

   return 0;
}

