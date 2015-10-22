#version 330

struct MaterialProperties {
	vec4 diffuse; 				// what part of diffuse light is scattered
	vec4 ambient; 				// what part of ambient light is reflected
	vec4 specular; 				// what part of specular light is scattered
	vec4 emissive; 				// light produced by the material
	float shininess;			// exponent for sharpening specular reflection
	int texCount;
};

struct LightProperties {
	bool isEnabled;				// true to apply this light in this invocation

	bool isLocal;				// true for a point light or a spotlight,
								// false for a positional light

	bool isSpot;				// true if the light is a spotlight

	vec4 ambient;				// light’s contribution to ambient light
	vec4 diffuse;
	vec4 specular;

	vec3 position;				// location of light, if is Local is true,
								// otherwise the direction toward the light

	vec3 halfVector;			// direction of highlights for directional light
	vec3 coneDirection;			// spotlight attributes

	float spotCosCutoff;
	float spotExponent;

	float constantAttenuation;	// local light attenuation coefficients
	float linearAttenuation;
	float quadraticAttenuation;
};

const int Maxlights = 10;
uniform MaterialProperties mat;
uniform LightProperties lights[Maxlights];

in vec3 Normal;
in vec4 Position;
in vec3 EyeDirection;


out vec4 colorOut;

void main() {
	vec4 spec = vec4(0.0);
	vec3 normal = normalize(Normal);
	vec3 eye = normalize(EyeDirection);
	colorOut = vec4(0.0);
	for (int light = 0; light < Maxlights; light++) {

		if (! lights[light].isEnabled) 
			continue;

		float attenuation = 1.0;
		vec3 lightDir = normalize(lights[light].position - vec3(Position));
		float lightDistance = length(lightDir);
		float intensity = max(dot(lightDir, normal), 0.0);
		attenuation = 1.0 /
						(lights[light].constantAttenuation
							+ lights[light].linearAttenuation * lightDistance
							+ lights[light].quadraticAttenuation * lightDistance
							* lightDistance);
		if (intensity > 0.0) {

			vec3 halfDir = normalize(lightDir + eye);
			float intSpec = max(dot(halfDir, normal), 0.0);
			spec = mat.specular * pow(intSpec, mat.shininess) * lights[light].specular * attenuation;
		}
		colorOut += max(intensity * mat.diffuse * attenuation + spec, mat.ambient * lights[light].ambient * attenuation);
	}
}