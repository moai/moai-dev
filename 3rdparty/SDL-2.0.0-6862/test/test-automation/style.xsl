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
	$("span.testResult[result='passed']").addClass('passed');
	$("span.testResult[result='failed']").addClass('failed');
	$("span.testResult[result='skipped']").addClass('skipped');
	
	/* Color the asserts based on the result */
	$("span.assertResult[result='pass']").addClass('passed');
	$("span.assertResult[result='failure']").addClass('failed');
});

</script>
<style>

div, h1 {
  padding: 2px 10px 2px 10px;
  margin: 5px 0px 5px 0px;
}

.assert {
  padding: 0px 10px 0px 10px;
  margin: 0px 0px 0px 0px;
}

.document {
  font-family: Arial;
  font-size: 11pt;
  background-color: #EDEDED;
}
	
.description {
  font-style: italic;
}	
	
.bigtitle {
  font-size: 14pt;
  font-weight: bold;
}
	
.title {
  font-weight: bold;
}
	
.switch {
  font-style: italic;
  color: rgb(10, 10, 200); 
  font-size: 10pt;
  cursor: pointer;
}	
	
.passed {
 color: green;
}

.failed {
 color: red;
}

.skipped {
 color: gray;
}

</style>

</head>
<body class="document">
  <h1>Test Report</h1>
  <div>
    <span class="title">Start time: </span><xsl:value-of select="testlog/startTime"/><br/>
    <span class="title">Total runtime: </span><xsl:value-of select="testlog/totalRuntime"/> seconds.<br/>
    <span class="title">Fuzz seed: </span><xsl:value-of select="testlog/seed"/><br/>
    <span class="title">Harness parameters: </span>
	<span xml:space="preserve">
      <xsl:for-each select="testlog/parameters/parameter">
        <xsl:value-of select="."/> 	 
      </xsl:for-each>
    </span>
	<br/>
    <span class="title">Statistics:</span><br/>
    <div class="statistics">
      <span>Executed </span> <xsl:value-of select="testlog/numSuites"/> test suites. <br/>
      <span>Tests in total: </span> <xsl:value-of select="testlog/numTests"/> (passed: <xsl:value-of select="testlog/numPassedTests"/>, failed: <xsl:value-of select="testlog/numFailedTests"/>, skipped: <xsl:value-of select="testlog/numSkippedTests"/>)
    </div>
  </div>
  <div>
	<br/>
	<span class="bigtitle">Test Results</span><br/>
    <span class="switch show-all-tests">[Show All Tests] </span> | 
    <span class="switch show-everything">[Show Everything]</span>
  </div>
	<br/>
	
  <xsl:for-each select="testlog/suite">
    <div id="suite">
    Suite: <span class="title"> <xsl:value-of select="name"/></span> (<xsl:value-of select="startTime"/>)
	<div class="suiteInfo">
		Tests: passed <xsl:value-of select="testsPassed"/>, failed <xsl:value-of select="testsFailed"/>, skipped <xsl:value-of select="testsSkipped"/>.<br/>
		Total runtime: <xsl:value-of select="totalRuntime"/> seconds. <br/>
		<span class="show-tests switch" uid="{generate-id(test)}">[Show tests]</span>
        <div class="tests" uid="{generate-id(test)}">
	      <xsl:for-each select="test">
			<div class="test">
			Test <span class="title"><xsl:value-of select="name"/>: </span>
			<span class="testResult">
			  <xsl:attribute name="result">
			    <xsl:value-of select="result"/> 
	           </xsl:attribute><xsl:value-of select="result"/> 
	        </span> 
			<xsl:if test="resultDescription != ''">
			  <span xml:space="preserve">
			  	 (<xsl:value-of select="resultDescription"/>)  
			  </span>
			</xsl:if>
			- exec-key: <xsl:value-of select="executionKey"/>
			(Total runtime: <xsl:value-of select="totalRuntime"/> seconds)<br/>
			Description: <span class="description"> <xsl:value-of select="description"/> </span><br/>
		      <span class="switch show-asserts" uid="{generate-id(assertSummary)}">[Show Assert Summary]</span><br/>
			  <div class="asserts" uid="{generate-id(assertSummary)}">
		        <xsl:for-each select="assert"> 
			      <div class="assert">
					<xsl:value-of select="name"/>: 
					<span class="assertResult">
						<xsl:attribute name="result">
					      <xsl:value-of select="result"/>
					    </xsl:attribute>
					  <xsl:value-of select="result"/>  
					</span>.
					 Message: <span class="description"><xsl:value-of select="message"/></span>
		          </div>
		        </xsl:for-each>
				Asserts in total: <xsl:value-of select="assertSummary/assertCount"/> (passed: <xsl:value-of select="assertSummary/assertsPassed"/>, failed: <xsl:value-of select="assertSummary/assertsFailed"/>)
		      </div>
    		</div>
		  </xsl:for-each>
	    </div>
	</div>
  </div>
  </xsl:for-each>
  <br/>
</body>
</html>


</xsl:template>
</xsl:stylesheet>

