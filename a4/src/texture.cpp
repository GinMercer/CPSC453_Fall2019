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

#include "texture.hpp"

#include <iostream>

namespace opengl {

Texture generateTexture() {
  GLuint name = 0;
  glGenTextures(1, &name);
  return Texture(name);
}

GLuint Texture::id() const { return m_object.name(); }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, id()); }

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

template <> void release_object<Texture>(GLuint &name) {
  glDeleteTextures(1, &name);
}

} // namespace opengl