#version 120

uniform vec3 meanInput;
uniform vec3 meanTarget;
uniform vec3 varianceInput;
uniform vec3 varianceTarget;
uniform sampler2D diffuse;

varying vec2 oUV0;

void main()
{
	vec3 quot = varianceTarget / varianceInput;
    vec3 color = texture2D(diffuse, oUV0).rgb - meanInput;        
    color = color.rgb * quot;
	color = color + meanTarget;
	gl_FragColor = vec4(color, 1.0);
	//gl_FragColor = texture2D(diffuse, oUV0);
}