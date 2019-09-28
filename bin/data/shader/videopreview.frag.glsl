#version 440

uniform sampler2DRect u_videoTexture;
uniform float u_scaleFactor;

out vec4 fragColor;

void main() {
  vec2 texCoord = vec2(gl_FragCoord.x, 1920 - gl_FragCoord.y);
  texCoord *= vec2(1.0 / u_scaleFactor);
  texCoord.x += 1080 / 4;
  vec3 video = texture2DRect(u_videoTexture, texCoord).rgb;
  fragColor = vec4(video, 1);
}
