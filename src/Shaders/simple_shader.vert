#version 450

//input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

//output
layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push 
{
	mat4 transform;
	mat4 normalMatrix;
} push;

const vec3 lightDir = normalize(vec3(1.0, -3.0, -3.0));
const float ambient = 0.1;

void main()
{
	gl_Position = push.transform * vec4(position, 1.0);

	// mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
	// vec3 normalWorldSpace = normalize(normalMatrix * normal);
	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

	float intensity = max(dot(normalWorldSpace, lightDir), ambient);
	fragColor = color * intensity;
}
