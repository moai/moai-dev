<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
	
<html>
<head>
<title>Test report</title>

<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js"/>

<script type="text/javascript">

var hideTests = '[Hide tests]';
var showTests = '[Show tests]';

var hideAsserts = '[Hide Assert Summary]';
var showAsserts = '[Show Assert Summary]';

var hideAllTests = '[Hide All Tests]';
var showAllTests = '[Show All Tests]';

var hideEverything = '[Hide Everything]';
var showEverything = '[Show Everything]';

var animationSpeed = 'fast';

$(document).ready(function() {
	$("span.show-tests").click(function() {
		var content = $(this).html();
		var id = $(this).attr('uid');
		var searchString = "div.tests[uid="+id+"]";
	
		if(content == hideTests) {
			$(searchString).hide(animationSpeed);
			$(this).text(showTests);
		} else {
			$(searchString).show(animationSpeed);
			$(this).text(hideTests);
		}
	});

	$("span.show-asserts").click(function() {
		var content = $(this).html();
		var id = $(this).attr('uid');
		var searchString = "div.asserts[uid="+id+"]";
		
		if(content == hideAsserts) {
			$(searchString).hide(animationSpeed);
			$(this).text(showAsserts);
		} else {
			$(searchString).show(animationSpeed);
			$(this).text(hideAsserts);
		}
	});
	
	$("span.show-all-tests").click(function() {
		var content = $(this).html();
		var searchString = "div.tests";
		
		if(content == hideAllTests) {
			$(searchString).hide(animationSpeed);
			$(this).text(showAllTests);
			
			/* handle the individual '[show tests]' switcher */
			$("span.show-tests[uid]").text(showTests);
		} else {
			$(searchString).show(animationSpeed);
			$(this).text(hideAllTests);
			
			/* handle the individual '[show tests]' switcher */
			$("span.show-tests[uid]").text(hideTests);
		}
	});
	
	$("span.show-everything").click(function() {
		var content = $(this).html();
		var searchString = "div.tests";
		
		if(content == hideEverything) {
			$("div.tests").hide(animationSpeed);
			$("div.asserts").hide(animationSpeed);
			$(this).text(showEverything);
			
			/* handle the individual switchers */
			$("span.show-tests[uid]").text(showTests);
			$("span.show-asserts[uid]").text(showAsserts);
		} else {
			$("div.tests").show(animationSpeed);
			$("div.asserts").show(animationSpeed);
			$(this).text(hideEverything);
			
			/* handle the individual switchers */
			$("span.show-tests[uid]").text(hideTests);
			$("span.show-asserts[uid]").text(hideAsserts);
		}
	});

	/* Initially everything is hidden */
	$("div.tests").hide();
	$("div.asserts").hide();
	
	/* Color the tests based on the result */
	$("div.test[result='passed']").addClass('passed');
	$("div.test[result='failed']").addClass('failed');
	
	/* Color the asserts based on the result */
	$("div.assert[result='pass']").addClass('passed');
	$("div.assert[result='failure']").addClass('failed');
});

</script>
<style>

div, h1 {
  padding: 3px 10px 2px 10px;
  margin: 5px 0px 5px 0px;
}

.document {
  font-family: Arial;
  font-size: 11pt;
  background-color: #EDEDED;
}
	
.title {
  font-weight: bold; 
}	
</style>
	
</head>
<body class="document">
  <h1>Test Report</h1>
  <div>
    <span class="title">Start time: </span><xsl:value-of select="testlog/startTime"/><br/>
    <!-- and ended at <xsl:value-of select="testlog/endTime"/>.<br/>-->
    <span class="title">Total runtime: </span><xsl:value-of select="testlog/totalRuntime"/> seconds.<br/>
    <span class="title">Harness parameters: </span>
	<span xml:space="preserve">
      <xsl:for-each select="testlog/parameters/parameter">
        <xsl:value-of select="."/> 	 
      </xsl:for-each>
    </span>
	<br/>
    <span class="title">Statistics:</span><br/>
    <div>
      <span>Executed </span> <xsl:value-of select="testlog/numSuites"/> test suites. <br/>
      <span>Tests in total: </span> <xsl:value-of select="testlog/numTests"/> (passed: <xsl:value-of select="testlog/numPassedTests"/>, failed: <xsl:value-of select="testlog/numFailedTests"/>)
    </div>
  </div>

</body>
</html>

</xsl:template>
</xsl:stylesheet>

