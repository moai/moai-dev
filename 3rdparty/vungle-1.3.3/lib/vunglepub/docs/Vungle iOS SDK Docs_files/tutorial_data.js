(function () {
  function sdk_cb(doc) {
    return /dev\/\w+$/i.test(doc.URL) ? "ON_SDK" : "LOST_SDK";
  }

  function app_cb(doc) {
    if (/dashboard\/apps\/?$/i.test(doc.URL))   { return "ON_APP_LIST"; }
    if (/dashboard\/apps\/new$/i.test(doc.URL)) { return "ON_APP_NEW";  }
    if (/dashboard\/apps\/[0-9a-f]+$/i.test(doc.URL)) { return "APP_ADDED"; }
    return "LOST_APP";
  }

  function back_to_sdk_cb(doc) {
    return /dev\/\w+$/i.test(doc.URL) ? "FINAL_STEP" : "BACK_TO_SDK_LOST";
  }

  function generate_app_added_text() {
    var id = $("#store-id").text();
    var platform = $("#platform").data("platform");

    var out = "Congratulations! The App is set up!<br><br>";
    out += "This value is the App Id. You'll use it when integrating our SDK into your App.";
    
    if (platform == "android") {
      out += " You will be using it like this:<br><br><pre>VunglePub.init(\"<b>"+id+"</b>\");</pre>";
    } else if (platform == "ios") {
      out += " You will be using it like this:<br><br><pre>[VGVunglePub startWithPubAppID:@\"<b>"+id+"</b>\" data: data];</pre>";
    }
    return out;
  }

  function final_step_cb(doc) {
    return null;
  }

  var publisher_track = {
    "START": {
      text : "First things first, click here.<br>Then, click either \"<b>iOS Publisher SDK</b>\" or \"<b>Android Publisher SDK</b>\" to download the Publisher kit for your mobile OS.",
      type : "under",
      args : {filter: "#sdk_header_btn", width: "full"},
      next : {onload: sdk_cb}
    },

    "ON_SDK": {
      text : "Click here, and agree to our terms and conditions to download.",
      type : "left",
      args : {filter: "#download_btn", width: 200},
      next : {onload: sdk_cb, onclick: {filter: "#i_agree", stage: "NEED_APP"} }
    },

    "LOST_SDK": {
      text : "Select a Publisher SDK from here...",
      type : "under",
      args : {filter: "#sdk_header_btn", width: "full"},
      next : {onload: sdk_cb}
    },

    "NEED_APP": {
      text : "Now that you have the Publisher SDK, click here to set up your App in our system.",
      type : "under",
      args : {filter: "#app_header_btn", width: "full"},
      next : {onload: app_cb}
    },

    "LOST_APP": {
      text : "Click here to set up your App.",
      type : "under",
      args : {filter: "#app_header_btn", width: "full"},
      next : {onload: app_cb}
    },

    "ON_APP_LIST": {
      text : "Now click this button to create a new App.",
      type : "under",
      args : {filter: "a.btn[href$=apps/new]", width: 200},
      next : {onload: app_cb}
    },

    "ON_APP_NEW": {
      text : "Fill out this form, leaving the URL blank if you haven't added the App to a market yet.<br><br>Then click this button to submit.",
      type : "left",
      args : {filter: "#ios-submit,#android-submit", width: 200},
      next : {onload: app_cb}
    },

    "APP_ADDED": {
      text : generate_app_added_text,
      type : "right",
      args : {filter: "#store-id", width: 450},
      next : {onnext: {text:"What else?", stage:"BACK_TO_SDK"}}
    },

    "BACK_TO_SDK" : {
      text : "Now it's time to learn more about integrating our SDK. Click here, and then select the Publisher SDK for your mobile OS to continue.",
      type : "under",
      args : {filter: "#sdk_header_btn", width:"full"},
      next : {onload: back_to_sdk_cb}
    },

    "BACK_TO_SDK_LOST" : {
      text : "Select the correct Publisher SDK from this menu to learn more about integration.",
      type : "under",
      args : {filter: "#sdk_header_btn", width:"full"},
      next : {onload: back_to_sdk_cb}
    },

    "FINAL_STEP" : {
      text : "Well there we go! This page contains the documentation for our Publisher SDK. Read it in depth for instructions and examples.<br><br>This is the end of our tutorial. We hope it helped, and thanks for choosing Vungle! :)",
      type : "center",
      args : {width:550},
      next : {onload: final_step_cb, onnext: {text:"Thanks!", stage: null}}
    }
  };

  var config_data = {"publisher" : publisher_track};

  // init:
  $(function () {
    tutorial.addPath(config_data);
  });
})();
