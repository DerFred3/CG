#include <iostream>
#include <GLApp.h>
#include <Vec2.h>
#include <Vec4.h>
#include <Mat4.h>

class MyGLApp : public GLApp {
public:
  double sa = 0;
  double ca = 0;
  const size_t maxLineSegments = 100;

  MyGLApp() : GLApp{1024,1024,1,"Spline Demo"} {}
  
  virtual void init() override {
    GL(glDisable(GL_CULL_FACE));
    GL(glDisable(GL_DEPTH_TEST));
    GL(glClearColor(1,1,1,1));
    GL(glEnable(GL_BLEND));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL(glBlendEquation(GL_FUNC_ADD));
  }
  
  virtual void animate(double animationTime) override {
    sa = sin(animationTime);
    ca = cos(animationTime);
  }
   
  void drawPolySegment(const Vec2& p0, const Vec2& p1,
                       const Vec2& p2, const Vec2& p3,
                       const Mat4& g, const Vec4& color) {
    std::vector<float> curve((maxLineSegments+1)*7);
    
    for (size_t i = 0;i<=maxLineSegments;++i) {
     const float t = float(i)/float(maxLineSegments);

      Vec4 x = Vec4{p0.x, p1.x, p2.x, p3.x};
      Vec4 y = {p0.y, p1.y, p2.y, p3.y};

      Vec4 xResult = Vec4{1, t, t * t, t * t * t} * (g * x);
      Vec4 yResult = Vec4{1, t, t * t, t * t * t} * (g * y);

      curve[i*7+0] = xResult[0] + xResult[1] + xResult[2] + xResult[3];  // x
      curve[i*7+1] = yResult[0] + yResult[1] + yResult[2] + yResult[3];  // y
      curve[i*7+2] = 0.0f;  // z  (no need to change)
      
      curve[i*7+3] = color.r;  // red
      curve[i*7+4] = color.g;  // green
      curve[i*7+5] = color.b;  // blue
      curve[i*7+6] = color.a;  // alpha
    }
    drawLines(curve, LineDrawType::STRIP, 3);
  }
 
  void drawHermiteSegment(const Vec2& p0, const Vec2& p1, const Vec2& m0,
                          const Vec2& m1, const Vec4& color) {
    Mat4 g{
      1, 0, 0, 0,
      0, 0, 1, 0,
     -3, 3,-2,-1,
      2,-2, 1, 1
    };
    drawPolySegment(p0,p1,m0,m1,g,color);
    drawPoints({p0.x,p0.y,0,1,0,0,1,
               p0.x+m0.x,p0.y+m0.y,0,0,0,1,1,
               p1.x+m1.x,p1.y-m1.y,0,0,0,1,1,
               p1.x,p1.y,0,1,0,0,1}, 20, true);
  }

  void drawBezierSegmentDeCasteljau(const Vec2& p0, const Vec2& p1,
                                    const Vec2& p2, const Vec2& p3,
                                    const Vec4& color) {
    // TODO SOLUTION 2:
    /*
    (1 − t) ⋅ ((1 − t) ⋅ ((1 − t) ⋅ p0 + t ⋅ p1) + t ⋅ ((1 − t) ⋅ p1 + t ⋅ p2))+
    t ⋅ ((1 − t) ⋅ ((1 − t) ⋅ p1 + t ⋅ p2) + t ⋅ ((1 − t) ⋅ p2 + t ⋅ p3))
    */
    std::vector<float> curve((maxLineSegments+1)*7);
    
    for (size_t i = 0;i<=maxLineSegments;++i) {
     const float t = float(i)/float(maxLineSegments);

      // SUGGESTION: Add <float * Vec2> as possible order for operator*
      Vec2 resultPoint = (1 - t) * ((1 - t) * ((1 - t) * p0 + t * p1) + t * ((1 - t) * p1 + t * p2)) + t * ((1 - t) * ((1 - t) * p1 + t * p2) + t * ((1 - t) * p2 + t * p3));

      curve[i*7+0] = 0.0f;  // x
      curve[i*7+1] = 0.0f;  // y
      curve[i*7+2] = 0.0f;  // z  (no need to change)
      
      curve[i*7+3] = color.r;  // red
      curve[i*7+4] = color.g;  // green
      curve[i*7+5] = color.b;  // blue
      curve[i*7+6] = color.a;  // alpha
    }
    drawLines(curve, LineDrawType::STRIP, 3);
  }

