//----------------------------------------------------------------//
// Using Facebook Android SDK 3.6 (January 2014)
// Copyright (c) 2013-present:
//		Author(s):
//				* An Nguyen (MeYuMe http://www.meyume.com)
//				* Ricard Sole (gh: rcsole, Herding Cats)
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import org.apache.http.util.ByteArrayBuffer;
import org.json.JSONArray;
import org.json.JSONObject;

import com.facebook.Session;
import com.facebook.Session.OpenRequest;
import com.facebook.model.GraphObject;
import com.facebook.model.GraphObjectList;
import com.facebook.widget.WebDialog;
import com.facebook.AppEventsLogger;
import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.Response;
import com.facebook.SessionDefaultAudience;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;
import com.ziplinegames.moai.Moai;

import android.app.Activity;
import android.content.Intent;
import android.content.SyncResult;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.CompressFormat;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;


//================================================================//
// MoaiFacebook
//================================================================//
public class MoaiFacebook implements Session.StatusCallback {
	private static Activity sActivity = null;
	private static MoaiFacebook instance = null;
	private static Handler handler = null;

	private static MoaiFacebookListener listener = null;

	private final static String LOG_TAG = "Facebook";
	private final static int RC_FB_LOGIN = 6001;
	private final static int RC_FB_REQUEST_PERMISSIONS = 6002;

	private Session currentSession;

	protected static native void AKUNotifyFBDidLogin ();
	protected static native void AKUNotifyFBDidLogout ();
	protected static native void AKUNotifyFBDidRetrieveUserDetails ( String jsonResult );
	protected static native void AKUNotifyFBDidRetrieveFriends ( String jsonResult );
	protected static native void AKUNotifyFBDidClosePublishDialog ( boolean success );
	protected static native void AKUNotifyFBDidCloseRequestDialog ( boolean success );
	protected static native void AKUNotifyFBDidRetrieveScores ( String jsonResult );
	protected static native void AKUNotifyFBDidRetrieveAppRequests ( String jsonResult );
	protected static native void AKUNotifyFBDidRetrievePermissions ( String jsonResult );
	protected static native void AKUNotifyFBDidRequestPermissions ();
	protected static native void AKUNotifyFBDidRetrieveProfilePicture ( String userId, String filepath );
	protected static native void AKUNotifyFBDidRetrieveGraphRequestData ( String jsonResult );

