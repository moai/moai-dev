
in LOWP vec4 colorVarying;
in MEDP vec2 uvVarying;

out MEDP vec4 fragColor;

uniform MEDP sampler2D sampler;

void main () {
    fragColor = texture ( sampler, uvVarying ) * colorVarying;
}
