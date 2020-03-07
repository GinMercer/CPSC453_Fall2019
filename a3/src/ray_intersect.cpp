/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */

#include "vec3f.hpp"
#include "ray_intersect.hpp"
#include "mat3f.hpp"
using namespace math;

namespace geometry {

Hit::operator bool() const { return didIntersect; }

Hit intersect(Ray const &ray, Triangle const &triangle) {
  Hit hit;
  //chapter 4.4.2 Ray-Triangle Intersection
  Vec3f origin = ray.origin;
  Vec3f direction = ray.direction;
  Vec3f a = triangle.a();
	Vec3f b = triangle.b();
  Vec3f c = triangle.c();

  Mat3f A = {a.x-b.x, a.x-c.x, direction.x,
             a.y-b.y, a.y-c.y, direction.y,
             a.z-b.z, a.z-c.z, direction.z};

  Mat3f B = {a.x-origin.x, a.x-c.x, direction.x,
             a.y-origin.y, a.y-c.y, direction.y,
             a.z-origin.z, a.z-c.z, direction.z};

  Mat3f C = {a.x-b.x, a.x-origin.x, direction.x,
             a.y-b.y, a.y-origin.y, direction.y,
             a.z-b.z, a.z-origin.z, direction.z};

  Mat3f D = {a.x-b.x, a.x-c.x, a.x-origin.x,
             a.y-b.y, a.y-c.y, a.y-origin.y,
             a.z-b.z, a.z-c.z, a.z-origin.z};

  float d=determinant(A);
  float e=determinant(B)/d;
  float f=determinant(C)/d;
  float t=determinant(D)/d;
  //pseudocode from page.79
  if (f <0 ||f>1){
    hit.didIntersect = false;
  }
  else if(e <0 ||e>1.f - f){
    hit.didIntersect = false;
  }else{
    hit.didIntersect = true;
	  hit.rayDepth = t;
  }
	return hit;
}

Hit intersect(Ray const &ray, Sphere const &sphere) {
  Hit hit;
  //TODO
  //chapter 4.4.1 Ray-Sphere Intersection
  Vec3f origin = ray.origin;
  Vec3f direction = ray.direction;
  Vec3f originsphere = sphere.origin;
  float radius = sphere.radius;
  Vec3f e_c = origin - originsphere;

  //At^2 + Bt + C = 0.
  float b = (2*direction)*e_c;
  float c = e_c*e_c - radius*radius;
  float delta = b*b - 4*c;

  if (delta < 1e-5f){
    return hit;
  } else{
    float t1 = (-b - sqrt(delta))/2;
    float t2 = (-b + sqrt(delta))/2;

    float t;
    if(t1<t2){t = t1;
    }else{t = t2;}
    
    hit.didIntersect = true;
    hit.rayDepth = t;
    return hit;
  }
}

Hit intersect(Ray const &r, Plane const &p) {
  Hit hit;

  auto denom = r.direction * p.normal;
  if (std::abs(denom) < 1e-5f)
    return hit;

  auto t = ((p.origin - r.origin) * p.normal) / denom;

  if (t < 0.f)
    return hit;

  hit.didIntersect = true;
  hit.rayDepth = t;

  return hit;
}

} // namespace geometry
