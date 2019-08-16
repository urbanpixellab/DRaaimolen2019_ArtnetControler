uniform float freq;/// repetitions
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
	float r  = max(c.r * colA.r,(1.0-c.r) * colB.r);
	float g  = max(c.r * colA.g,(1.0-c.r) * colB.g);
	float b  = max(c.r * colA.b,(1.0-c.r) * colB.b);
	
    gl_FragColor = vec4(r*bright,g*bright,b*bright,1.0);
}