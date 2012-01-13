//bind from prototype
Function.prototype.bind = function(scope) {
  var boundContext = this;
  return function() {
    return boundContext.apply(scope, arguments);
  }
}
