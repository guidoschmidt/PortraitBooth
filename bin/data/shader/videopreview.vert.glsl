#version 440

in vec3 position;
in vec3 uv;

void main() {
  gl_Position = vec4(position, 1.0); 
}
