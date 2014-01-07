package com.ziplinegames.moai;

import com.facebook.Response;

public interface MoaiFacebookListener {

    public void onDidLogin ();

    public void onDidLogout ();

    public void onDidRequestPermissions ();

    public void onDidGetUserDetails ( Response response );

    public void onDidGetScores ( Response response );

}
