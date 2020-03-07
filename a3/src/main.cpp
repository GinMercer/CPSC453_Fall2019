//CPSC453 Assignment03
//Sijia Yin
//30049836
//2019-11-19
//Note: reference--here are some websites i search for some resources.
//      https://blog.csdn.net/hmbxsy/article/details/80509876 the ray tracing principle
//      https://blog.csdn.net/qq_32095699/article/details/86767960 the ray tracing principle
//      https://www.cnblogs.com/qingsunny/archive/2013/02/13/2910692.html
//      https://stackoverflow.com/questions/47858571/ray-tracing-sphere-reflection-bug reflrction

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert> //assert
#include <random>

#include "mat3f.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "image.hpp"
#include "vec2i.hpp"
#include "grid2.hpp"
#include "timer.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "ray_intersect.hpp"

// only do this kinda stuff in main.cpp or *.cpp; never *.h
using namespace math;
using namespace geometry;
using namespace std;

namespace raytracing {

Vec3f normalAt(Vec3f const &p, Triangle const &t) {
  return normal(t);
}

Vec3f normalAt(Vec3f const &p, Sphere const &s) {
  Vec3f n=(p-s.origin)*(1.f/s.radius);
  return n;
}

Vec3f normalAt(Vec3f const & /*p*/, Plane const &p) {
  return p.normal;
}

struct Surface {
  virtual ~Surface() = default;
  virtual Hit intersectSelf(Ray const &ray) const = 0;
  virtual Vec3f normalAtSelf(Vec3f const &p) const = 0;
};

// helper class/function to make, e.g., class Sphere : public Surface
// Wrapping the geometry (e.g., sphere) in a class for intersection
// does not 'pollute' the geometry with inheritence
template <class T> struct Intersect_ : public Surface {
  template <typename... Args>
  Intersect_(Args... args)
      : m_self(std::forward<Args>(args)...) {}

  Hit intersectSelf(Ray const &ray) const { return intersect(ray, m_self); }
  Vec3f normalAtSelf(Vec3f const &p) const { return normalAt(p, m_self); }

  T m_self;
};

template <typename T> std::unique_ptr<Intersect_<T>> makeIntersectable(T t) {
  return std::unique_ptr<Intersect_<T>>(new Intersect_<T>(t));
}

struct ImagePlane {
  using Screen = geometry::Grid2<raster::RGB>;

  Screen screen;
  math::Vec3f origin;
  math::Vec3f u;
  math::Vec3f v;
  float left;   // right symmetric
  float bottom; // top symmetric

  ImagePlane &resolution(uint32_t width, uint32_t height) {
    screen = Screen(width, height);
    return *this;
  }
  ImagePlane &center(Vec3f center) {
    origin = center;
    return *this;
  }
  ImagePlane &uvAxes(Vec3f up, Vec3f right) {
    u = up;
    v = right;
    return *this;
  }
  ImagePlane &dimensions(float width, float height) {
    left = -(0.5f * width);
    bottom = -(0.5f * height);
    return *this;
  }

