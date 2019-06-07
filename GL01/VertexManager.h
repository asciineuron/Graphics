#ifndef VERTEX_MANAGER_H
#define VERTEX_MANAGER_H

#include <iostream>

class VertexManager
{
public:
  float *vertices;
  unsigned int *indices;
  unsigned int ID; // id of VAO as will be used to wrap vbo/ebo
  unsigned int VBOID;
  unsigned int EBOID;
  int numVertices;
  int numIndices;
  bool colored;
  // for now can handle 2 types, colored and uncolored
  VertexManager(float *vertices, int verticesSize, unsigned int *indices, int indicesSize, bool colored);
  void use(); // binds vao, draws, unbinds
};

VertexManager::VertexManager(float *vertices, int verticesSize, unsigned int *indices, int indicesSize, bool colored)
{
  this->vertices = vertices;
  this->indices = indices;
  // initialize VAO
  glGenVertexArrays(1, &ID);
  glBindVertexArray(ID);
  // initialize VBO
  glGenBuffers(1, &VBOID);
  glBindBuffer(GL_ARRAY_BUFFER, VBOID);
  glBufferData(GL_ARRAY_BUFFER, verticesSize, this->vertices, GL_STATIC_DRAW);
  // if indices specified i.e. not null, initialize EBO
  if (indices != NULL)
    {
      glGenBuffers(1, &EBOID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, this->indices, GL_STATIC_DRAW);
    }
  // if colored, use two vertex attrib pointers, else one
  if (colored)
    {
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
    } else
    {
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
    }
  glBindVertexArray(0); //unbind so doesn't interfere
  // determine number of indices and vertexes, 6 floats each if colored, else 3
  this->numVertices = verticesSize / ((colored ? 6.0 : 3.0)*(sizeof(float)));
  this->numIndices = indicesSize / sizeof(unsigned int);
}

void VertexManager::use()
{
  // bind, draw, unbind
  glBindVertexArray(ID);
  if (indices != NULL)
    {
      glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    } else
    {
      glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }
  glBindVertexArray(0);
}

#endif
