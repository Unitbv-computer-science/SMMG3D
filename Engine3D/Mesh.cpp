#include "stdafx.h"

#include "Mesh.h"
#include "Shader.h"

#include <GL/glew.h>
#include <glfw3.h>

#include <gtx/normal.hpp>

Mesh::Mesh()
{
}

Mesh::Mesh(const size_t nVertices, const GLfloat pVertices[],
   const size_t nIndices, const unsigned int pIndices[])
{
   std::vector<Vertex> vertices;
   vertices.resize(nVertices);
   for (size_t i = 0; i < nVertices; i++) {
      vertices[i].Position = glm::vec3(pVertices[i*3 + 0], pVertices[i*3 + 1], pVertices[i*3 + 2]);
   }

   std::vector<unsigned int> indices;
   indices.resize(nIndices);
   for (size_t i = 0; i < nIndices; i++) {
      indices[i] = pIndices[i];
   }

   std::vector<STexture> textures;
   Init(vertices, indices, textures, true);
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
   std::vector<STexture> textures, bool bComputeNormals)
{
   Init(vertices, indices, textures, bComputeNormals);
}

void Mesh::ComputeNormals()
{
   if (vertices.size() <= 0 || indices.size() <= 0) {
      return;
   }

   glm::vec3 vNormal;

   glm::vec3* pNormals = new glm::vec3[vertices.size()];
   for (size_t i = 0; i < vertices.size(); i++) {
      pNormals[i] = glm::vec3(0.0);
   }
   
   for (size_t i = 0; i < indices.size(); i += 3)
   {
      // compute triangle normal and normalize
      vNormal = glm::triangleNormal(vertices[indices[i + 0]].Position, 
         vertices[indices[i + 1]].Position, vertices[indices[i + 2]].Position);

      // add triangle normal
      pNormals[indices[i + 0]] = pNormals[indices[i + 0]] + vNormal;
      pNormals[indices[i + 1]] = pNormals[indices[i + 1]] + vNormal;
      pNormals[indices[i + 2]] = pNormals[indices[i + 2]] + vNormal;
   } // i

   for (size_t i = 0; i < vertices.size(); i++) {
      vertices[i].Normal = glm::normalize(pNormals[i]);
   }

   SAFE_FREE_ARRAY(pNormals);
}


void Mesh::Draw(const Shader& shader) const
{
   // bind appropriate textures
   unsigned int diffuseNr = 1;
   unsigned int specularNr = 1;
   unsigned int normalNr = 1;
   unsigned int heightNr = 1;
   for (unsigned int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                                        // retrieve texture number (the N in diffuse_textureN)
      std::string number;
      std::string name = textures[i].type;
      if (name == "texture_diffuse")
         number = std::to_string(diffuseNr++);
      else if (name == "texture_specular")
         number = std::to_string(specularNr++); // transfer unsigned int to stream
      else if (name == "texture_normal")
         number = std::to_string(normalNr++); // transfer unsigned int to stream
      else if (name == "texture_height")
         number = std::to_string(heightNr++); // transfer unsigned int to stream

                                              // now set the sampler to the correct texture unit
      glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);
      // and finally bind the texture
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
   }

   // draw mesh
   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

   int indexArraySize;
   glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indexArraySize);
   int nTriangles = indexArraySize / sizeof(unsigned int);
   glDrawElements(GL_TRIANGLES, nTriangles, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);

   // always good practice to set everything back to defaults once configured.
   glActiveTexture(GL_TEXTURE0);
}

void Mesh::Init(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
   std::vector<STexture> textures, bool bComputeNormals) 
{
   this->vertices = vertices;
   this->indices = indices;
   this->textures = textures;

   if (bComputeNormals) {
      ComputeNormals();
   }

   // now that we have all the required data, set the vertex buffers and its attribute pointers.
   SetupMesh();
}

void Mesh::SetupMesh()
{
   // create buffers/arrays
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);
   // load data into vertex buffers
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   // A great thing about structs is that their memory layout is sequential for all its items.
   // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
   // again translates to 3/2 floats which translates to a byte array.
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

   // set the vertex attribute pointers
   // vertex Positions
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
   // vertex normals
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

   // vertex color
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

   // vertex texture coords
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
   // vertex tangent
   glEnableVertexAttribArray(4);
   glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
   // vertex bitangent
   glEnableVertexAttribArray(5);
   glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

   glBindVertexArray(0);
}