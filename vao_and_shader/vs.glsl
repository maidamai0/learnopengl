#version 400
in vec3 vp;
void main() {
  gl_Position = vec4(vp.x,vp.y+4, vp.z-2, 1.0);
}
