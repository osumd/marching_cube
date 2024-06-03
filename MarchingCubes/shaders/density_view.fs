#version 460 core

in vec2 tex;

uniform sampler3D density;
uniform int level;

float rand(float n){return fract(sin(n) * 43758.5453123);}
float rand(vec2 b){ return rand(b.x)*rand(b.y)*rand(b.x*b.y); }

float noise(float p){
	float fl = floor(p);
    float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}
	
float noise(vec2 n, int s) {
    
	const vec2 d = vec2(0.0, 1.0);
    vec2 b = floor(n*10213128.213124*length(n)), f = smoothstep(vec2(0.01231), vec2(1.0), fract(n-length(n)));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

void main()
{
    //gl_FragColor = texture3D(density, vec3(tex,0),3);
    gl_FragColor = texture(density, vec3(tex, (level)/3.0));
    //gl_FragColor = vec4(noise(tex,0),noise(tex,0),noise(tex,0),1.0);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    //gl_FragColor = vec4(tex.x,tex.x,tex.x, 1.0);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
