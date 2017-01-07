#version 120

uniform sampler2D MaskTexture;
uniform sampler2D SceneTexture;

uniform float MinimumIntensity;

uniform vec4 LightHue;
uniform float HueIntensity;
uniform float MaximumIntensity;

void main()
{
  vec4 color = texture2D(SceneTexture, gl_TexCoord[0].st);
  vec4 maskColor = texture2D(MaskTexture, gl_TexCoord[0].st);

  vec3 rgbcolors = vec3(maskColor.rgb);
  float intensity = length(rgbcolors) * 7;
  intensity = min(MaximumIntensity, max(MinimumIntensity, intensity));

  color += intensity > MinimumIntensity ? color * ( LightHue * HueIntensity * intensity / 2) : color * 0;
  color *= intensity;

  gl_FragColor = color * intensity;
}