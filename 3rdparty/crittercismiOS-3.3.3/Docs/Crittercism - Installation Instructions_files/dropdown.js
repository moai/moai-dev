

$(".dropdown").click(function(){
	var thisAttr = $(this).attr("dropdown-link"),
		dList = $("ul.dropdown-list[dropdown="+thisAttr+"]");
	//this doesnt work yet... will add it back in
	//create scroll container if height too big
/*	if(dList.height()>200){
		//does this scroll container already exist? if not make it happen
		if(!dList.parent().hasClass("scrollable-container")){
			$(dList).wrap("<div class='scrollable-container'/>");
			$("<div class='scroll-dot'></div>").prependTo(dList);
		}
	}
	$(".scrollable-container").mouseenter(function(){
		var winY = $(window).scrollTop();
		$(".scrollable-container").bind("mousewheel",function(e){
			var $scrollList = $(this).children("ul");
			var scrollYMax = -($scrollList.height() - 200);
			var wDelta = $(e.originalEvent.wheelDelta)[0];
			wDelta = wDelta.valueOf();
			e.preventDefault();
			$(window).scrollTop(winY);
			console.log($scrollList.position().top);
			if ($scrollList.position().top >= scrollYMax && $scrollList.position().top <= 0){
				$scrollList.animate({
					"top":"+="+wDelta/10+"px"
				}, 50, "swing");
				
			} else if ($scrollList.position().top < scrollYMax){
				$scrollList.animate({
					"top":scrollYMax
				}, 10, "linear");
			} else if ($scrollList.position().top > 0){
				$scrollList.animate({
					"top":0
				}, 10, "linear");
			}
			
		});		
	});
*/
	dList.toggle();
	if (dList.is(":visible")){
		$(this).find(".ui-icon").removeClass("ui-icon-triangle-1-s").addClass("ui-icon-triangle-1-n");
		if (thisAttr === "crash-status"){
			$('#btn-submit').fadeIn("fast");			
		}
		$(this).addClass("txt-box-pressed");
	} else {
		$(this).removeClass("txt-box-pressed");
		$(this).find(".ui-icon").removeClass("ui-icon-triangle-1-n").addClass("ui-icon-triangle-1-s");	
	}
});

$("ul.dropdown-list > li").click(function(){
	var thisVal = $(this).attr("val");
	var txtBox = $(this).parent('ul').attr("dropdown");
	$(this).parent().children().not(this).removeClass();
	$(this).addClass('selected');
	$("[dropdown-link="+txtBox+"]").children(".name").html(thisVal);
	$(this).parent().hide();
	$("[dropdown-link="+txtBox+"]").removeClass("txt-box-pressed");
	$("[dropdown-link="+txtBox+"]").children(".ui-icon").removeClass("ui-icon-triangle-1-n").addClass("ui-icon-triangle-1-s");
	if (txtBox === "crash-status"){
		if (thisVal === "resolved"){
			$("#version_span").fadeIn("fast");
		} else {
			$("#version_span").hide();
		}
	}
	
});
$(document).bind("click",function(e){
	var $clicked = $(e.target);
	
	if ($clicked.parent().hasClass("dropdown") || $clicked.hasClass("dropdown")){
		
	} else if (! $clicked.parents().hasClass("dropdown-list")){
		$("ul.dropdown-list").hide();
		$(".dropdown").removeClass("txt-box-pressed");
		$(".dropdown").find(".ui-icon").removeClass("ui-icon-triangle-1-n").addClass("ui-icon-triangle-1-s");
	
	}
});


