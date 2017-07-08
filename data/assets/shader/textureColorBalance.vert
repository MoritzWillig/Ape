attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv0;

uniform mat4 worldViewProj;
uniform mat4 worldViewMatrix;

varying vec2 oUV0;
varying vec3 N;
varying vec4 v;

void main()
{
   // vertex normal in world space
   N = normalize(normal);
   
   // vertex position in world space
   v = worldViewMatrix * vec4(position.xyz, 1.0);
   
   gl_Position = worldViewProj * vec4(position.xyz, 1.0);
   oUV0 = uv0;
}