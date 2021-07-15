#version 330 core

// ____ CONSTANTS ____
const uint MAX_SHADOWS = 15u;


// ____ DATATYPES ____
struct Shadow
{
	bool active;
	mat4 viewProjectionMatrix;
	sampler2D shadowMap;
};


// ____ GLOBAL VARIABLES ____
// Input data from the vertex shader
in vec3 vsPosition;

// Uniform variables
uniform sampler2D uDepthTexture;
uniform mat4 uInvCameraViewProjectionMatrix;
uniform Shadow uShadows[MAX_SHADOWS];

// Output data
layout (location = 0) out vec4 oColor;


// ____ FUNCTION DEFINITIONS ____
/*  Returns the location in world space of the vertex located at the given
 * position in the depth buffer */
vec3 decodeLocation(vec2 texCoords)
{
	vec4 clipSpaceLocation;
	clipSpaceLocation.xy = 2.0 * texCoords - 1.0;
	clipSpaceLocation.z = 2.0 * texture(uDepthTexture, texCoords).r - 1.0;
	clipSpaceLocation.w = 1.0;
	vec4 homogenousLocation = uInvCameraViewProjectionMatrix * clipSpaceLocation;
	return homogenousLocation.xyz / homogenousLocation.w;
}


/* Calculates wether the given point is in shadow or not */
float calculateShadow(uint shadowIndex, vec3 position, vec3 normal)
{
	vec4 shadowNormal = uShadows[shadowIndex].viewProjectionMatrix * vec4(normal, 1.0);
	vec4 shadowPosition = uShadows[shadowIndex].viewProjectionMatrix * vec4(position, 1.0);

	vec3 projCoords = shadowPosition.xyz / shadowPosition.w;	// Perspective divide
	projCoords = projCoords * 0.5 + 0.5;						// [0,1] range

	vec3 lightDirection = -vec3(shadowPosition);
	float bias = max(0.005 * (1.0 - dot(normal, lightDirection)), 0.0005);

	float shadow = 0.0;
	float currentDepth = projCoords.z;
	vec2 texelSize = 1.0 / textureSize(uShadows[shadowIndex].shadowMap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(uShadows[shadowIndex].shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth? 1.0 : 0.0;
		}
	}
	shadow = (projCoords.z > 1.0)? 0.0 : shadow / 9.0;

	return shadow;
}


/** Stores the given float as a 8 bit value inside the oColor output */
void storeColor(uint i, float f)
{
	uint value = uint(256.0 * f);

	uint q = i / 4u;
	uint r = i % 4u;
	uint insidePos = (r == 0u)? 16777216u : (r == 1u)? 65536u : (r == 2u)? 256u : 1u;

	if (q == 0u) {
		oColor.r = float(uint(oColor.r) + insidePos * value);
	}
	else if (q == 1u) {
		oColor.g = float(uint(oColor.g) + insidePos * value);
	}
	else if (q == 2u) {
		oColor.b = float(uint(oColor.b) + insidePos * value);
	}
	else {
		oColor.a = float(uint(oColor.a) + insidePos * value);
	}
}


// ____ MAIN PROGRAM ____
void main()
{
	// Extract the data from the depth buffer
	vec2 texCoords = (0.5 * vsPosition + 0.5).xy;
	vec3 position = decodeLocation(texCoords);

	vec2 texOffset = 1.0 / textureSize(uDepthTexture, 0);
	vec3 rightPos = decodeLocation(texCoords + vec2(texOffset.x, 0.0));
	vec3 topPos = decodeLocation(texCoords + vec2(0.0, texOffset.y));
	vec3 normal = normalize(cross(rightPos - position, topPos - position));

	// Calculate the shadows
	float shadow = 0.0;
	for (uint i = 0u; i < MAX_SHADOWS; ++i) {
		if (uShadows[i].active) {
			shadow += calculateShadow(i, position, normal);
			storeColor(i, shadow);
		}
	}
}
