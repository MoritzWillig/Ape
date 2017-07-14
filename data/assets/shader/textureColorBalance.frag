uniform vec3 meanInput;
uniform vec3 meanTarget;
uniform vec3 quotient;
uniform sampler2D diffuse;
uniform vec4 lightPosition, sceneAmbient;

varying vec2 oUV0;
varying vec3 N;
varying vec3 v;

float log10(float a){
    return log(a)/log(10);
}

void RGBtoLAlphaBeta(vec3 color) {
  float oneOverRootTwo = 1 / sqrt(2);
  float oneOverRootThree = 1 / sqrt(3);
  float oneOverRootSix = 1 / sqrt(6);

  float L = 0.3811 * color.r + 0.5783 * color.g + 0.0402 * color.b;
  float M = 0.1967 * color.r + 1.1834 * color.g + 0.0782 * color.b;
  float S = 0.0241 * color.r + 0.1288 * color.g + 0.8444 * color.b;

  // Workaround to fix undefined log of 0
  if (L == 0)
    L = 1 / 255.0;
  if (M == 0)
    M = 1 / 255.0;
  if (S == 0)
    S = 1 / 255.0;
    
  L = log10(L);
  M = log10(M);
  S = log10(S);

  float l = oneOverRootThree * L + oneOverRootThree * M + oneOverRootThree * S;
  float alpha = oneOverRootSix * L + oneOverRootSix * M - 2 * oneOverRootSix * S;
  float beta = oneOverRootTwo * L - oneOverRootTwo * M;

  color.r = l;
  color.g = alpha;
  color.b = beta;
}

void LAlphaBetaToRGB(vec3 color) {
  float rootTwoOverTwo = sqrt(2) / 2;
  float rootThreeOverThree = sqrt(3) / 3;
  float rootSixOverSix = sqrt(6) / 6;
  
  float L = rootThreeOverThree * color.r + rootSixOverSix * color.g + rootTwoOverTwo * color.b;
  float M = rootThreeOverThree * color.r + rootSixOverSix * color.g - rootTwoOverTwo * color.b;
  float S = rootThreeOverThree * color.r - 2 * rootSixOverSix * color.g;

  L = pow(L, 10);
  M = pow(M, 10);
  S = pow(S, 10);

  float R =  4.4679 * color.r - 3.5873 * color.g + 0.1193 * color.b;
  float G = -1.2186 * color.r + 2.3809 * color.g - 0.1624 * color.b;
  float B =  0.0241 * color.r - 0.2439 * color.g + 1.2045 * color.b;

  color.r = R;
  color.g = G;
  color.b = B;
}

void main()
{
	// directional light direction
	vec3 lightDirection = normalize(lightPosition.xyz);
   
	// angle between normal and light direction
	float _NdotL = max(dot(N, lightDirection), 0.0);
 

	vec3 diffuseTex = texture2D(diffuse, oUV0).rgb;
	
	// Ambient Term
	vec3 ambient = diffuseTex * sceneAmbient.rgb;
	
	// Diffuse term
	vec3 diffuse = _NdotL * diffuseTex;  
	
	vec3 color = vec3(ambient + diffuse);
	
	// Apply color balancing
    RGBtoLAlphaBeta(color);
    color = color.rgb - meanInput;        
    color = color.rgb * quotient;
	color = color + meanTarget;
    LAlphaBetaToRGB(color);
    
	gl_FragColor = vec4(color, 1.0);
}