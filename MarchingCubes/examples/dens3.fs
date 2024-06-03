#version 460 core

in VertexData {
  vec4 wsCoord;
  vec3 chunkCoord;
  flat int nInstanceID;
} inData;

float rand(float n){return fract(sin(n) * 43758.5453123);}
float rand(vec2 b){ return rand(b.x)*rand(b.y)*rand(b.x*b.y); }

float noise(float p){
	float fl = floor(p);
    float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}
	
float noise(vec2 n, float s) {
    
	const vec2 d = vec2(s, 1.0);
    vec2 b = floor(n*10213128.213124*length(n+s)-s), f = smoothstep(vec2(0.01231), vec2(1.0), fract(n-length(n)));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

uniform int depth;

void main()
{
  //gl_FragColor = vec4(DENSITY(inData.wsCoord.xyz)); //Suppose the density texture slice is actually mapped to the framebuffer
  //gl_FragColor = vec4(inData.chunkCoord.x+cos(inData.nInstanceID*180/3.14), inData.chunkCoord.x+cos(inData.nInstanceID*180/3.14), inData.chunkCoord.x+cos(inData.nInstanceID*180/3.14), 1.0);

  float layer_difference = inData.nInstanceID/4.0;

  if(inData.nInstanceID == -69)
  {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  }else
  {
    vec4 r = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 g = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 b = vec4(0.0, 0.0, 1.0, 1.0);
    vec4 p = vec4(1.0, 0.6, 0.0, 1.0);

    if(inData.nInstanceID == 0)
    {
      //gl_FragColor = r;
    }
    if(inData.nInstanceID == 1)
    {
      //gl_FragColor = g;
    }
    if(inData.nInstanceID == 2)
    {
      //gl_FragColor = b;
    }
    if(inData.nInstanceID == 3)
    {
      //gl_FragColor = p;
    }

    gl_FragColor = vec4(inData.chunkCoord.x,0,0,1);

    //gl_FragColor = vec4(noise(inData.chunkCoord.xy,layer_difference)+layer_difference, noise(inData.chunkCoord.xy,0), noise(inData.chunkCoord.xy,0), 1.0);
  }

  
  
    
}