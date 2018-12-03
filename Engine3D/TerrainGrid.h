#pragma once

#include "Mesh.h"

class TerrainGrid
{
public:
   TerrainGrid(float width, float depth, unsigned int m, unsigned int n);
   ~TerrainGrid();

   bool GetElevation(const glm::vec3& cameraPosition, float& elevation) const;

   // render the mesh
   void Draw(const Shader& shader) const;

private:
   void CreateGrid(float width, float depth, unsigned int m, unsigned int n);

   bool IsInside(const glm::vec3& pos) const;

private:
   Mesh _grid;

   unsigned int _columns;
   unsigned int _rows;

   glm::vec3 _min;
   glm::vec3 _max;
};

