#include <GLApp.h>
#include <cmath>
#include <optional>

class MyGLApp : public GLApp {
public:
  Image image{1024,1024};
  
  MyGLApp() : GLApp{1024,1024,1,"Intersection Demo"} {}
    
  std::optional<Vec3> raySphereIntersect(const Vec3& sphereCenter,
                                         const float& radius,
                                         const Vec3& rayOrigin,
                                         const Vec3& pixelPos) {
    // TODO:
    // Implement a ray/sphere intersection here
    // the sphere is given as sphereCenter and
    // radius, the ray starts at rayOrigin and
    // passes through pixelPos. If no interscation
    // is found, simply return {} otherwise return
    // the interscetion position
    // If the function works correctly, you should
    // see a glossy red sphere, illuminated from
    // the top front
    // Refer to https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    const Vec3 direction = Vec3::normalize(pixelPos - rayOrigin);
    const Vec3 rayToCenter = sphereCenter - rayOrigin;
    const float distanceToCenter = rayToCenter.length();
    const float rayOrigDistPerpCenter = Vec3::dot(rayToCenter, direction);
    const float perpDistToDirection = sqrt(pow(distanceToCenter, 2) - pow(rayOrigDistPerpCenter, 2));

    // Ray misses sphere
    if (perpDistToDirection > radius) {
      return {};
    }

    const float distIntersectToPerpCenter = sqrt(pow(radius, 2) - pow(perpDistToDirection, 2));
    const float t0 = rayOrigDistPerpCenter - distIntersectToPerpCenter;
    
    return rayOrigin + direction * t0;

    // Only first intersection is needed
    // const float t1 = t0 + 2 * distIntersectToPerpCenter;
  }
  
  Vec3 computeLighting(const Vec3& rayOrigin, const Vec3& lightPos, const Vec3& intersectionPoint, const Vec3& normal,
                       const Vec3& specularColor, const Vec3& diffuseColor, const Vec3& ambientColor) {
    const Vec3 viewDir  = Vec3::normalize(rayOrigin-intersectionPoint);
    const Vec3 lightDir = Vec3::normalize(lightPos-intersectionPoint);
    const Vec3 reflectedDir = normal * 2.0f * Vec3::dot(normal, lightDir) - lightDir;
    
    const Vec3 specular = specularColor * pow(std::max(0.0f,Vec3::dot(reflectedDir, viewDir)),16.0f);
    const Vec3 diffuse  = diffuseColor * std::max(0.0f,Vec3::dot(normal, lightDir));
    const Vec3 ambient  = ambientColor;
    return specular + diffuse + ambient;
  }
    
  virtual void init() override {
    GL(glDisable(GL_CULL_FACE));
    GL(glClearColor(0,0,0,0));
    
    const Vec3 lightPos{0.0f,4.0f,0.0f};
    const Vec3 sphereCenter{0.0f, 0.0f, -4.0f};
    const float radius = 2.0f;
    const Vec3 rayOrigin{0.0f, 0.0f, 4.0f};
    const Vec3 topLeftCorner{-2.0f, 2.0f, 0.0f}, topRightCorner{2.0f, 2.0f, 0.0f};
    const Vec3 bottomLeftCorner{-2.0f, -2.0f, 0.0f}, bottomRightCorner{2.0f, -2.0f, 0.0f};
    const Vec3 deltaX = (topRightCorner-topLeftCorner)/ float(image.width);
    const Vec3 deltaY = (topRightCorner-bottomRightCorner)/ float(image.height);
    
    for (uint32_t y = 0;y< uint32_t(image.height);++y) {
      for (uint32_t x = 0;x< uint32_t(image.width);++x) {
        const Vec3 pixelPos = bottomLeftCorner + deltaX*float(x) + deltaY*float(y);
        const std::optional<Vec3> intersection = raySphereIntersect(sphereCenter, radius, rayOrigin, pixelPos);
        Vec3 color;
        if (intersection) {
          const Vec3 normal=(*intersection - sphereCenter) / radius;
          color = computeLighting(rayOrigin, lightPos, *intersection, normal,
                                             Vec3{1.0f,1.0f,1.0f}, Vec3{1.0f,0.0f,0.0f}, Vec3{0.1f,0.0f,0.0f});
        } else {
          color = Vec3{0.0f,0.0f,0.0f};
        }
        image.setNormalizedValue(x,y,0,color.r);
        image.setNormalizedValue(x,y,1,color.g);
        image.setNormalizedValue(x,y,2,color.b);
        image.setValue(x,y,3,255);
      }
    }
  }
    
  virtual void draw() override {
    GL(glClear(GL_COLOR_BUFFER_BIT));
    drawImage(image);
  }

} myApp;

int main(int argc, char ** argv) {
  myApp.run();
  return EXIT_SUCCESS;
}  
