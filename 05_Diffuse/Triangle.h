#pragma once

#include "Vertex.h"
#include "Image.h"
#include "Shader.h"

class Triangle {
public:
  Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Shader& s);
  void draw(Image& image);

private:
  // TODO: add the appropriate member variables here
  Vertex const *m_v0;
  Vertex const *m_v1;
  Vertex const *m_v2;
  Shader const *m_shader;
  float m_area;

  bool is_inside(const Vec3& v, float& a, float& b, float& c);
};

float area(Vec3 p1, Vec3 p2, Vec3 p3);