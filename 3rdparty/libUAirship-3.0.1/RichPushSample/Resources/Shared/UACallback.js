
// UAListener is the object that communicates with the UAInboxDefaultJSDelegate.
// The name UAListener is hard-coded into that sample.
UAListener = {
    result : {},
    error : {}
};

UAListener.invokeIPhoneCallback = function() {
    var args = arguments;
    var uri = [];
    var dict = null;
    
    // get iPhone callback arguments and dictionary
    for (var i = 0; i < args.length; i++) {
        
        var arg = args[i];

        if (arg == undefined || arg == null) {
            arg = '';
        }

        if (typeof(arg) == 'object') {
            dict = arg;
        } else {
            uri.push(encodeURIComponent(arg));
        }
    }

    // flatten arguments into url
    var url = "ua://callbackArguments:withDictionary:/" + uri.join("/");

    // flatten dictionary into url
    if (dict != null) {
        var query_args = [];
        for (var name in dict) {
            if (typeof(name) != 'string') {
                continue;
            }
            query_args.push(encodeURIComponent(name) + "=" + encodeURIComponent(dict[name]));
        }

        if (query_args.length > 0) {
            url += "?" + query_args.join("&");
        }
    }

    // send to iPhone
    UAirship.invoke(url);
};

// This is a demo function that illustrates how to invoke iOS side callback
function demoFunction() {

    // Customize UAListener
    // Register your own JS callback that might be invoked when iPhone callback finished

    UAListener.onSuccess = function(){
        console.log("iOS callback succeeded");
        alert("UAListener.iOSCallbackDidSucceed: "+this.result);
    };
    
    UAListener.onError = function(){
        console.log("iOS callback failed");
        alert("UAListener.iOSCallbackDidFail: "+this.error);
    };

    // set options
    var opt = {};
    opt.property1 = 1;
    opt.property2 = "option 2";

    // invoke iPhone delegate
    UAListener.invokeIPhoneCallback("arg0", "arg1", "Called from JavaScript running on UIWebView", opt);
};
