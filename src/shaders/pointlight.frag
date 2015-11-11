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

	vec3 position_point;
	vec4 direction;
	vec4 position;				// location of light, if is Local is true,
								// otherwise the direction toward the light

	vec3 halfVector;			// direction of highlights for directional light

	float spotCosCutoff;
	float spotExponent;

	float constantAttenuation;	// local light attenuation coefficients
	float linearAttenuation;
	float quadraticAttenuation;
};

const int Maxlights = 9;
uniform MaterialProperties mat;
uniform LightProperties lights[Maxlights];
uniform bool useTextures;
uniform bool useLights;
uniform bool writingMode;
uniform bool particleMode;
uniform int texMode;

uniform int fogState;
uniform int fogMode;
uniform vec3 fogColor;
uniform float fogDensity;

uniform sampler2D texmap1;
uniform sampler2D texmap2;

in vec3 Normal;
in vec4 Position;
in vec3 EyeDirection;
in vec2 tex_coord;

out vec4 colorOut;

vec3 colorFogFunction(in vec3 frag_rgb_in, in vec3 pos) {
	float dist = length(pos);
	float fogAmount = 0;
	if (fogMode == 0) {
		fogAmount = (15 - dist)/(15 - 7.5);
	}
	else if (fogMode == 1) {
		fogAmount = 1.0 / exp(dist * fogDensity);
	}
	else if (fogMode == 2) {
		fogAmount = 1.0 / exp((dist * fogDensity) * (dist * fogDensity));
	}
	fogAmount = clamp(fogAmount, 0.0, 1.0);
	return mix(fogColor, frag_rgb_in, fogAmount);
}

void main() {
	if (!particleMode) {
		if (!writingMode) {
			vec3 scatteredLight = vec3(0.0); // or, to a global ambient light
			vec3 reflectedLight = vec3(0.0);
			vec4 texel, texel1;
			texel = texture(texmap1, tex_coord);
			texel1 = texture(texmap2, tex_coord);
			vec4 tex = vec4(1.0);
			if (useTextures) {
				if (texMode == 1) {
					tex = texel * texel1;
				}
				else if (texMode == 2) {
					tex = texel;
				}
			}
			
			// loop over all the lights
			for (int light = 0; light < Maxlights; light++) {

				if (! lights[light].isEnabled) 
					continue;

				vec3 halfVector;
				vec3 lightDirection;
				if (lights[light].isSpot) {
					lightDirection  = vec3(lights[light].position);
				}
				else {
					lightDirection = lights[light].position_point;
				}
				float attenuation = 1.0;

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
						float spotCos = dot(lightDirection, normalize(vec3(-lights[light].direction)));

						if (spotCos < lights[light].spotCosCutoff)
							attenuation = 0.0;
						else
							attenuation *= pow(spotCos, lights[light].spotExponent);
					}

					halfVector = normalize(lightDirection + EyeDirection);
				}
				else {
					lightDirection = normalize(vec3(-lights[light].direction));
					halfVector = normalize(lightDirection + EyeDirection / 2);
				}

				float diffuse = max(0.0, dot(Normal, lightDirection));
				float specular = max(0.0, dot(Normal, halfVector));
				if (diffuse == 0.0)
					specular = 0.0;
				else
				specular = pow(specular, mat.shininess);

				// Accumulate all the lights’ effects
				scatteredLight += lights[light].diffuse.rgb * mat.ambient.rgb * attenuation + lights[light].ambient.rgb * mat.diffuse.rgb * diffuse * attenuation;
				reflectedLight += lights[light].ambient.rgb * mat.specular.rgb * specular * attenuation;

			}
			vec3 frag_rgb = min((mat.emissive.rgb + scatteredLight + reflectedLight)*tex.rgb, vec3(1.0));
			if (fogState == 1) {
				frag_rgb = colorFogFunction(frag_rgb, Position.xyz);
			}
			colorOut = vec4(frag_rgb, mat.diffuse.a*tex.a);
			if (tex.a == 0)
				discard;
		}
		else {
			vec4 col = vec4(1,1,1,1);
			colorOut = texture(texmap1, tex_coord)*col;
			if (colorOut.rgb == vec3(0.0))
				discard;
		}
	}
	else {
		vec4 tex = texture(texmap1, tex_coord);
		colorOut = tex * mat.diffuse;
	}
	
}