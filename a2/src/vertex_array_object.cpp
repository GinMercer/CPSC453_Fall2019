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

#include "vertex_array_object.hpp"

#include <utility>

namespace opengl {

VertexArrayObject::VertexArrayObject(GLuint id) : m_id(id) {}

VertexArrayObject makeVertexArrayObject() {
  GLuint id = 0;
  glGenVertexArrays(1, &id);
  return VertexArrayObject(id);
}

void VertexArrayObject::bind() const { glBindVertexArray(m_id.name()); }
void VertexArrayObject::unbind() const { glBindVertexArray(0); }

GLuint VertexArrayObject::id() const { return m_id.name(); }

void swap(VertexArrayObject &lhs, VertexArrayObject &rhs) {
  std::swap(lhs.m_id, rhs.m_id);
}

template <> void release_object<VertexArrayObject>(GLuint &name) {
  glDeleteBuffers(1, &name);
}

} // namespace openGL
