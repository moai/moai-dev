
in LOWP vec4 colorVarying;
in MEDP vec2 uvVarying;

out MEDP vec4 fragColor;

uniform MEDP sampler2D sampler;

void main () {
    fragColor = pow ( texture ( sampler, uvVarying ) * colorVarying, vec4 ( 4.0, 4.0, 4.0, 1.0 ));
}
