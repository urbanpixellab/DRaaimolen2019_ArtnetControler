#version 120

uniform float width;
uniform float freqR;//frquency(0.1 ... 100) and phase(0...1) 
uniform float ampR;//frquency(0.1 ... 100) and phase(0...1) 
uniform float phaseshiftR;//frquency(0.1 ... 100) and phase(0...1) 
uniform float freqG;//frquency(0.1 ... 100) and phase(0...1) 
uniform float ampG;//frquency(0.1 ... 100) and phase(0...1) 
uniform float phaseshiftG;//frquency(0.1 ... 100) and phase(0...1) 
uniform float freqB;//frquency(0.1 ... 100) and phase(0...1) 
uniform float ampB;//frquency(0.1 ... 100) and phase(0...1) 
uniform float phaseshiftB;//frquency(0.1 ... 100) and phase(0...1) 

void main(){
	//this is the fragment shader
	//this is where the pixel level drawing happens
	//gl_FragCoord gives us the x and y of the current pixel its drawing
	
	//we grab the x and y and store them in an int
	float xVal = gl_FragCoord.x/width;
	float yVal = gl_FragCoord.y;

	float red = sin(freqR * (xVal+phaseshiftR) * 6.2814);
	float green = sin(freqG * (xVal+phaseshiftG) * 6.2814);
	float blue = sin(freqB * (xVal+phaseshiftB) * 6.2814);

	gl_FragColor = vec4(red*ampR,green*ampG,blue*ampB,1);    
	/*
	//we use the mod function to only draw pixels if they are every 2 in x or every 4 in y
	if( mod(xVal, 2.0) == 0.1 && mod(yVal, 4.0) == 0.5 ){
		gl_FragColor = gl_Color;    
    }else{
		gl_FragColor.a = 0.0;
	}
	*/
}