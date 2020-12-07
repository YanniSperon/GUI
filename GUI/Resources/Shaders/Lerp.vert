#version 460

//uniform vec3[4] controlPoints;
uniform vec3[8] controlPoints;

vec3 quadraticBezier(vec3 A, vec3 B, vec3 C, float t) {
	vec3 D = mix(A, B, t);
	vec3 E = mix(B, C, t);

	return mix(D, E, t);
}

vec3 cubicBezier(vec3 A, vec3 B, vec3 C, vec3 D, float t) {
	vec3 E = mix(A, B, t);
	vec3 F = mix(B, C, t);
	vec3 G = mix(C, D, t);

	return quadraticBezier(E, F, G, t);
}

vec3 quarticBezier(vec3 A, vec3 B, vec3 C, vec3 D, vec3 E, float t) {
	vec3 F = mix(A, B, t);
	vec3 G = mix(B, C, t);
	vec3 H = mix(C, D, t);
	vec3 I = mix(D, E, t);

	return cubicBezier(F, G, H, I, t);
}

vec3 quinticBezier(vec3 A, vec3 B, vec3 C, vec3 D, vec3 E, vec3 F, float t) {
	vec3 G = mix(A, B, t);
	vec3 H = mix(B, C, t);
	vec3 I = mix(C, D, t);
	vec3 J = mix(D, E, t);
	vec3 K = mix(E, F, t);

	return quarticBezier(G, H, I, J, K, t);
}

vec3 sexticBezier(vec3 A, vec3 B, vec3 C, vec3 D, vec3 E, vec3 F, vec3 G, float t) {
	vec3 H = mix(A, B, t);
	vec3 I = mix(B, C, t);
	vec3 J = mix(C, D, t);
	vec3 K = mix(D, E, t);
	vec3 L = mix(E, F, t);
	vec3 M = mix(F, G, t);

	return quinticBezier(H, I, J, K, L, M, t);
}

vec3 septicBezier(vec3 A, vec3 B, vec3 C, vec3 D, vec3 E, vec3 F, vec3 G, vec3 H, float t) {
	vec3 I = mix(A, B, t);
	vec3 J = mix(B, C, t);
	vec3 K = mix(C, D, t);
	vec3 L = mix(D, E, t);
	vec3 M = mix(E, F, t);
	vec3 N = mix(F, G, t);
	vec3 O = mix(G, H, t);

	return sexticBezier(I, J, K, L, M, N, O, t);
}

void main()
{
	gl_Position = vec4(cubicBezier(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], gl_VertexID / 200.0), 1.0);
	//gl_Position = vec4(septicBezier(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], controlPoints[4], controlPoints[5], controlPoints[6], controlPoints[7], gl_VertexID / 200.0), 1.0);
}