	@Override
	public void call ( Session session, SessionState state, Exception exception ) {
		onSessionStateChanged ( session, state, exception );
	}

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
		if (requestCode == RC_FB_LOGIN || requestCode == RC_FB_REQUEST_PERMISSIONS) {
			if (Session.getActiveSession() != null) {
				Session.getActiveSession().onActivityResult(sActivity, requestCode, resultCode, data);
			}

			if (requestCode == RC_FB_REQUEST_PERMISSIONS) {
				if (listener != null) {
					listener.onDidRequestPermissions();
				} else {
					synchronized (Moai.sAkuLock) {
						AKUNotifyFBDidRequestPermissions();
					}
				}
			}
		} else {
			Log.w(LOG_TAG, "unknown requestCode: " + requestCode);
		}
	}

	//----------------------------------------------------------------//
	private static void onSessionStateChanged ( Session session, SessionState state, Exception exception ) {
		instance.currentSession = session;
		if ( state.isOpened () ) {
			if ( listener != null ) {
				listener.onDidLogin ();
			}

			synchronized (Moai.sAkuLock) {
				AKUNotifyFBDidLogin ();
			}
		} else if ( state.isClosed () ) {
			if ( listener != null ) {
				listener.onDidLogout ();
			}

			synchronized ( Moai.sAkuLock ) {
				AKUNotifyFBDidLogout ();
			}
		}
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		sActivity = activity;
		instance = new MoaiFacebook ();
		handler = new Handler ( Looper.getMainLooper () );
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Bundle savedInstanceState ) {
	}

	//----------------------------------------------------------------//
	public static void onResume () {
	}

	//----------------------------------------------------------------//
	public static void onPause () {
	}

	//----------------------------------------------------------------//
	public static void onDestroy () {
	}

	//----------------------------------------------------------------//
	public static void setListener ( MoaiFacebookListener theListener ) {
		listener = theListener;
	}

	//----------------------------------------------------------------//
	private static class DownloadFBProfilePictureAsynTask extends AsyncTask < String, Void, Void > {

		@Override
		protected Void doInBackground(String... params) {
			download(params[0], params[1], params[2]);

			return null;
		}

		public void download ( final String userId, String urlString, final String saveFilePath ) {
			try {
				URL url = new URL(urlString);
				URLConnection urlCon = url.openConnection();
				InputStream is = urlCon.getInputStream();
				BufferedInputStream bis = new BufferedInputStream(is);

				ByteArrayBuffer baf = new ByteArrayBuffer(50);
				int current = 0;
				while ((current = bis.read()) != -1) {
					baf.append((byte)current);
				}

				// convert to PNG and save
				byte byteArray[] = baf.toByteArray();
				Bitmap bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.length);
				FileOutputStream fos = new FileOutputStream(saveFilePath);
				bitmap.compress(CompressFormat.PNG, 0, fos);
				fos.flush();
				fos.close();

				synchronized (Moai.sAkuLock) {
					AKUNotifyFBDidRetrieveProfilePicture(userId, saveFilePath);
				}
			} catch (MalformedURLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//
	//----------------------------------------------------------------//
	public static void init ( final String appID ) {

		instance.currentSession = Session.getActiveSession ();
		if ( instance.currentSession != null ) {

			onSessionStateChanged ( instance.currentSession, instance.currentSession.getState (), null );
		} else {

			// instance.currentSession = Session.openActiveSessionFromCache ( sActivity );
			if ( instance.currentSession != null ) {

				Session.setActiveSession ( instance.currentSession );
				onSessionStateChanged ( instance.currentSession, instance.currentSession.getState (), null );
			} else {

				instance.currentSession = new Session.Builder ( sActivity ).setApplicationId ( appID ).build ();
			}
		}
	}

	//----------------------------------------------------------------//
	public static void login ( final String appID ) {
		handler.post ( new Runnable () {

			@Override
			public void run() {
				OpenRequest open = new OpenRequest(sActivity);
				open.setLoginBehavior(SessionLoginBehavior.SUPPRESS_SSO);
				open.setPermissions(Arrays.asList(new String[] {"basic_info"}));
				open.setCallback(instance);
				open.setDefaultAudience(SessionDefaultAudience.FRIENDS);
				open.setRequestCode(RC_FB_LOGIN);
				Session session = new Session(sActivity);
				Session.setActiveSession(session);
				session.openForPublish(open);
			}
		});
	}

	//----------------------------------------------------------------//
	public static void logout () {
		handler.post(new Runnable() {

			@Override
			public void run() {
				Session session = Session.getActiveSession();
				if (session != null) {
					session.close();
					session.closeAndClearTokenInformation();
				} else {
					if (instance.currentSession != null) {
						instance.currentSession.close();
						instance.currentSession.closeAndClearTokenInformation();
					}
				}

				if (listener != null) {
					listener.onDidLogout();
				}

				// callback to moai
				synchronized (Moai.sAkuLock) {
					AKUNotifyFBDidLogout();
				}
			}
		});
	}

	//----------------------------------------------------------------//
	// TODO: This needs a rework
	public static String getPermissions () {

		List<String> permissions = null;
		Session session = Session.getActiveSession ();

		if ( session != null ) {
			permissions = session.getPermissions ();
		}

		String ret [];
		if ( permissions != null ) {

			ret = new String [ permissions.size () ];
			permissions.toArray ( ret );
		} else {

			ret = new String [ 0 ];
		}

		String json = "{";

		for ( int i = 0; i < ret.length; i++ ) {
			if ( i > 0 ) {
				json += ",";
			}
			json += "\"" + ret [ i ] + "\":1";
		}
		json += "}";

		synchronized ( Moai.sAkuLock ) {

			AKUNotifyFBDidRetrievePermissions ( json );
		}

		return json;
	}

	//----------------------------------------------------------------//
	//-----------------DOES NOT HAVE LUA BINDINGS---------------------//
	//----------------------------------------------------------------//
	public static boolean hasPublishPermission () {
		List<String> permissions = null;
		Session session = Session.getActiveSession ();
		if ( session != null ) {
			permissions = session.getPermissions ();
		}

		if ( permissions != null ) {
			Iterator<String> it = permissions.iterator ();
			while ( it.hasNext () ) {
				String p = it.next ();
				if ( p.equalsIgnoreCase ( "publish_actions" ) ) {
					return true;
				}
			}
		}

		return false;
	}

	//----------------------------------------------------------------//
	//--------------TODO: SOMETHING IS WRONG--------------------------//
	//----------------------------------------------------------------//
	public static void requestPermissions ( final String [] array ) {
		handler.post( new Runnable () {

			@Override
			public void run () {
				Session.NewPermissionsRequest request = new Session.NewPermissionsRequest ( sActivity, new ArrayList<String> ( Arrays.asList ( array ) ) );
				Session session = Session.getActiveSession ();
				request.setDefaultAudience ( SessionDefaultAudience.FRIENDS );
				request.setRequestCode ( RC_FB_REQUEST_PERMISSIONS );
				session.requestNewPublishPermissions ( request );
			}
		} );
	}

	//----------------------------------------------------------------//
	public static void publish ( final String link, final String picture, final String name, final String caption, final String description, String userId ) {

		handler.post( new Runnable () {

			@Override
			public void run () {
				Bundle params = new Bundle ();
				params.putString ( "link", link );
				params.putString ( "picture", picture );
				params.putString ( "name", name );
				params.putString ( "caption", caption );
				params.putString ( "description", description );

				WebDialog feedDialog = ( new WebDialog.FeedDialogBuilder ( sActivity, instance.currentSession, params ) ).setOnCompleteListener ( new WebDialog.OnCompleteListener () {

					@Override
					public void onComplete( final Bundle values, final FacebookException error ) {

						synchronized ( Moai.sAkuLock ) {
							if ( error == null ) {

								// success?
								String postId = values.getString( "post_id" );
								if ( postId != null ) {

									AKUNotifyFBDidClosePublishDialog ( true );
								} else {

									// user clicked Cancel button
									AKUNotifyFBDidClosePublishDialog ( false );
								}
							} else if ( error instanceof FacebookOperationCanceledException ) {

								// user clicked X button
								AKUNotifyFBDidClosePublishDialog ( false );
							} else {

								// error
								AKUNotifyFBDidClosePublishDialog ( false );
							}
						}
					}
				} ).build ();

				feedDialog.show ();
			}
		} );
	}

	//----------------------------------------------------------------//
	public static void openRequestDialog ( final String title, final String message ) {
		handler.post( new Runnable () {

			@Override
			public void run () {
				Bundle params = new Bundle ();
				params.putString ( "name", title );
				params.putString( "message", message );

				WebDialog requestDialog = ( new WebDialog.RequestsDialogBuilder ( sActivity, instance.currentSession, params ) ).setOnCompleteListener ( new WebDialog.OnCompleteListener () {

					@Override
					public void onComplete ( final Bundle values, final FacebookException error ) {

						synchronized ( Moai.sAkuLock ) {
							if ( error == null ) {

								// success?
								String requestId = values.getString ( "request" );
								if ( requestId != null ) {

									AKUNotifyFBDidCloseRequestDialog ( true );
								} else {

									// user clicked Cancel button
									AKUNotifyFBDidCloseRequestDialog ( false );
								}
							} else if ( error instanceof FacebookOperationCanceledException ) {

								// user clicked X button
								AKUNotifyFBDidCloseRequestDialog ( false );
							} else {

								// error
								AKUNotifyFBDidCloseRequestDialog ( false );
							}
						}
					}
				} ).build ();
				requestDialog.show ();
			}
		} );
	}

	//----------------------------------------------------------------//
	//------TO BE USED IN CONJUCTION WITH GET FRIENDS TO GET IDs------//
	//----------------------------------------------------------------//
	public static void sendRequest ( final String userId, final String message, final String item, final int qty ) {
		handler.post ( new Runnable () {

			@Override
			public void run () {
				Bundle params = new Bundle ();
				params.putString ( "message", message );
				params.putString ( "to", userId );
				params.putString ( "frictionless", "1" );
				params.putString ( "data", item + ";" + qty );

				WebDialog requestDialog =  ( new WebDialog.RequestsDialogBuilder ( sActivity, instance.currentSession, params ) ).setOnCompleteListener ( new WebDialog.OnCompleteListener () {

					@Override
					public void onComplete ( final Bundle values, final FacebookException error ) {
						synchronized ( Moai.sAkuLock ) {
							if ( error == null ) {
								// success?
								String requestId = values.getString ( "request" );
								if ( requestId != null ) {
									AKUNotifyFBDidCloseRequestDialog ( true );
								} else {
									// user clicked Cancel button
									AKUNotifyFBDidCloseRequestDialog ( false );
								}
							} else if ( error instanceof FacebookOperationCanceledException ) {
								// user clicked X button
								AKUNotifyFBDidCloseRequestDialog ( false );
							} else {
								// error
								AKUNotifyFBDidCloseRequestDialog ( false );
							}
						}
					}
				} ).build ();
				requestDialog.show ();
			}
		} );
	}

	//----------------------------------------------------------------//
	public static void getAppRequests () {
		handler.post ( new Runnable () {

			@Override
			public void run () {

				Request request = new Request ( instance.currentSession, "me/apprequests", null, HttpMethod.GET, new Request.Callback () {

					@Override
					public void onCompleted ( Response response ) {
						try {

							final String jsonResult = response.getGraphObject ().getInnerJSONObject ().toString ();
							synchronized ( Moai.sAkuLock ) {

								AKUNotifyFBDidRetrieveAppRequests ( jsonResult );
							}
						} catch ( Exception e ) {

							// TODO: send callback
						}
					}
				});
				Request.executeBatchAsync ( request );
			}
		});
	}

	//----------------------------------------------------------------//
	public static void deleteRequest ( final String requestId ) {
		handler.post ( new Runnable () {

			@Override
			public void run () {
				Request request = new Request ( instance.currentSession, requestId, null, HttpMethod.DELETE, new Request.Callback () {

					@Override
					public void onCompleted ( Response response ) {
						// TODO: notify action went well or wrong
					}
				} );
				Request.executeBatchAsync ( request );
			}
		} );
	}

	//----------------------------------------------------------------//
	//------------TO GET CURRENT USER JUST USE "me" AS USERID---------//
	//----------------------------------------------------------------//
	public static void getUserDetails ( final String userId ) {
		handler.post ( new Runnable () {

			@Override
			public void run () {
				String path;

				if ( userId == "" ) {

					path = "me";
				} else {

					path = userId;
				}
				Bundle params = new Bundle ();
				params.putString ( "fields", "picture,id,name,first_name,last_name,middle_name,gender,username,installed,devices,email,birthday" );

				Request request = new Request ( instance.currentSession, path, params, HttpMethod.GET, new Request.Callback () {

					@Override
					public void onCompleted ( Response response ) {
						try {

							if ( listener != null ) {

								listener.onDidGetUserDetails ( response );
							} else {

								final String jsonResult = response.getGraphObject ().getInnerJSONObject ().toString ();
								synchronized ( Moai.sAkuLock ) {

									AKUNotifyFBDidRetrieveUserDetails ( jsonResult );
								}
							}
						} catch  ( Exception e ) {

							// TODO: send callback
							Log.w ( LOG_TAG, "get user details exception: " + e.getMessage () );
						}
					}
				} );
				Request.executeBatchAsync ( request );
			}
		} );
	}

	//----------------------------------------------------------------//
	public static void getFriends () {
		handler.post ( new Runnable () {

			@Override
			public void run () {
				Bundle params = new Bundle ();
				params.putString ( "fields", "picture,id,name,installed" );

				Request request = new Request ( instance.currentSession, "me/friends", params, HttpMethod.GET, new Request.Callback () {

					@Override
					public void onCompleted ( Response response ) {
						try {
							final String jsonResult = response.getGraphObject ().getInnerJSONObject ().toString ();
							synchronized ( Moai.sAkuLock ) {
								AKUNotifyFBDidRetrieveFriends ( jsonResult );
							}
						} catch ( Exception e ) {
							// TODO: send callback
							e.printStackTrace ();
						}
					}
				} );
				Request.executeBatchAsync ( request );
			}
		} );
	}

	//----------------------------------------------------------------//
	public static void sendScore ( final long score [] ) {
		handler.post(new Runnable() {

			@Override
			public void run() {
				Bundle params = new Bundle();
				params.putString("score", "" + score[0]);

				Request request = new Request(instance.currentSession, "me/scores", params, HttpMethod.POST, new Request.Callback() {

					@Override
					public void onCompleted(Response response) {
						// do nothing
					}
				});
				Request.executeBatchAsync(request);
			}
		});
	}

	//----------------------------------------------------------------//
	public static void getScores ( final String appId ) {
		handler.post ( new Runnable () {

			@Override
			public void run () {
				Request request = new Request ( instance.currentSession, appId + "/scores", null, HttpMethod.GET, new Request.Callback () {

					@Override
					public void onCompleted ( Response response ) {
						try {

							if ( listener != null ) {

								listener.onDidGetScores ( response );
							} else {

								final String jsonResult = response.getGraphObject ().getInnerJSONObject ().toString ();
								synchronized ( Moai.sAkuLock ) {

									AKUNotifyFBDidRetrieveScores ( jsonResult );
								}
							}
						} catch ( Exception e ) {

							// TODO: send callback
							Log.w ( LOG_TAG, ">>>> fb scores exception: " + "failed to retrieve scores" );
						}
					}
				} );
				Request.executeBatchAsync ( request );
			}
		} );
	}

	//----------------------------------------------------------------//
	public static void graphRequest ( final String path, final String m, final Bundle paramsFromLua ) {
		handler.post ( new Runnable () {

			@Override
			public void run () {

				HttpMethod method = HttpMethod.GET;

				if ( m.equalsIgnoreCase ( "POST" ) ) {

					method = HttpMethod.POST;
				} else if ( m.equalsIgnoreCase( "DELETE" ) ) {

					method = HttpMethod.DELETE;
				}

				JSONObject graphObjectJSON = new JSONObject();
				Bundle params = new Bundle ();

				try {
					graphObjectJSON.put ( "title", paramsFromLua.getString ( "title" ) );
					graphObjectJSON.put ( "url", paramsFromLua.getString ( "url" ) );
					graphObjectJSON.put ( "type", paramsFromLua.getString ( "type" ) );
					graphObjectJSON.put ( "description", paramsFromLua.getString ( "description" ) );
				} catch ( Exception e ) {
					MoaiLog.i ( "JSON error" );
				}

				params.putString ( "object", graphObjectJSON.toString () );

				Request request = new Request ( instance.currentSession, path, params, method, new Request.Callback () {

					@Override
					public void onCompleted ( Response response ) {
						try {

							final String jsonResult = response.getGraphObject ().getInnerJSONObject ().toString ();
							synchronized ( Moai.sAkuLock ) {

								AKUNotifyFBDidRetrieveGraphRequestData ( jsonResult );
							}
						} catch ( Exception e ) {

							// TODO: send callback
							Log.w( LOG_TAG, ">>>> fb graph request exception: " + e );
						}
					}
				} );
				Request.executeBatchAsync ( request );
			}
		} );
	}

	//----------------------------------------------------------------//
	public static String loadProfilePicture ( final String userId, String url, boolean fromCacheOnly ) {

		final String picturePath = sActivity.getFilesDir ().getAbsolutePath () + "/" + userId + ".png";
		File file = new File ( picturePath );
		if ( file.exists () ) {

			// file existed, return the path
			return picturePath;
		} else if ( !fromCacheOnly ) {

			if ( url != null && url != "" ) {

				// download picture from url
				MoaiFacebook.downloadProfilePicture ( userId, url, picturePath );
			} else {

				// get url from facebook
				handler.post ( new Runnable () {

					@Override
					public void run () {

						Bundle params = new Bundle ();
						params.putString ( "fields", "picture" );

						Request request = new Request ( instance.currentSession, userId, params, HttpMethod.GET, new Request.Callback () {

							@Override
							public void onCompleted ( Response response ) {
								try {

									JSONObject result = response.getGraphObject ().getInnerJSONObject ();
									JSONObject picture = result.getJSONObject ("picture" );
									JSONObject data = picture.getJSONObject ("data" );
									String urlString = data.getString ("url" );
									if ( urlString != null && urlString != "" ) {

										// download picture from url
										MoaiFacebook.downloadProfilePicture ( userId, urlString, picturePath );
									}
								} catch ( Exception e ) {

									// TODO: send callback
									Log.w ( LOG_TAG, "get fb user's picture exception: " + e.getMessage () );
								}
							}
						} );
						Request.executeBatchAsync ( request );
					}
				} );
			}
		}

		return "";
	}

	//----------------------------------------------------------------//
	private static void downloadProfilePicture ( String userId, String urlString, String saveFilePath ) {
		new DownloadFBProfilePictureAsynTask().execute(userId, urlString, saveFilePath);
	}
}
