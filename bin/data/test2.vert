varying vec2 texcoord;

void main(){
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        texcoord = gl_MultiTexCoord0.st;
}