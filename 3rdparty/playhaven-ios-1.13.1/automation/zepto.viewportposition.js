(function(Zepto){
    Zepto.fn.viewportPosition = function(){
        var offset = $(this).offset();
        var tapOffsetX = offset.width /2;
        var tapOffsetY = offset.height /2;
 
        return {
            x: offset.left - window.scrollX + tapOffsetX,
            y: offset.top - window.scrollY + tapOffsetY
        }
    }

    Zepto.fn.isTappable = function(){
        var visible = $(this).css("display") != "none";
        var offset = $(this).offset();
        return (visible && offset.left >= 0 && offset.top >= 0 && offset.width > 0 && offset.height > 0);
    }
})($);