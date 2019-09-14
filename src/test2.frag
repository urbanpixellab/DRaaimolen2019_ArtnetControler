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
    
	vec4 a = vec4(colA.r*c.r,colA.g*c.r,colA.b*c.r,c.r);
	vec4 b = vec4(colB.r*(1.0-c.r),colB.g*(1.0-c.r),colB.b*(1.0-c.r),(1.0-c.r));
    
	vec4 AusgabeFarbe = vec4(a+b) * vec4(pow(bright,2.));


    gl_FragColor = AusgabeFarbe;

    //gl_FragColor = vec4(a.r,a.g,a.b,a.a);
}

//	r *= bright;
//	g *= bright;
//	b *= bright;

//	float r = pow(cc.r,2.);
//	float g = pow(cc.g,2.);
//	float b = pow(cc.b,2.);
    
