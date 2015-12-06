// Utilized and adapted code from https://github.com/opengl-tutorials/ogl/ to help with shaders & camera movement.
#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif