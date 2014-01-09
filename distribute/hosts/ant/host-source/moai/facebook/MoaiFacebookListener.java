//----------------------------------------------------------------//
// Using Facebook Android SDK 3.6 (January 2014)
// Copyright (c) 2013-present:
//      Author(s):
//              * An Nguyen (MeYuMe http://www.meyume.com)
//              * Ricard Sole (gh: rcsole, Herding Cats)
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import com.facebook.Response;

public interface MoaiFacebookListener {

    public void onDidLogin ();

    public void onDidLogout ();

    public void onDidRequestPermissions ();

    public void onDidGetUserDetails ( Response response );

    public void onDidGetScores ( Response response );

}
