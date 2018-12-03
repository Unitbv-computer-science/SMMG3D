#include "stdafx.h"
#include "TerrainGrid.h"
#include "GeometryGenerator.h"


TerrainGrid::TerrainGrid(float width, float depth, unsigned int m, unsigned int n)
{
   CreateGrid(width, depth, m, n);
}


TerrainGrid::~TerrainGrid()
{
}

void TerrainGrid::CreateGrid(float width, float depth, unsigned int m, unsigned int n)
{
   GeometryGenerator geometryGenerator;
   _grid = geometryGenerator.CreatePerlinHill(width, depth, m, n);

   _min = glm::vec3(0, MinElevation, 0);
   _max = glm::vec3(width, MaxElevation, depth);

   _columns = n;
   _rows = m;
}

// render the mesh
void TerrainGrid::Draw(const Shader& shader) const
{
   _grid.Draw(shader);
}

bool TerrainGrid::GetElevation(const glm::vec3& position, float& elevation) const
{
   if (IsInside(position))
   {
      glm::vec3 localPos = position - _min;

      int width  = (int)fabs(_max.x - _min.x);
      int length = (int)fabs(_max.z - _min.z);

      int x = static_cast<int>((position.x*width) / _columns);
      int z = static_cast<int>((position.z*length) / _rows);

      float resX = static_cast<float>(fmod((position.x*width) / _columns, 1.0f));
      float resZ = static_cast<float>(fmod(((position.z*length) / _rows), 1.0f));

      float y1 = _grid.vertices[x + z * width].Position.y + 
         resZ * ((int)(_grid.vertices[x + (z + 1)*width].Position.y) - _grid.vertices[x + z * width].Position.y);
      float y2 = _grid.vertices[x + 1 + z * width].Position.y +
         resZ * ((int)(_grid.vertices[x + 1 + (z + 1)*width].Position.y) - _grid.vertices[x + 1 + z * width].Position.y);

      elevation = y1 + (y2 - y1)*resX;

      return true;
   }

   return false;
}

bool TerrainGrid::IsInside(const glm::vec3& pos) const
{
   if (pos.x < _min.x || pos.x > _max.x)
      return false;
   if (pos.y < _min.y || pos.y > _max.y)
      return false;
   if (pos.z < _min.z || pos.z > _max.z)
      return false;

   return true;
}