  void drawBezierSegment(const Vec2& p0, const Vec2& p1, const Vec2& p2,
                         const Vec2& p3, const Vec4& color) {
    Mat4 g{
      1, 0, 0, 0,
     -3, 3, 0, 0,
      3,-6, 3, 0,
     -1, 3,-3, 1
    };
    drawPolySegment(p0,p1,p2,p3,g,color);
    drawPoints({p0.x,p0.y,0,1,0,0,1,
               p1.x,p1.y,0,0,0,1,1,
               p2.x,p2.y,0,0,0,1,1,
               p3.x,p3.y,0,1,0,0,1}, 20, true);
  }
  
  void drawBSplineSegment(const Vec2& p0, const Vec2& p1, const Vec2& p2,
                          const Vec2& p3, const Vec4& color) {
    Mat4 g{
      1/6.0f, 4/6.0f, 1/6.0f, 0/6.0f,
     -3/6.0f, 0/6.0f, 3/6.0f, 0/6.0f,
      3/6.0f,-6/6.0f, 3/6.0f, 0/6.0f,
     -1/6.0f, 3/6.0f,-3/6.0f, 1/6.0f
    };
    drawPolySegment(p0,p1,p2,p3,g,color);
    drawPoints({p0.x,p0.y,0,1,0,0,1,
               p1.x,p1.y,0,0,0,1,1,
               p2.x,p2.y,0,0,0,1,1,
               p3.x,p3.y,0,1,0,0,1}, 20, true);
  }
  
  virtual void draw() override {
    GL(glClear(GL_COLOR_BUFFER_BIT));

    {
      setDrawTransform(Mat4::translation(0.0f,0.7f,0.0f));
      const Vec2 p0{-0.5f,0.0f};
      const Vec2 m0{float(sa)*0.2f,float(ca)*0.2f};
      const Vec2 m1{0.0f,-0.2f};
      const Vec2 p1{0.5f,0.0f};
      drawHermiteSegment(p0,p1,m0,m1,{0.0f,0.0f,0.0f,1.0f});
    }
    

    {
      setDrawTransform(Mat4::translation(0.0f,0.0f,0.0f));
      const Vec2 p0{-0.5f,0.0f};
      const Vec2 p1{float(sa)*0.2f-0.5f,float(ca)*0.2f};
      const Vec2 p2{0.5f,0.2f};
      const Vec2 p3{0.5f,0.0f};
      drawBezierSegment(p0,p1,p2,p3,{0.0f,0.0f,0.0f,1.0f});
    }
   
    {
      setDrawTransform(Mat4::translation(0.0f,-0.2f,0.0f));
      const Vec2 p0{-0.5f,0.0f};
      const Vec2 p1{float(sa)*0.2f-0.5f,float(ca)*0.2f};
      const Vec2 p2{0.5f,0.2f};
      const Vec2 p3{0.5f,0.0f};
      drawBezierSegmentDeCasteljau(p0,p1,p2,p3,{0.0f,0.0f,0.0f,1.0f});
    }

    {
      setDrawTransform(Mat4::translation(0.0f,-0.7f,0.0f));
      const Vec2 p0{-0.5f,0.0f};
      const Vec2 p1{float(sa)*0.2f-0.5f,float(ca)*0.2f};
      const Vec2 p2{0.5f,0.2f};
      const Vec2 p3{0.5f,0.0f};
      drawBSplineSegment(p0,p0,p0,p1,{1.0f,0.0f,0.0f,1.0f});
      drawBSplineSegment(p0,p0,p1,p2,{0.0f,1.0f,0.0f,1.0f});
      drawBSplineSegment(p0,p1,p2,p3,{0.0f,0.0f,1.0f,1.0f});
      drawBSplineSegment(p1,p2,p3,p3,{0.0f,1.0f,1.0f,1.0f});
      drawBSplineSegment(p2,p3,p3,p3,{1.0f,0.0f,1.0f,1.0f});
    }
  }
} myApp;

int main(int argc, char ** argv) {
  myApp.run();
  return EXIT_SUCCESS;
}  
