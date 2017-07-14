uniform vec3 meanInput;
uniform vec3 meanTarget;
uniform vec3 quotient;
uniform sampler2D diffuseSampler;
uniform vec4 lightPosition, sceneAmbient;

varying vec2 oUV0;
varying vec3 N;
varying vec3 v;

float log10(float a){
    return log(a)/log(10);
}

vec3 RGBtoLAlphaBeta(vec3 color) {
  float oneOverRootTwo = 0.70710678118; // 1 / sqrt(2);
  float oneOverRootThree = 0.57735026919; //1 / sqrt(3);
  float oneOverRootSix = 0.40824829046; // 1 / sqrt(6);

  float L = 0.3811 * color.r + 0.5783 * color.g + 0.0402 * color.b;
  float M = 0.1967 * color.r + 0.7244 * color.g + 0.0782 * color.b;
  float S = 0.0241 * color.r + 0.1288 * color.g + 0.8444 * color.b;
  
  L = log(L);
  M = log(M);
  S = log(S);

  float l = oneOverRootThree * L + oneOverRootThree * M + oneOverRootThree * S;
  float alpha = oneOverRootSix * L + oneOverRootSix * M - 2 * oneOverRootSix * S;
  float beta = oneOverRootTwo * L - oneOverRootTwo * M;

  color.r = l;
  color.g = alpha;
  color.b = beta;
  return color;
}

vec3 LAlphaBetaToRGB(vec3 color) {
  float rootTwoOverTwo = 0.70710678118; //sqrt(2) / 2;
  float rootThreeOverThree = 0.57735026919; // sqrt(3) / 3;
  float rootSixOverSix = 0.40824829046; // sqrt(6) / 6;
  
  float L = rootThreeOverThree * color.r + rootSixOverSix * color.g + rootTwoOverTwo * color.b;
  float M = rootThreeOverThree * color.r + rootSixOverSix * color.g - rootTwoOverTwo * color.b;
  float S = rootThreeOverThree * color.r - 2 * rootSixOverSix * color.g;

  L = exp(L);
  M = exp(M);
  S = exp(S);

  float R =  4.4679 * L - 3.5873 * M + 0.1193 * S;
  float G = -1.2186 * L + 2.3809 * M - 0.1624 * S;
  float B =  0.0497 * L - 0.2439 * M + 1.2045 * S;
  
  color.r = R;
  color.g = G;
  color.b = B;
  
  return color;
}

void main()
{
	// directional light direction
	vec3 lightDirection = normalize(lightPosition.xyz);
   
	// angle between normal and light direction
	float _NdotL = max(dot(N, lightDirection), 0.0);
 

	vec3 diffuseTex = texture2D(diffuseSampler, oUV0).rgb;
	
	// Ambient Term
	vec3 ambient = diffuseTex * sceneAmbient.rgb;
	
	// Diffuse term
	vec3 diffuse = _NdotL * diffuseTex;  
	
	vec3 color = vec3(ambient + 0.8 * diffuse);
	
	// Apply color balancing
    color = RGBtoLAlphaBeta(color);
    color = color - meanInput;        
    color = color * quotient;
	color = color + meanTarget;
    color = LAlphaBetaToRGB(color);
    
	gl_FragColor = vec4(color, 1.0);
}