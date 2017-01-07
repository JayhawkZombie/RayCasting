uniform float offset = 1.0/300;
uniform sampler2D SCENE;
uniform float radius;

float BlurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

vec4 KernelMethod(vec4 color)
{
  vec2 offsets[9] = vec2[](
      vec2(-offset, offset),  // top-left
      vec2(0.0f,    offset),  // top-center
      vec2(offset,  offset),  // top-right
      vec2(-offset, 0.0f),    // center-left
      vec2(0.0f,    0.0f),    // center-center
      vec2(offset,  0.0f),    // center-right
      vec2(-offset, -offset), // bottom-left
      vec2(0.0f,    -offset), // bottom-center
      vec2(offset,  -offset)  // bottom-right    
  );

  float kernel[9] = BlurKernel;

  vec3 sampletex[9];
  for (int i = 0; i < 9; i++) {
    sampletex[i] = vec3(texture2D(SCENE, gl_TexCoord[0].st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for (int i = 0; i < 9; i++) {
    col += sampletex[i] * kernel[i];
  }

  vec4 c = vec4(1.0, 1.0, 1.0, 1.0);
  c.rgb = col;
  return c;
}


void main()
{
  vec4 pixel = texture2D(SCENE, gl_TexCoord[0].st);
  pixel *= gl_Color;

  vec4 _pixel = KernelMethod(pixel);
  gl_FragColor = _pixel;
}