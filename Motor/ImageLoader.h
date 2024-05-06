#pragma once
#include"GLTexture.h"
#include<string>

using namespace std;
// img -> ioManager(bytes) -> ImageLoader 
class ImageLoader
{
public:
	static GLTexture loadPng(string filePath);
};

