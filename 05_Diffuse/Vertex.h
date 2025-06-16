#pragma once
#include "Material.h"

class Vertex {
public:
  // TODO: add the appropriate memeber variables here
  Vec3 m_Position;
  Material const *m_Material;
  Vec3 m_Normal;

  Vertex(const Vec3& position, const Material& material, const Vec3& normal) {
    // TODO: initialize member variables here
    m_Position = position;
    m_Material = &material;
    m_Normal = normal;
  }

  Vertex(const Vec3& position, const Material& material)
  : Vertex(position, material, {0, 0, 1}) {
    m_Position = position;
    m_Material = &material;
  }

  Vertex(const Vec3& position)
  : Vertex(position, {{1, 1, 1}}) {
    m_Position = position;
  }

  Vertex()
  : Vertex({0, 0, 0}) 
  {}

};
