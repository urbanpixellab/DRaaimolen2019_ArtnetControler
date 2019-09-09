uniform float freq;//the frequency or repetition of the pattern
uniform float shift;
uniform sampler2DRect tex;
varying vec2 texcoord;
uniform vec2 res;
uniform vec3 colA;
uniform vec3 colB;
uniform float bright;

void main()
{
	vec2 s = texcoord;
	float val = mod((s.x * freq) + shift,res.x);
	s.x = val;
	vec4 c = texture2DRect(tex,s);
	vec3 ca = colA/255.;
	vec3 cb = colA/255.;
	float r  = max(c.r * ca.r,(1.0-c.r) * cb.r);
	float g  = max(c.r * ca.g,(1.0-c.r) * cb.g);
	float b  = max(c.r * ca.b,(1.0-c.r) * cb.b);
	
	r *= bright;
	g *= bright;
	b *= bright;

//	r = pow(r,4.);
//	g = pow(g,4.);
//	b = pow(b,4.);

    gl_FragColor = vec4(r,g,b,1.0);
}