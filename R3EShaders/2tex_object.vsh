uniform vec3 lightmapTrans;

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1].st = (gl_MultiTexCoord1.xy * lightmapTrans.z) + lightmapTrans.xy;
	
	gl_Position = ftransform();
}
