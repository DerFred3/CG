#include "Triangle.h"

Triangle::Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2,
                   const Shader& s) {
  // TODO: initialize member variables here
  m_v0 = &v0;
  m_v1 = &v1;
  m_v2 = &v2;
  m_shader = &s;
  m_area = area(m_v0->m_Position, m_v1->m_Position, m_v2->m_Position);
}

void Triangle::draw(Image& image) {
  // TODO:
  // Rasterize the triangle into the image. A possible, yet not very efficient,
  // solution would be to compute for every pixel in the image, whether it is
  // inside this trianhle or not. If the pixel is inside the triangle you
  // should assign a color to the piel using the shader of the triangle
  for (uint32_t y = 0; y < image.height; y++) {
    for (uint32_t x = 0; x < image.width; x++) {
      const Vec3 p = Vec3(x, y, 0.0);
      float &a = *new float(0.0);
      float &b = *new float(0.0);
      float &c = *new float(0.0);
      if (is_inside(p, a, b, c)) {
        float valX = (float)x / (float)image.width;
        float valY = (float)y / (float)image.height;
        
        Vec3 shade = m_shader->shade(*m_v0) * b + m_shader->shade(*m_v1) * c + m_shader->shade(*m_v2) * a;

        image.setValue(x, y, 0, shade.r * 255.0);
        image.setValue(x, y, 1, shade.g * 255.0);
        image.setValue(x, y, 2, shade.b * 255.0);
      }
    }
  }
}

bool Triangle::is_inside(const Vec3& p, float& a, float& b, float& c) {
  a = area(p, m_v0->m_Position, m_v1->m_Position) / m_area;
  if (a <= 0.0 || a >= 1.0) return false;
  
  b = area(p, m_v1->m_Position, m_v2->m_Position) / m_area;
  if (b <= 0.0 || b >= 1.0) return false;
  
  c = area(p, m_v2->m_Position, m_v0->m_Position) / m_area;
  if (c <= 0.0 || c >= 1.0) return false;

  float sum = a + b + c;
  float epsilon = 0.0001;
  if (sum > 1.0 + epsilon || sum < 1.0 - epsilon) return false;

  return true;
}

float area(Vec3 p1, Vec3 p2, Vec3 p3) {
  Vec3 a = p2 - p1;
  Vec3 b = p3 - p1;

  Vec3 cross = Vec3::cross(a, b);
  return 0.5 * cross.length();
}