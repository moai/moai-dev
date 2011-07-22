//
//  Shader.fsh
//  opengl
//
//  Created by Patrick Meehan on 7/14/11.
//  Copyright 2011 Groundspeak, Inc. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
