// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec4 prevPosition;
attribute vec4 nextPosition;
attribute float time;
attribute float side; // -1 == left, 1 == right
attribute float edge; // 0 == prev, 1 == next

uniform mat4    transform;
uniform vec4    color;
uniform float   viewWidth;
uniform float   viewHeight;
uniform float   radius;

varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

vec4 clipToWindowSpace ( vec4 clipPos ) {
    
    clipPos.xyz /= clipPos.w;
    clipPos.x *= viewWidth / 2.0;
    clipPos.y *= viewHeight / 2.0;
    return clipPos;
}

vec4 windowToClipSpace ( vec4 windowPos ) {
    
    windowPos.x /= viewWidth / 2.0;
    windowPos.y /= viewHeight / 2.0;
    windowPos.w = 1.0;
    return windowPos;
}

void main () {

    colorVarying = color;

    vec4 currPos = clipToWindowSpace ( position * transform );
    vec4 nextPos = clipToWindowSpace ( nextPosition * transform );
    vec4 prevPos = clipToWindowSpace ( prevPosition * transform );

    vec2 edge0 = vec2 ( currPos.xy - prevPos.xy );
    vec2 edge1 = vec2 ( nextPos.xy - currPos.xy );

    float length0 = currPos == prevPos ? 0.0 : length ( edge0 );
    float length1 = currPos == nextPos ? 0.0 : length ( edge1 );

    if ( length0 == 0.0 && length1 == 0.0 ) {

        gl_Position = windowToClipSpace ( currPos );
        return;
    }
    else if ( length0 == 0.0 ) {

        edge1 = edge1 / length1;
        vec4 jointNorm = vec4 ( -edge1.y, edge1.x, 0.0, 0.0 );
        gl_Position = windowToClipSpace ( currPos + ( jointNorm * radius ));

        uvVarying = vec2 ( 0.0, dot ( currPos.xy, edge1 ) * side );
    }
    else if ( length1 == 0.0 ) {

        edge0 = edge0 / length0;
        vec4 jointNorm = vec4 ( -edge0.y, edge0.x, 0.0, 0.0 );
        gl_Position = windowToClipSpace ( currPos + ( jointNorm * radius ));

        uvVarying = vec2 ( 0.0, dot ( currPos.xy, edge0 ) * side );
    }
    else {

        edge0 = edge0 / length0;
        edge1 = edge1 / length1;

        uvVarying = vec2 ( 0.0, dot ( currPos.xy, edge == 0.0 ? edge0 : edge1 ) * side );

        vec2 edgeNorm0 = vec2 ( -edge0.y, edge0.x );
        vec2 edgeNorm1 = vec2 ( -edge1.y, edge1.x );

        edgeNorm0 = normalize ( edgeNorm0 );
        edgeNorm1 = normalize ( edgeNorm1 );

        // early out if line doubles back on itself (180 degrees)
        if ( dot ( edgeNorm0, edgeNorm1 ) <= -0.99999 ) {

            vec2 v = time < 0.5 ? edgeNorm0 : edgeNorm1;
            gl_Position = windowToClipSpace ( currPos + vec4 ( v * radius, 0.0, 0.0 ));
            return;
        }

        vec4 jointNorm = vec4 ( normalize ( edgeNorm0 + edgeNorm1 ), 0.0, 0.0 ) ;
        
        float miter = radius / dot ( vec4 ( edgeNorm0, 0.0, 0.0 ), jointNorm );

        if (( miter / radius ) > 1.0 ) {
            
            if ( dot ( jointNorm.xy, edge1 ) < 0.0 ) {

                float angle = -acos ( dot ( edgeNorm0, edgeNorm1 )) * time;

                vec2 n = edgeNorm0;

                float ca = cos ( angle );
                float sa = sin ( angle );

                vec2 v = vec2 (( n.x * ca ) - ( n.y * sa ), ( n.y * ca ) + ( n.x * sa ));

                gl_Position = windowToClipSpace ( currPos + vec4 ( v * radius, 0.0, 0.0 ));
            }
            else {

                vec2 v = time < 0.5 ? edgeNorm0 : edgeNorm1;
                gl_Position = windowToClipSpace ( currPos + vec4 ( v * radius, 0.0, 0.0 ));
            }
        }
        else {

            gl_Position = windowToClipSpace ( currPos + ( jointNorm * miter ));
        }		
    }
}