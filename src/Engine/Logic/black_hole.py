import moderngl as mgl
import numpy as np
import pygame as pg

class BlackHole:
    def __init__(self, engine):
        self.engine = engine
        self.ctx = engine.ctx
        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330
                in vec2 in_vert;
                out vec2 v_text;
                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_text = in_vert * 0.5 + 0.5;
                }
            ''',
            fragment_shader='''
                #version 330
                
                #define AA 2
                #define _Speed 3.0
                #define _Steps 12.
                #define _Size 0.3

                in vec2 v_text;
                out vec4 f_color;
                
                uniform vec2 iResolution;
                uniform float iTime;
                uniform vec4 iMouse;

                float hash(float x){ return fract(sin(x)*152754.742);}
                float hash(vec2 x){ return hash(x.x + hash(x.y));}

                float value(vec2 p, float f)
                {
                    float bl = hash(floor(p*f + vec2(0.,0.)));
                    float br = hash(floor(p*f + vec2(1.,0.)));
                    float tl = hash(floor(p*f + vec2(0.,1.)));
                    float tr = hash(floor(p*f + vec2(1.,1.)));
                    
                    vec2 fr = fract(p*f);    
                    fr = (3. - 2.*fr)*fr*fr;    
                    float b = mix(bl, br, fr.x);    
                    float t = mix(tl, tr, fr.x);
                    return  mix(b,t, fr.y);
                }

                vec4 background(vec3 ray)
                {
                    vec2 uv = ray.xy;
                    
                    if( abs(ray.x) > 0.5)
                        uv.x = ray.z;
                    else if( abs(ray.y) > 0.5)
                        uv.y = ray.z;

                    float brightness = value( uv*3., 100.);
                    float color = value( uv*2., 20.); 
                    brightness = pow(brightness, 256.);
                
                    brightness = brightness*100.;
                    brightness = clamp(brightness, 0., 1.);
                    
                    vec3 stars = brightness * mix(vec3(1., .6, .2), vec3(.2, .6, 1), color);

                    vec4 nebulae = vec4(uv, 0.5 + 0.5*sin(iTime), 1.0);
                    nebulae.xyz *= 0.5;
                
                    nebulae.xyz += stars;
                    return nebulae;
                }

                vec4 raymarchDisk(vec3 ray, vec3 zeroPos)
                {
                    vec3 position = zeroPos;      
                    float lengthPos = length(position.xz);
                    float dist = min(1., lengthPos*(1./_Size) *0.5) * _Size * 0.4 *(1./_Steps) /( abs(ray.y) );

                    position += dist*_Steps*ray*0.5;     

                    vec2 deltaPos;
                    deltaPos.x = -zeroPos.z*0.01 + zeroPos.x;
                    deltaPos.y = zeroPos.x*0.01 + zeroPos.z;
                    deltaPos = normalize(deltaPos - zeroPos.xz);
                    
                    float parallel = dot(ray.xz, deltaPos);
                    parallel /= sqrt(lengthPos);
                    parallel *= 0.5;
                    float redShift = parallel +0.3;
                    redShift *= redShift;

                    redShift = clamp(redShift, 0., 1.);
                    
                    float disMix = clamp((lengthPos - _Size * 2.)*(1./_Size)*0.24, 0., 1.);
                    vec3 insideCol =  mix(vec3(1.0,0.8,0.0), vec3(0.5,0.13,0.02)*0.2, disMix);
                    
                    insideCol *= mix(vec3(0.4, 0.2, 0.1), vec3(1.6, 2.4, 4.0), redShift);
                    insideCol *= 1.25;
                    redShift += 0.12;
                    redShift *= redShift;

                    vec4 o = vec4(0.);

                    for(float i = 0. ; i < _Steps; i++)
                    {                      
                        position -= dist * ray ;  

                        float intensity =clamp( 1. - abs((i - 0.8) * (1./_Steps) * 2.), 0., 1.); 
                        float lengthPos = length(position.xz);
                        float distMult = 1.;

                        distMult *=  clamp((lengthPos -  _Size * 0.75) * (1./_Size) * 1.5, 0., 1.);        
                        distMult *= clamp(( _Size * 10. -lengthPos) * (1./_Size) * 0.20, 0., 1.);
                        distMult *= distMult;

                        float u = lengthPos + iTime* _Size*0.3 + intensity * _Size * 0.2;

                        vec2 xy ;
                        float rot = mod(iTime*_Speed, 8192.);
                        xy.x = -position.z*sin(rot) + position.x*cos(rot);
                        xy.y = position.x*sin(rot) + position.z*cos(rot);

                        float x = abs( xy.x/(xy.y));         
                        float angle = 0.02*atan(x);
          
                        const float f = 70.;
                        float noise = value( vec2( angle, u * (1./_Size) * 0.05), f);
                        noise = noise*0.66 + 0.33*value( vec2( angle, u * (1./_Size) * 0.05), f*2.);     

                        float extraWidth =  noise * 1. * (1. -  clamp(i * (1./_Steps)*2. - 1., 0., 1.));

                        float alpha = clamp(noise*(intensity + extraWidth)*( (1./_Size) * 10.  + 0.01 ) *  dist * distMult , 0., 1.);

                        vec3 col = 2.*mix(vec3(0.3,0.2,0.15)*insideCol, insideCol, min(1.,intensity*2.));
                        o = clamp(vec4(col*alpha + o.rgb*(1.-alpha), o.a*(1.-alpha) + alpha), vec4(0.), vec4(1.));

                        lengthPos *= (1./_Size);
           
                        o.rgb+= redShift*(intensity*1. + 0.5)* (1./_Steps) * 100.*distMult/(lengthPos*lengthPos);
                    }  
         
                    o.rgb = clamp(o.rgb - 0.005, 0., 1.);
                    return o ;
                }

                void Rotate(inout vec3 vector, vec2 angle)
                {
                    vector.yz = cos(angle.y)*vector.yz
                        +sin(angle.y)*vec2(-1,1)*vector.zy;
                    vector.xz = cos(angle.x)*vector.xz
                        +sin(angle.x)*vec2(-1,1)*vector.zx;
                }

                void main() {
                    vec4 colOut = vec4(0.);
                    
                    vec2 fragCoord = gl_FragCoord.xy;
                    vec2 fragCoordRot;
                    fragCoordRot.x = fragCoord.x*0.985 + fragCoord.y * 0.174;
                    fragCoordRot.y = fragCoord.y*0.985 - fragCoord.x * 0.174;
                    fragCoordRot += vec2(-0.06, 0.12) * iResolution.xy;
                    
                    for( int j=0; j<AA; j++ )
                    for( int i=0; i<AA; i++ )
                    {
                        vec3 ray = normalize( vec3((fragCoordRot-iResolution.xy*.5  + vec2(i,j)/(float(AA)))/iResolution.x, 1 )); 
                        vec3 pos = vec3(0.,0.05,-(20.*iMouse.xy/iResolution.y-10.)*(20.*iMouse.xy/iResolution.y-10.)*.05); 
                        vec2 angle = vec2(iTime*0.1,.2);      
                        angle.y = (2.*iMouse.y/iResolution.y)*3.14 + 0.1 + 3.14;
                        float dist = length(pos);
                        Rotate(pos,angle);
                        angle.xy -= min(.3/dist , 3.14) * vec2(1, 0.5);
                        Rotate(ray,angle);

                        vec4 col = vec4(0.); 
                        vec4 glow = vec4(0.); 
                        vec4 outCol =vec4(100.);

                        for(int disks = 0; disks< 20; disks++) //steps
                        {

                            for (int h = 0; h < 6; h++) //reduces tests for exit conditions (to minimise branching)
                            {
                                float dotpos = dot(pos,pos);
                                float invDist = inversesqrt(dotpos); //1/distance to BH
                                float centDist = dotpos * invDist; 	//distance to BH
                                float stepDist = 0.92 * abs(pos.y /(ray.y));  //conservative distance to disk (y==0)   
                                float farLimit = centDist * 0.5; //limit step size far from to BH
                                float closeLimit = centDist*0.1 + 0.05*centDist*centDist*(1./_Size); //limit step size closse to BH
                                stepDist = min(stepDist, min(farLimit, closeLimit));
                                
                                float invDistSqr = invDist * invDist;
                                float bendForce = stepDist * invDistSqr * _Size * 0.625;  //bending force
                                ray =  normalize(ray - (bendForce * invDist )*pos);  //bend ray towards BH
                                pos += stepDist * ray; 
                                
                                glow += vec4(1.2,1.1,1, 1.0) *(0.01*stepDist * invDistSqr * invDistSqr *clamp( centDist*(2.) - 1.2,0.,1.)); //adds fairly cheap glow
                            }

                            float dist2 = length(pos);

                            if(dist2 < _Size * 0.1) //ray sucked in to BH
                            {
                                outCol =  vec4( col.rgb * col.a + glow.rgb *(1.-col.a ) ,1.) ;
                                break;
                            }

                            else if(dist2 > _Size * 1000.) //ray escaped BH
                            {                   
                                vec4 bg = background (ray);
                                outCol = vec4(col.rgb*col.a + bg.rgb*(1.-col.a)  + glow.rgb *(1.-col.a    ), 1.);       
                                break;
                            }

                            else if (abs(pos.y) <= _Size * 0.002 ) //ray hit accretion disk
                            {                             
                                vec4 diskCol = raymarchDisk(ray, pos);   //render disk
                                pos.y = 0.;
                                pos += abs(_Size * 0.001 /ray.y) * ray;  
                                            col = vec4(diskCol.rgb*(1.-col.a) + col.rgb, col.a + diskCol.a*(1.-col.a));
                                        }	
                            }
                            
                        if(outCol.r == 100.)
                        outCol = vec4(col.rgb + glow.rgb *(col.a +  glow.a) , 1.);

                            col = outCol;
                            col.rgb =  pow( col.rgb, vec3(0.6) );
                            
                        colOut += col/float(AA*AA);
                    }
                    
                    f_color = colOut;
                }
            '''
        )
        
        vertices = np.array([-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0], dtype='f4')
        self.vbo = self.ctx.buffer(vertices)
        self.vao = self.ctx.vertex_array(self.prog, [(self.vbo, '2f', 'in_vert')])

    def render(self):
        self.prog['iResolution'].value = self.engine.WIN_SIZE
        self.prog['iTime'].value = self.engine.time
        mouse_pos = pg.mouse.get_pos()
        self.prog['iMouse'].value = (mouse_pos[0], mouse_pos[1], 0, 0)
        self.vao.render(mgl.TRIANGLE_FAN)