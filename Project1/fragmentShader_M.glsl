#version 330 core
#extension GL_OES_standard_derivatives : enable
#define PI 3.14159
precision highp float;

in vec4 ourColor;

uniform int sceltaFS;
uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
const int STAR_COUNT = 500;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 4758.5453);
}

void point(vec2 pos, float size) {
    pos *= resolution;
    float d = distance(gl_FragCoord.xy, pos);
    gl_FragColor.rgb += smoothstep(size, 0.0, d);
}

void stars() {
    for (int i = 1; i < STAR_COUNT; i++) {
        float sx = rand(vec2(i, -i));
        float sy = rand(vec2(-sx, sx));
        float size = rand(vec2(sx, sy)) ;
        point(vec2(sx * 2.0 - 1.0 + mouse.x, sy * 2.0 - 1.0 + mouse.y), size * size * 2.0 + 1.0);
    }
}
//nuovo
vec4 textureRND2D(vec2 uv){
	uv = floor(fract(uv)*1e3);
	float v = uv.x+uv.y*1e3;
	return fract(1e5*sin(vec4(v*1e-2, (v+1.)*1e-2, (v+1e3)*1e-2, (v+1e3+1.)*1e-2)));
}

float noise(vec2 p) {
	vec2 f = fract(p*1e3);
	vec4 r = textureRND2D(p);
	f = f*f*(3.0-2.0*f);
	return (mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y));	
}

float cloud(vec2 p) {
	float v = 0.0;
	v += noise(p*1.)*.80000;
	v += noise(p*6.)*.25000;
	v += noise(p*4.)*.12500;
	v += noise(p*8.)*.06250;
	v += noise(p*16.)*.03125;
	return v*v*v;
}

void main() {
    if (sceltaFS==0)
    {
        gl_FragColor = vec4(ourColor);
    }

    if (sceltaFS==1)
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        vec2 rotation = vec2((mouse.x * 5.0 - 1.0), (mouse.y - 5.00)) * -PI;
        float xzLen = cos(rotation.y);
        vec3 dir = vec3(xzLen * sin(-rotation.x), sin(rotation.y), xzLen * cos(rotation.x));
        vec2 res = 1.0 / resolution;
        stars();
    }
    if(sceltaFS==2){
       vec2 p = gl_FragCoord.xy*.05+.5;
	vec3 c = vec3(.1, .1, .1);
	float c1 = cloud(p*.3+time*.02)*.5;
	c1 = pow(c1,1.5);
	float c2 = cloud(p*.2+time*.002)*.75;
	c2 = pow(c2, 1.5);
	c.rgb += vec3(.0, .0, .0) * c1*.5;
	p += c1*0.006125;
	p -= c2*0.006125;
	c.gbr += vec3(0., 0., 0.) * c2*.5;
	//p.y *= c.g;
	c.grb += vec3(1., 1., 1.) * cloud(p*.1+time*-.00005)*1.;
	//c += .125;
	gl_FragColor = vec4(c, 1.);
    }
}