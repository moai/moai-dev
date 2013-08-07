var tutorial = {};

(function($) {
  var HIGHLIGHT_ID = "__tut_highlight";
  var __config = {};

  // Our mixpanel code (which is used in our template) will create a umail variable
  // that contains the user's email. (Good for a unique identifier.)
  // Kinda hackey, but we can't really wrap stuff in dynamically here, cause this is
  // a static js file, not a template:
  
  function cookieSave(name, val) {
    name = umail + "::" + name;
    $.cookie(name, val, {path:'/', expires:3});
  }

  function cookieRead(name) {
    name = umail + "::" + name;
    return $.cookie(name, {path:'/', expires:3});
  }


  function getDimensions(elem) {
    var l = elem.offset().left;
    var t = elem.offset().top;
    var w = elem.innerWidth();
    var h = elem.innerHeight();

    return {l:l, t:t, w:w, h:h};
  }

  // Will animate the main block:
  function doAnim() {
    // Bounce it, to make it more visible:
    setTimeout(function () {
      $("#" + HIGHLIGHT_ID).effect("bounce", { times:3, distance: 10, direction: "down" }, 300);
    }, 500);
  }

  function doTrack(custom_str) {
    if (!custom_str) {
      var path = cookieRead('wiz_path');
      var stage = cookieRead('wiz_stage');
      if (!path || !stage) {
        return;
      }
      custom_str = path + " -> " + stage;
    }
    
    mixpanel.track('tutorial -> ' + custom_str);
  }


  // Adds a tutorial configuration to the current context. Call this before drawTutorial gets called.
  tutorial.addPath = function addPath(config) {

    $.extend(__config, config);

    // If the current stage (if it even exists) has an onload handler, then use it to transition to the new stage:
    var path = cookieRead('wiz_path');
    var stage = cookieRead('wiz_stage');

    // Act on the `next` arguments:
    if ( typeof(__config) === 'object' && (path in __config) && (stage in __config[path]) && typeof(__config[path][stage].next) === 'object' ) {
      var cur = __config[path][stage];

      // Handle onload events:
      if ( cur.next.onload ) {
        var ret = cur.next.onload(document);
        cookieSave('wiz_stage', ret);
        if (ret === null) {
          // End of tutorial:
          doTrack("ended naturally");
          return tutorial.stop();
        }
        stage = ret;
        cur = __config[path][stage];
      } 

      // Handle onclick events:
      if ( cur.next.onclick ) {
        $(cur.next.onclick.filter).click( (function (state) {
          return (function () {
            cookieSave('wiz_stage', state);
            if (state === null) {
              // End of tutorial:
              doTrack("ended naturally");
              return tutorial.stop();
            }
            drawTutorial();
            doAnim();
            doTrack();
          });
        })(cur.next.onclick.stage));
      }
    }

    // Draw the tutorial block, and also set it to redraw when the window resizes:
    $(window).resize(drawTutorial);
    drawTutorial();
    doAnim();
    doTrack();
  }


  tutorial.start = function start(tut_name) {
    // Don't do anything if this tutorial hasn't been configured:
    if ( typeof(__config) !== "object" || ! (tut_name in __config) ) {
      throw new Error("Either bad tutorial path, or bad configuration");
    }

    // Else, set the session cookies to the correct tutorial path:
    cookieSave('wiz_path', tut_name);
    cookieSave('wiz_stage', 'START');

    // Draw the current stage:
    drawTutorial();
    doAnim();
    doTrack();
  }


  tutorial.stop = function stop() {
    cookieSave('wiz_path',  null);
    cookieSave('wiz_stage', null);
    drawTutorial();
  }


  function popover_cancel_click_event() {
    doTrack("cancelled");
    tutorial.stop();
  }


  function popover_next_click_event() {
    var newStage = $(this).data("nextstage");
    cookieSave('wiz_stage', newStage);
    if (newStage === null) {
      // End of tutorial:
      doTrack("ended naturally");
      return tutorial.stop();
    }
    drawTutorial();
    doAnim();
    doTrack();
  }


  /**
   * Creates the pop-over item for the current tutorial stage:
   * @cur - The current tutorial stage object.
   * @target - What we're pointing at.
   * @dims - Object representing the placement of the popover. Formatted like {x:#, y:#, w:#, h:#}.
   * @direction - Direction class to point the arrow in the right direction.
   * Returns a jQuery-encapsulated DOM element.
   */
  function newPopover(cur, direction_class) {

    direction_class = (!direction_class) ? "" : " " + direction_class;
    
    var html;
    html += '<div id="'+ HIGHLIGHT_ID + '" class="tutorialPopover'+ direction_class +'" style="display: block;">';
    if (direction_class.length > 0) {
      html += '<div class="arrow"></div>';  
    }
    html += '<div class="popover-inner"><div class="popover-content tutorial" style="position:relative;">';
    html += '<button id="end-tut-btn" class="close"><i class="icon-remove"></i></button>';
    html += '<p style="font-size:larger;text-align:center;"></p>';
    html += '</div>';
    if (cur.next.onnext) {
      html += '<a id="next-btn" href="#" style="margin-top:3px;" class="btn btn-info pull-right" data-nextstage="'+ cur.next.onnext.stage +'"><i class="icon-forward icon-white"></i>&nbsp;' + cur.next.onnext.text + '</a></div></div>';
    }

    $("#" + HIGHLIGHT_ID).remove();
    $(html).appendTo("body");

    var highlight = $("#" + HIGHLIGHT_ID);
    var arrow = highlight.find(".arrow");
    var inner = highlight.find(".popover-inner");

    // Send in the text!
    var txt = typeof(cur.text) === 'function' ? cur.text() : cur.text;
    inner.find(".popover-content>p").html(txt);

    // Attach button actions:
    inner.find("div>#end-tut-btn").click(popover_cancel_click_event);
    if (cur.next.onnext) {
      inner.find("#next-btn").click(popover_next_click_event);
    }
    return highlight;
  }


  function drawUnder(cur) {
    var thing = $(cur.args.filter).filter(":visible");
    if (thing.length == 0) { return; }

    var dim = getDimensions(thing);
    
    var highlight = newPopover(cur, "bottom");
    var arrow = highlight.find(".arrow");
    var inner = highlight.find(".popover-inner");
    
    // Full width:
    if (typeof(cur.args.width) === 'string' && cur.args.width.toLowerCase() === 'full') {
      var padding = 20;
      var page_w = $(document).innerWidth() - padding * 2 - 10; // 10 extra to account for styles.

      arrow.css("position", "absolute");
      arrow.css("left", (dim.l + dim.w/2) + "px");

      highlight.css("position", "absolute");
      highlight.css("left", padding + "px");
      inner.css("width", (page_w) + "px");
      
      highlight.css("top", (dim.t+dim.h+5) + "px");

    // Else, this uses an absolute size:
    } else {
      highlight.css("position", "absolute");
      inner.css("width", cur.args.width + "px");
      highlight.css("left", dim.l - highlight.width()/2 + dim.w/2 - 8 );
      
      
      highlight.css("top", (dim.t+dim.h+5) + "px");
    }
  }


  function drawLeft(cur) {
    var thing = $(cur.args.filter).filter(":visible");
    if (thing.length == 0) { return; }

    var dim = getDimensions(thing);
    
    var highlight = newPopover(cur, "left");
    var arrow = highlight.find(".arrow");
    var inner = highlight.find(".popover-inner");

    highlight.css("position", "absolute");
    inner.css("width", cur.args.width + "px");

    highlight.css("left", dim.l - highlight.width() - 20);
    highlight.css("top", dim.t - highlight.height()/2 + 4);
  }


  function drawRight(cur) {
    var thing = $(cur.args.filter).filter(":visible");
    if (thing.length == 0) { return; }

    var dim = getDimensions(thing);

    var highlight = newPopover(cur, "right");
    var arrow = highlight.find(".arrow");
    var inner = highlight.find(".popover-inner");

    highlight.css("position", "absolute");
    inner.css("width", cur.args.width + "px");

    highlight.css("left", dim.l + dim.w);
    highlight.css("top", dim.t - highlight.height()/2 + 4);
  }


  function drawCenter(cur) {

    var highlight = newPopover(cur);
    var inner = highlight.find(".popover-inner");

    var page_l = ($(document).innerWidth() - cur.args.width)/2; // 10 extra to account for styles.

    highlight.css("position", "absolute");
    highlight.css("top", "100px");
    highlight.css("left", page_l + "px");
    inner.css("width", cur.args.width + "px");
  }


  var drawTutorial = tutorial.redraw = function drawTutorial() {
    var path = cookieRead('wiz_path');
    var stage = cookieRead('wiz_stage');

    // Somehow configured incorrectly.
    if ( typeof(__config) !== 'object' || !(path in __config) || !(stage in __config[path]) ) {
      // Make sure nothing is visible, and return:
      var highlight = $("#" + HIGHLIGHT_ID);
      if (highlight.length > 0) {
        highlight.css("display", "none");
      }

      return;
    }

    var current = __config[path][stage];
    var render_funcs = {
      "under":  drawUnder,
      "left":   drawLeft,
      "right":  drawRight,
      "center": drawCenter
    };

    render_funcs[current.type](current);
  };

})(jQuery);
