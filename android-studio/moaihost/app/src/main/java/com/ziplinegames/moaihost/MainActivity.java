package com.ziplinegames.moaihost;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import com.external.base64.*;
import com.ziplinegames.moai.*;

public class MainActivity extends ActionBarActivity {

    //----------------------------------------------------------------//
    static {

        try {
            MoaiLog.i ( "Loading libmoai.so" );
            System.loadLibrary("moai");

            //MoaiLog.i ( "Loading libfloob.so" );
            //System.loadLibrary("floob");
        }
        catch ( Exception e ) {
            MoaiLog.i ( "EXCEPTION Loading libmoai.so" );
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            String str = new String ( Base64.decode ( "Zm9vYmFyYmF6" ), "UTF8" );
            MoaiLog.i ( str );
        }
        catch ( Exception e ) {
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
