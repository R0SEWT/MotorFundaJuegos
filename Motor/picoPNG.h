#pragma once
#include<vector>

//  Usamos picoPNG para manejar los diferentes tipos de formatos de 
//  imagenes y cargalos a la grafica

extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

// metodo extern
// propio de C++
// varible global, al incluir inyectara las declaraciones y asignaciones