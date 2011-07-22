//
//  Shader.vsh
//  opengl
//
//  Created by Patrick Meehan on 7/14/11.
//  Copyright 2011 Groundspeak, Inc. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

//uniform float translate;
uniform mat4 transform;

void main()
{
    gl_Position = position * transform;
    colorVarying = color;
}
