
in MEDP vec2 uvVarying;

out MEDP vec4 fragmentColor;

uniform MEDP float glow;
uniform MEDP sampler2D colorSampler;
uniform MEDP sampler2D glowSampler;

//----------------------------------------------------------------//
void main () {

    fragmentColor = texture ( colorSampler, uvVarying ) + ( texture ( glowSampler, uvVarying ) * glow );
}