  math::Vec3f pixelTo3D(math::Vec2f pixel) const {
    using std::abs;

    // shift to center
    pixel += {0.5f, 0.5f};

    float u_x = left + (2.f * abs(left)) * (pixel.x) / screen.width();
    float v_y = bottom + (2.f * abs(bottom)) * (pixel.y) / screen.height();

    return origin + u_x * u + v_y * v;
  }
};

ImagePlane makeImagePlane(Vec3f const &eye, Vec3f const &lookatPosition,
                          Vec3f const &canonicalUp, int xResolution,
                          int yResolution, float width, float height,
                          float nearPlaneDistanace) {
  // make orthonormal basis around eye
  auto gaze = normalized(lookatPosition - eye);

  auto u = gaze ^ canonicalUp;
  u.normalize();

  auto v = u ^ gaze;
  v.normalize();

  // image plane is orthogoanl to camera gaze so use u,v for image plane
  ImagePlane imagePlane;
  // using method chaining to have named parameter/configuation)
  imagePlane.resolution(xResolution, yResolution)
      .center(eye + gaze * nearPlaneDistanace)
      .dimensions(width, height)
      .uvAxes(u, v);

  return imagePlane;
}

using s_ptr = std::unique_ptr<Surface>;

//******************************************************************
//******************************************************************
//******************************************************************
Vec3f shadow(Ray ray,
             Hit closest,                         //
             Surface const* surface,              //
             math::Vec3f light,                   //
             std::vector<s_ptr> const &surfaces) {

	constexpr float ambientIntensity = 0.1f;
  constexpr float specularIntensity = 1.0f;

  Vec3f sphereColor(1.0f, 0.8f, 0.89f);
  Vec3f lightColor(1.0f, 1.0f, 1.0f);

  Plane planecolor;
  Hit planecolorhit = intersect(ray,planecolor);
  if(planecolorhit.rayDepth > closest.rayDepth){
    sphereColor = Vec3f(1.0,0.55,0.45);
  }
  //******************************************************************
  //code from assignmenr02 texture_fs.glsl
	Vec3f direction = ray.direction;
  Vec3f origin = ray.origin;
  Vec3f dataposition = origin + direction * closest.rayDepth;    
  Vec3f datanormal = surface->normalAtSelf(dataposition);
  //ambient
	Vec3f ambient = ambientIntensity * lightColor;

  Vec3f norm = normalized(datanormal);
  Vec3f lightDir = normalized(light - dataposition); 
  //diffuse
  float diff = norm* lightDir;
  Vec3f diffuse = diff * lightColor;

  Vec3f viewDir = normalized(origin - dataposition);
  Vec3f reflectDir = (2 * lightDir * datanormal) * datanormal - lightDir;
  //specular
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
  Vec3f specular = specularIntensity * spec * lightColor; 
  //phong
  Vec3f phong =  ambient + diffuse  + specular;
  //******************************************************************
  float lighting = 1.0;
	//get the shadow
	Ray shadowRay(dataposition,lightDir);
	float L = distance(light, dataposition);
  // pointer to closest object
	for (auto const &s : surfaces) {
		auto hit = s->intersectSelf(shadowRay);
		if (hit && (hit.rayDepth < L)){
			if(hit.rayDepth > 0.01){
          lighting = 0.0;
      }
		}
	}
  //get shadow color
	Vec3f colorOut = lighting * Vec3f(phong.x * sphereColor.x, //
                                    phong.y * sphereColor.y, //
                                    phong.z * sphereColor.z);
	return colorOut;
}

//******************************************************************
//******************************************************************
//******************************************************************
Vec3f castRay(Ray ray,
              math::Vec3f eye,                    //
              math::Vec3f light,                  //
              std::vector<s_ptr> const &surfaces) {

  constexpr float reflectIntensity = 0.1f;

  // background color
  Vec3f colorOut(0.1f, 0.1f, 0.1f);
  // find closed object, if any
  Hit closest;
  // pointer to closest object
  Surface const *surface = nullptr;
  //******************************************************
  for (auto const &s : surfaces) {
    auto hit = s->intersectSelf(ray);
    if (hit && (hit.rayDepth < closest.rayDepth)){
      closest = hit;
      surface = s.get();
    }
  }
  //******************************************************
  // if hit get point
  if (surface != nullptr) {
	  //TODO
    colorOut=shadow(ray, closest, surface, light, surfaces);
		
		Vec3f position = ray.origin + ray.direction * closest.rayDepth;
		Vec3f normal = surface->normalAtSelf(position);
		Vec3f reflectDir = ray.direction-2*(ray.direction*normal)*normal;

		ray = Ray(position, reflectDir);
    //******************************************************
    // pointer to closest object
    for (auto const &s : surfaces) {
			auto hit = s->intersectSelf(ray);
			if (hit && (hit.rayDepth < closest.rayDepth)){
        if(hit.rayDepth>0.01){
          closest = hit;
				  surface = s.get();
          colorOut = colorOut + reflectIntensity*shadow(ray, closest, surface, light, surfaces);
        }
			}
		}
  }
  return colorOut;
}

//******************************************************************
//******************************************************************
//******************************************************************
void render(ImagePlane &imagePlane, //
            math::Vec3f eye,        // all below could be in 'scene' object
            math::Vec3f light,      //
            std::vector<s_ptr> const &surfaces) {

  // Standard mersenne_twister_engine seeded
  thread_local std::mt19937 gen(0);
  auto sampleRange = [](float a, float b) {
    using distrubution = std::uniform_real_distribution<>;
    return distrubution(a, b)(gen);
  };

  // question? if I switch the order here of rows vs. coloumns is it faster?
  // slower? why?
  for (int32_t y = 0; y < imagePlane.screen.height(); ++y) {
    for (int32_t x = 0; x < imagePlane.screen.width(); ++x) {

      math::Vec2f pixel(x, y);
      auto pixel3D = imagePlane.pixelTo3D(pixel);
      auto direction = normalized(pixel3D - eye);
      auto bias = 1e-4f;
      auto p = pointOnLne(eye, direction, bias);
      Ray r(p, direction);

      auto colorOut = castRay(r, eye, light, surfaces);

      // correct to quantiezed error
      // (i.e., removes banded aliasing when converting to 8bit RGB)
      constexpr float halfStep = 1.f / 512;
      colorOut = raster::quantizedErrorCorrection(
          colorOut, sampleRange(-halfStep, halfStep));

      imagePlane.screen({x, y}) = raster::convertToRGB(colorOut);
    }
  }
}
} // namespace

int main() {
  // setup camera and imagePlane
  Vec3f light{20, 15, 10};
  Vec3f eye{0.f, 5.f, 10.f};
  Vec3f lookat{0.f, 0.f, 0.f};
  Vec3f up{0.f, 1.f, 0.f};
  int resolutionX = 1000;
  int resolutionY = 1000;
  float planeWidth = 50.f;
  float planeHeight = 50.f;
  float focalDist = 50.f;

  using namespace raytracing;

  auto imagePlane = makeImagePlane(eye, lookat, up,          //
                                   resolutionX, resolutionY, //
                                   planeWidth, planeHeight,  //
                                   focalDist);

  // setup scene
  std::vector<s_ptr> surfaces;

  Plane p;
  surfaces.push_back(makeIntersectable(p));

  surfaces.push_back(makeIntersectable(Sphere({-0.3f, 2.1f, 1.34f}, 0.18f)));
  surfaces.push_back(makeIntersectable(Sphere({ 0.3f, 2.1f, 1.34f}, 0.18f)));
  surfaces.push_back(makeIntersectable(Sphere({ 0.0f, 1.5f, 0.0f}, 1.5f)));
  surfaces.push_back(makeIntersectable(Sphere({ 0.0f, 1.6f, 1.32f}, 0.4f)));

  Triangle testTriangle{Vec3f{-5,0,0}, Vec3f{-2,0,-5}, Vec3f{-3.5,4,-1} };
  surfaces.push_back(makeIntersectable(testTriangle));

  // render that thing...
  temporal::Timer timer(true);

  render(imagePlane, eye, light, surfaces);

  std::cout << "Time elapsed: " << std::fixed << timer.minutes() << " min.\n";

  raster::write_screen_to_file("./test.png", imagePlane.screen);

  return EXIT_SUCCESS;
}
