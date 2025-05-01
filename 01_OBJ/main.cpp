#include <GLApp.h>
#include <OBJFile.h>
#include <Mat4.h>

class MyGLApp : public GLApp {
public:
  double angle{0.0};
  std::vector<float> data;
  
  MyGLApp() : GLApp{640,480,1,"Shared vertices to explicit representation demo"} {}
  
  int16_t selection;

  virtual void init() override {
    loadBunny();
    selection = 0;
  }
  
  virtual void animate(double animationTime) override {
    angle = animationTime*30;
  }
  
  virtual void draw() override {
    GL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
    setDrawProjection(Mat4::perspective(45, glEnv.getFramebufferSize().aspect(), 0.0001f, 100.0f));
    setDrawTransform(Mat4::lookAt({0,0,2},{0,0,0},{0,1,0}) * Mat4::rotationY(float(angle)));
    drawTriangles(data, TrisDrawType::LIST, false, true);
  }
  
  virtual void keyboardChar(unsigned int key) override {
    // 32 = spacebar
    if (key != 32) {
       return;
    }
    if (selection % 2 == 0) {
       loadCube();
    }
    else {
       loadBunny();
    }
    selection++;
  }

  void loadBunny() {
      loadObject("bunny.obj");
  }

  void loadCube() {
      loadObject("cube.obj");
  }

  void loadObject(const char *objectName) {
      GL(glDisable(GL_CULL_FACE));
      GL(glEnable(GL_DEPTH_TEST));
      GL(glClearColor(0,0,0,0));
      data.clear();

      const OBJFile m{objectName, true};

      for (size_t i = 0; i < m.indices.size(); i++) {
          for (size_t idx_triangle = 0; idx_triangle < 3; idx_triangle++) {
              for (size_t idx_vert = 0; idx_vert < 3; idx_vert++) {
                  data.push_back(m.vertices[m.indices[i][idx_triangle]][idx_vert]);
              }
              // Just white
              data.push_back(1.0f); data.push_back(1.0f); data.push_back(1.0f); data.push_back(1.0f);
              for (size_t idx_norm = 0; idx_norm < 3; idx_norm++) {
                  data.push_back(m.normals[m.indices[i][idx_triangle]][idx_norm]); 
              }
          }
      }
  }

} myApp;

int main(int argc, char ** argv) {
  myApp.run();
  return EXIT_SUCCESS;
}
