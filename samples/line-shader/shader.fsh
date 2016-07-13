// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform MEDP float dash;

void main() { 

    if (( dash <= 0.0 ) || (( mod (( uvVarying.y - ( dash * 0.5 )) / ( dash * 2.0 ), 1.0 ) - 0.5 ) < 0.0 )) {
        gl_FragColor = colorVarying;
    }
    else {
        discard;
    }
}
