#include "ImageLoader.h"
#include "picoPNG.h"
#include "Error.h"
#include "IOManager.h"

GLTexture ImageLoader::loadPng(string filePath)
{
    GLTexture texture = {};
    vector<unsigned char>out, in;

    unsigned long width, height;// ulong ': respuesta del pico
    if (!IOManager::readFileToBuffer(filePath, in)) {
        fatalError("No se puede abrir el archivo " + filePath);
    }
    int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
    if (errorCode != 0) {
        fatalError("Decode PNG con error " + to_string(errorCode));
    }
    //vamos a generar texturas
    glGenTextures(1, &(texture.id));//solo 1 textura y le mandamos &
    // all gl_texture2D
    glBindTexture(GL_TEXTURE_2D, texture.id); //enlazamos a la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, // seteamos RGBA
        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
    // cuando no sabes los parametro pon , para ver el constructor
    // fondos que se repiten (EL qe salio de escena sale atras)
    // Wrap, acomodas el fondo y no se pixelea
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D); //Solo tiene x como textura chiqt dentro
    glBindTexture(GL_TEXTURE, 0);
    texture.height = height;
    texture.width = width;
    return texture;
}
