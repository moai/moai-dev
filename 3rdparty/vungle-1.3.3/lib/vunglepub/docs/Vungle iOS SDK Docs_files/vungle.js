// Global definitions
if (typeof String.prototype.startsWith !== 'function') {
  String.prototype.startsWith = function (str){
    return this.indexOf(str) === 0;
  };
}
// -- END Global definitions

if (typeof Vungle === 'undefined') {
  Vungle = {};
}

function isEmptyObject(obj) {
  for(var prop in obj) {
    if (Object.prototype.hasOwnProperty.call(obj, prop)) {
      return false;
    }
  }
  return true;
}

Vungle.collapseAndNotify = function (id) {
  var el = $("#" + id);
  el.append('<p>No data to plot</p>');
  el.height('auto');
};

Vungle.initPopovers = function() {
  $('.pop').popover({
    delay: { hide: 700 },
    placement: 'top'
  });
};

Vungle.initTooltips = function() {
  var tooltips = $('.tooltip');
  if (tooltips && tooltips.length > 0) {
    tooltips.tooltip({
      delay: { show: 100, hide: 300 }
    });
  }
  var tt = $('.tt');
  if (tt && tt.length > 0) {
    tt.tooltip({
      delay: { show: 100, hide: 300 }
    });
  }
};

Vungle.initTypeaheads = function() {
  var types = $('.typeahead')
  if (types && types.length > 0) {
    types.typeahead(); 
  }
};

Vungle.initAffix = function() {
  var a = $('#affix-me');
  a.height(a.height());
  a.width(a.width());
  if (a) {
    a.affix({
      offset: {
        top: 69
      }
    })
  }
};

Vungle.init = function() {
  $(function() {
    Vungle.initPopovers();
    Vungle.initTooltips();
    Vungle.initAffix();

    $('.localized').each(function() {
      var $this = $(this);
      var m = moment($this.text());
      if ($this.hasClass('short')) {
        $this.text(m.format('YYYY-MM-DD'));
      } else {
        $this.text(m.format('YYYY-MM-DD hh:mm A'));
      }
    });
  });
};

Vungle.init();
