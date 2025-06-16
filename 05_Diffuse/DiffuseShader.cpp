#include "DiffuseShader.h"

DiffuseShader::DiffuseShader(const Vec3& light, const Vec3& light_diffuse_color) {
  // TODO: initialize member variables here
  m_lightPos = light;
  m_lightColor = light_diffuse_color;
}

Vec3 DiffuseShader::shade(Vertex surface) const {
  /* TODO:
   Implement this method and the rest of this class for the assignment
   This method should compute and return color of the given vertex using
   diffuse illumination.
   */
  Vec3 lightDir = m_lightPos - surface.m_Position;
  float NdotL = Vec3::dot(Vec3::normalize(surface.m_Normal), Vec3::normalize(lightDir));

  // just a dummy return value, needs to be replaced with the right one
  Vec3 output = surface.m_Material->color_diffuse * NdotL * m_lightColor;
  return Vec3::clamp(output, 0.0, 1.0);
}
