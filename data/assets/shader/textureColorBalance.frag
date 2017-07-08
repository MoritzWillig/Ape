uniform vec3 meanInput;
uniform vec3 meanTarget;
uniform vec3 varianceInput;
uniform vec3 varianceTarget;
uniform sampler2D diffuse;
uniform vec4 lightPosition, sceneAmbient;

varying vec2 oUV0;
varying vec3 N;
varying vec3 v;

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
	
	vec3 quot = varianceTarget / varianceInput;
    //vec3 color = texture2D(diffuse, oUV0).rgb - meanInput;        
    color = color.rgb * quot;
	color = color + meanTarget;
	gl_FragColor = vec4(color, 1.0);
	//gl_FragColor = vec4(lightPosition);
}