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

const int Maxlights = 1;
uniform MaterialProperties mat;
uniform LightProperties lights[Maxlights];

in vec3 Normal;
in vec4 Position;
in vec3 EyeDirection;

out vec4 colorOut;

void main() {

	vec4 scatteredLight = vec4(0.0); // or, to a global ambient light
	vec4 reflectedLight = vec4(0.0);
	/*vec3 LightContribution = vec3(0.0);
	vec3 ambient_term = vec3(0.0);
	vec3 diffuse_term = vec3(0.0);
	vec3 specular_term = vec3(0.0);*/

	// loop over all the lights
	for (int light = 0; light < Maxlights; light++) {

		if (! lights[light].isEnabled) 
			continue;

		vec3 halfVector;
		vec3 lightDirection = lights[light].position;

		float attenuation = 1.0;
		// for local lights, compute per-fragment direction,
		// halfVector, and attenuation

		if (lights[light].isLocal) {
			lightDirection = lightDirection - vec3(Position);
			float lightDistance = length(lightDirection);
			lightDirection = lightDirection / lightDistance;
			attenuation = 1.0 /
							(lights[light].constantAttenuation
								+ lights[light].linearAttenuation * lightDistance
								+ lights[light].quadraticAttenuation * lightDistance
								* lightDistance);
			if (lights[light].isSpot) {
				float spotCos = dot(lightDirection, -lights[light].coneDirection);

				if (spotCos < lights[light].spotCosCutoff)
					attenuation = 0.0;
				else
					attenuation *= pow(spotCos, lights[light].spotExponent);
			}

			halfVector = normalize(lightDirection + EyeDirection);
		}
		else {
			halfVector = lights[light].halfVector;
		}

		float diffuse = max(0.0, dot(Normal, lightDirection));
		float specular = max(0.0, dot(Normal, halfVector));
		if (diffuse == 0.0)
			specular = 0.0;
		else
		specular = pow(specular, mat.shininess);

		// Accumulate all the lights’ effects
		scatteredLight += lights[light].ambient * mat.ambient * attenuation + lights[light].diffuse * mat.diffuse * diffuse * attenuation;
		reflectedLight += lights[light].specular * mat.specular * specular * attenuation;

		/*ambient_term += lights[light].ambient * mat.ambient;
		diffuse_term += lights[light].diffuse * mat.diffuse * diffuse;
		specular_term += lights[light].specular * mat.specular * specular;

		LightContribution += attenuation * (ambient_term + diffuse_term + specular_term); */
	}
	vec4 rgb = mat.emissive + scatteredLight + reflectedLight;
	colorOut = rgb;
	//colorOut = Color;
}