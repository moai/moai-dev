/**
 * Created by David on 7/11/2015.
 */

var fileUtil = function() {
    var M = {};

    var typemap = {
        'ttf': {type: 'font', mime: 'application/font-sfnt'},
        'bmp': {type: 'image', mime: 'image/bmp'},
        'svg': {type: 'image', mime: 'image/svg+xml'},
        'png': {type: 'image', mime: 'image/png'},
        'jpg': {type: 'image', mime: 'image/jpeg'},
        'jpeg': {type: 'image', mime: 'image/jpeg'},
        'ogg': {type: 'audio', mime: 'audio/ogg'},
        'wav': {type: 'audio', mime: 'audio/vnd.wave'},
        'mp3': {type: 'audio', mime: 'audio/mpeg'},
        'lua': {type: 'text', mime: 'text/plain'},
        'json': {type: 'text', mime: 'text/plain'},
        'xml': {type: 'text', mime: 'text/plain'},
        'conf': {type: 'text', mime: 'text/plain'},
        'cfg': {type: 'text', mime: 'text/plain'},
        'txt': {type: 'text', mime: 'text/plain'},
        'fnt': {type: 'text', mime: 'text/plain'}
    };

    function arrayToString(arr) {
        return (String.fromCharCode).apply(null, new Uint16Array(arr));
    }

    M.asText = function (file) {
        return arrayToString(file.data);
    };


    M.getExtension = function(filename) {
        var parts = filename.split('.');
        return _(parts).last().toLowerCase();
    };

    M.getType = function(file) {
        var ext = M.getExtension(file.filename);
        var typeinfo = typemap[ext];
        if (!typeinfo) return 'unknown';
        return typeinfo.type;
    };

    M.getMimetype = function(file) {
        var ext = M.getExtension(file.filename);
        var typeinfo = typemap[ext];
        if (!typeinfo) return 'application/octet-stream';
        return typeinfo.mime;
    };

    M.asObjectUrl = function(file) {
        var arr = file.data;
        var mime = M.getMimetype(file);
        var blob = new Blob([arr], {type: mime});
        return URL.createObjectURL(blob);
    };


    M.isType = function(file, type) {
        return M.getType(file) == type;
    };
    return M;
}();


