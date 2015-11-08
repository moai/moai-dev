/**
 * Created by David on 4/11/2015.
 */
function appendScriptTag(path) {
    var s = document.createElement("script");
    s.type = "text/javascript";
    s.src = path;
    document.body.appendChild(s);
    console.log("Appended script",path);
}

function loadDeferredScripts( scripts, defines, callback ) {
    //we defer moaijs.js and our rom

    for (var i = 0; i < defines.length; i++) {
        window[defines[i]] = undefined;
    }
    for (var i = 0; i < scripts.length; i++) {
        appendScriptTag(scripts[i]);
    }

    console.log("started deferred loading of scripts",scripts);
    console.log("waiting on scripts to load by watching for ",defines);
    var awaiter = window.setInterval(
        function() {
            var finished = 0;
            for(var i = 0; i < defines.length; i++)  {
                if (window[defines[i]]) {
                    console.log("script finished loading", scripts[i]);
                    finished = finished + 1;
                }
            }
            if (finished == defines.length) {
                console.log("Deferred scripts loaded");
                window.clearInterval(awaiter);
                callback();
            }
        }, 10);
};