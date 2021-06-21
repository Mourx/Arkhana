#define PI 3.14159

uniform vec2 centre;
uniform vec2 iResolution;
uniform float t;
uniform sampler2D currentTexture;
void main()
{
    float effectRadius = 0.0001 + 1.3 * t ;
    float effectAngle = 3. * PI;


    vec2 uv = gl_FragCoord.xy / iResolution.xy - centre;

    float len = length(uv * vec2(iResolution.x / iResolution.y, 1.));
    float angle = atan(uv.y, uv.x) + effectAngle * smoothstep(effectRadius, 0., len);
    float radius = length(uv);

    gl_Color = gl_Color * vec4(1-1.4*t,1-1.4*t,1-1.4*t,1);
    gl_FragColor = gl_Color * texture2D(currentTexture,vec2(radius * cos(angle), radius * sin(angle)) + centre);


}
