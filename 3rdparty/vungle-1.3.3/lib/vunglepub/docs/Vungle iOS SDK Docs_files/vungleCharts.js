
if (typeof VungleCharts === 'undefined') {
  VungleCharts = {};
}

var charts = {};

function makeDateString(str) {
  var d = new Date(str);
  
  var datestr = ["Sun, ", "Mon, ", "Tue, ", "Wed, ", "Thu, ", "Fri, ", "Sat, "][d.getUTCDay()];
  datestr += d.getUTCDate() + " ";
  datestr += ["Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ", "Jul ", "Aug ", "Sep ", "Oct ", "Nov ", "Dec "][d.getUTCMonth()];
  datestr += d.getUTCFullYear();

  return datestr;
}

function parseDate(str) {
  // str is a date formatted like YYYY-MM-DD:
  var parsed = str.split("-");
  var args = [ parseInt(parsed[0],10), parseInt(parsed[1],10)-1, parseInt(parsed[2],10) ].concat([0,0,0,0]);
  return Date.UTC.apply(Date, args);
}


function padData(data) {
  var paddedData = [];
  var j = 0;
  if(data[0][0] > data[data.length-1][0]) {
    data = data.reverse();
  }
  var date = new Date(data[0][0]);
  for(var i = date; i < new Date(); i.setDate(i.getDate() + 1)) {
    if(j < data.length && (new Date(data[j][0]).toDateString()) == (i.toDateString())) {
      if(isNaN(data[j][1])) {
        paddedData.push([data[j][0], 0]);
      } else {
        paddedData.push([data[j][0], data[j][1]]);
      }
      j++;
    } else {
      paddedData.push([i.getTime(), 0]);
    }
  }
  return paddedData;
}


/*
* options:
* axis: 0[default], 1, 2, 3, 4, 5. Decides which axis to draw. 1-topleft 2-topright 3-bottomleft 4-bottomright 5-bottomleft% 6-bottomright%
* double: true, false[default]. Enables two panes and opens axis 2, 3, 4, and 5 for graphing.
* color: Auto-assigned by default.
* modifier: %, $. Adds formatting options for tooltip.
* type: spline[default], bar, line, scatter
*/
VungleCharts.renderStockChart = function(id, name, data, options) {
  var options = options || {};
  if (!$('#' + id)) {
    return;
  } 
  if(!data || data.length === 0) {
    $('#' + id).text("No data to plot.");
    return;
  }
  var series = {
    name: name,
    data: padData(data),
    yAxis: options.axis ? options.axis : 0,
    type: options.type ? options.type : 'spline'
  };
  if(options.color) {
    series.color = options.color;
  }

  // show div before rendering to grab correct width
  $('#' + id).css({'visibility':'hidden','display':'block'});
  var width = $('#' + id).width();
  $('#' + id).removeAttr('style');

  var singlePane = options.double ? false : true;
  var height = (singlePane) ? 400 : 600;

  var now = new Date();
  now.setMinutes(0);
  now.setHours(0);

  if(singlePane) {
    var axes = [{
      min: 0,
    },{
      min: 0,
      opposite: true
    }];
  } else {
    var axes = [{
      height: 236,
      min: 0,
      lineWidth: 2
    },{
      height: 236,
      opposite: true,
      min: 0,
      lineWidth: 2
    },{
      top: 333,
      height: 174,
      min: 0,
      lineWidth: 2
    },{
      top: 333,
      height: 174,
      min: 0,
      lineWidth: 2,
      opposite: true
    },{
      // for percentages
      height: 236,
      min: 0,
      max: 100,
      tickInterval: 20,
      labels: {
        formatter: function() {
          return this.value + "%";
        }
      }
    },{
      // for percentages
      top: 333,
      height: 174,
      min: 0,
      max: 100,
      tickInterval: 20,
      labels: {
        formatter: function() {
          return this.value + "%";
        }
      }
    }];
  }

  if(charts[id]) {
    charts[id].addSeries(series);
  } else {
    charts[id] = new Highcharts.StockChart({
      legend: {
        enabled: true,
        align: 'center',
        verticalAlign: 'top',
      },
      chart: {
        renderTo: id,
        height: height,
        zoomType: "x",
        width: width
      },

      rangeSelector: { enabled: false },

      credits: { enabled: false },
      xAxis: {
        dateTimeLabelFormats: {
          day: '%m/%d'
        },
        min : now.getTime() - 14*24*3600000, // set viewing window to 14 days
        max : now.getTime()
      },
      yAxis: axes,
      tooltip: {
        formatter: function() {

          var retval = "<em>" + makeDateString(this.x) + "</em>"
          for(var i = 0; i < this.points.length; i++) {
            if(options.modifier && options.modifier === '$') {
              retval += "<br/><b><span style=\"color:" + this.points[i].series.color + "\">" + 
              this.points[i].series.name + "</span>:</b> $" + (Math.round(this.points[i].point.y*100)/100).toFixed(2);
            } else if(options.modifier && options.modifier === '%') {
              retval += "<br/><b><span style=\"color:" + this.points[i].series.color + "\">" + 
              this.points[i].series.name + "</span>:</b> " + (Math.round(this.points[i].point.y*100)/100).toFixed(2) + "%";
            } else {
              retval += "<br/><b><span style=\"color:" + this.points[i].series.color + "\">" + 
              this.points[i].series.name + "</span>:</b> " + Math.round(this.points[i].point.y*100)/100;
            }
          }
          return retval;
        }
      },
      series: [series],
    });
  }
};

/*
* options:
*   xtitle/ytitle/ytitle2: titles of corresponding axes
*   xtype: linear[default], datetime, logarithmic. Controls type of x axis.
*   axis: 0[default] ... 1. Decides which axis to draw data on, 0 being left and 1 being right
*   labels: true, false[default]. If on, points are labeled and mouse tracking is disabled.
*   modifier: %, $. Adds formatting options for tooltip.
*   charttitle: text for the top,
*   ymin, ymax: what you expect.
*/
VungleCharts.renderChart = function(id, name, data, options) {
  if (!$('#' + id)) {
    return;
  }
  if (!data || isEmptyObject(data)) {
    return Vungle.collapseAndNotify(id);
  }
  $('#' + id).css({'visibility':'hidden','display':'block'});
  var width = $('#' + id).width();
  $('#' + id).removeAttr('style');

  if(charts[id]) {
    charts[id].addSeries(
      {
        name: name,
        data: data,
        yAxis: options ? options.axis : 0
      }
    );
  } else {
    charts[id] = new Highcharts.Chart({
      chart: {
        renderTo: id,
        type: 'spline',
        zoomType: 'xy',
        width: width,
      },
      title: options.charttitle ? {text: options.charttitle} : null,
      credits: { enabled: false },
      xAxis: {
        title: options.xtitle ? { text: options.xtitle} : null,
        type: (options && options.xtype) ? options.xtype : 'linear',
        dateTimeLabelFormats: {
          day: '%m/%d'
        }        
      },
      yAxis: [
        {
          title: options.ytitle ? { text: options.ytitle} : null,
          min: options.ymin || 0,
          max: options.ymax || undefined
        },{
          title: options.ytitle2 ? { text: options.ytitle2} : null,
          min: 0,
          opposite: true
        }
      ],
      plotOptions: {
        line: {
          dataLabels: {
            enabled: true,
            formatter: function() {
              return Math.round(this.y*100)/100;
            }
          },
          enableMouseTracking: true
        },
        spline: {
          dataLabels: {
            enabled: (options && options.labels === true) ? true : false,
            formatter: function() {
              return Math.round(this.y*100)/100;
            }
          },
          enableMouseTracking: (options && options.labels == true) ? false : true
        }
      },
      series: [{
        name: name,
        data: data,
        yAxis: options ? options.axis : 0
      }],
      tooltip: {
        formatter: function() {
          if(options) {
            if(options.xtype === 'datetime') {
              var retval = "<em>" + makeDateString(this.x) + "</em>";
            } else if(options.xtitle) {
              var retval = "<em>" + options.xtitle + ": " + this.x + "</em>";
            }
            if(options.modifier === '$') {
              retval += "<br/><b><span style=\"color:" + this.series.color + "\">" + 
              this.series.name + "</span>:</b> $" + this.y.toFixed(2);
            } else if(options.modifier === '%') {
              retval += "<br/><b><span style=\"color:" + this.series.color + "\">" + 
              this.series.name + "</span>:</b> " + this.y.toFixed(2) + '%';
            } else {
              retval += "<br/><b><span style=\"color:" + this.series.color + "\">" + 
              this.series.name + "</span>:</b> " + this.y;
            }
            return retval;
          }
          return this.y;
        }
      }
    });
  }
};

VungleCharts.renderEmptyChart = function(id) {
  this.renderStockChart(id, "", []);
};

VungleCharts.renderDateChart = function(json, id, seriesName) {
  var line = [];
  json.data.forEach(function(row) {
    line.push([parseDate(row.start), row.count]);
  });
  this.renderStockChart(id, seriesName, line);
};

VungleCharts.renderRevenue = function(json, id) {
  var line = [];
  var revenue;
  for(var i = 0; i < json[0].length; i++) {
    if(json[0][i] == "Revenue") {
      revenue = i;
    }
  }
  for(var i = 1; i < json.length; i++) {
    var row = json[i];
    line.push([parseDate(row[0]), parseFloat(row[revenue].substring(1, row[revenue].length-1))]);
  }
  this.renderStockChart(id, "Revenue", line, {'modifier': '$'});
};

VungleCharts.renderECPM = function(json, id) {
  var line = [];
  var revenue, impressions;
  for(var i = 0; i < json[0].length; i++) {
    if(json[0][i] == "Revenue") {
      revenue = i;
    } else if(json[0][i] == "Impressions") {
      impressions = i;
    }
  }
  for(var i = 1; i < json.length; i++) {
    var row = json[i];
    line.push([parseDate(row[0]), parseFloat(row[revenue].substring(1, row[revenue].length-1))*1000/parseInt(row[impressions])]);
  }
  this.renderStockChart(id, "eCPM", line, {'modifier': '$'});
};


VungleCharts.renderViews = function(json, id) {
  var line = [];
  var views;
  for(var i = 0; i < json[0].length; i++) {
    if(json[0][i] == "Views") {
      views = i;
    }
  }
  for(var i = 1; i < json.length; i++) {
    var row = json[i];
    line.push([parseDate(row[0]), row[views]]);
  }
  this.renderStockChart(id, "Views", line);
};

VungleCharts.renderFillRate = function(json, id) {
  var line = [];
  for(var i = 0; i < json[0].length; i++) {
    var requests = json[0][i];
    var unfilled = json[3][i];
    var requestsCount = parseFloat(requests[1]);
    var fillRate = (requestsCount-parseFloat(unfilled[1]))*100/requestsCount;
    if(isNaN(fillRate)) {
      fillRate = 0;
    }
    line.push([requests[0], fillRate]);
  }
  this.renderStockChart(id, "Fill Rate", line, {'modifier': '%'});
};

VungleCharts.renderOverall = function(json, id) {
  // [[date, value]...]
  var revenueData = [],
      payoutData = [],
      profitData = [],
      conversionData = [],
      impressionData = [],
      clickData = [],
      rpmData = [],
      cpmData = [];
  var ad_revenue, impressions, pub_payout, conversions, clicks;
  for(var i = 0; i < json[0].length; i++) {
    if(json[0][i] == "Ad Revenue") {
      ad_revenue = i;
    } else if(json[0][i] == "Impressions") {
      impressions = i;
    } else if(json[0][i] == "Publisher Payout") {
      pub_payout = i;
    } else if(json[0][i] == "Conversions") {
      conversions = i;
    } else if(json[0][i] == "Clicks") {
      clicks = i;
    }
  }
  for(var i = 1; i < json.length; i++) {
    var row = json[i];
    var date = parseDate(row[0]);
    revenueData.push([date, parseFloat(row[ad_revenue].substring(1, row[ad_revenue].length))]);
    payoutData.push([date, parseFloat(row[pub_payout].substring(1, row[pub_payout].length))]);
    profitData.push([date, revenueData[i-1][1] - payoutData[i-1][1]]);
    conversionData.push([date, parseInt(row[conversions])]);
    impressionData.push([date, parseInt(row[impressions])]);
    clickData.push([date, parseInt(row[clicks])]);
    rpmData.push([date, (revenueData[i-1][1] / impressionData[i-1][1]) * 1000]);
    cpmData.push([date, (payoutData[i-1][1] / impressionData[i-1][1]) * 1000]);
  }
  this.renderStockChart(id, "Clicks", clickData.reverse(), {'double': true});
  this.renderStockChart(id, "Impressions", impressionData.reverse(), {'axis': 1, 'double': true});
  this.renderStockChart(id, "Conversions", conversionData.reverse(), {'double': true});
  this.renderStockChart(id, "Ad Revenue ($)", revenueData.reverse(), {'axis': 2, 'double': true, 'color': '#23DE3C'});
  this.renderStockChart(id, "Publisher Payout ($)", payoutData.reverse(), {'axis': 2, 'double': true, 'color': '#B51212'});
  this.renderStockChart(id, "Net Profit ($)", profitData.reverse(), {'axis': 2, 'type': 'column', 'double': true});
  this.renderStockChart(id, "Ad RPM", rpmData.reverse(), {'axis': 3, 'double': true, 'color': '#05EDFF'});
  this.renderStockChart(id, "Publisher CPM", cpmData.reverse(), {'axis': 3, 'double': true, 'color': '#ffcc00'});
};

VungleCharts.renderCompletions = function(json, id, jsonUrl) {
  var line = [];
  // find desired columns
  var columnNames = json[0];
  var dateIndex = columnNames.indexOf('Date');
  var completesIndex = columnNames.indexOf('Completes');
  var impressionsIndex = columnNames.indexOf('Impressions');
  // calculate daily completion %
  for(var i = 1; i < json.length; i++) {
    var row = json[i];
    line.push([parseDate(row[dateIndex]), parseInt(row[completesIndex])*100/parseInt(row[impressionsIndex])]);
  }
  this.renderStockChart(id, "Completion %", line, {'modifier': '%'});
};

VungleCharts.renderCampaign = function(json, id) {
  var line = [];
  for(var i = 0; i < 8; i++) {
    line.push([]);
  }
  json.forEach(function(row) {
    var pdate = parseDate(row.date);
    line[0].push([pdate, row.clicks]);
    line[1].push([pdate, row.installs]);
    line[2].push([pdate, row.views]);
    line[3].push([pdate, row.completes]);
    line[4].push([pdate, row.revenue / row.installs]);
    line[5].push([pdate, row.clicks*100/row.views]);
    line[6].push([pdate, row.completes*100/row.views]);
    line[7].push([pdate, row.installs*100/row.views]);
  });
  this.renderStockChart(id, "Clicks", line[0], {'axis': 1, 'double': true});
  this.renderStockChart(id, "Installs", line[1], {'axis': 1, 'double': true});
  this.renderStockChart(id, "Views", line[2], {'double': true});
  this.renderStockChart(id, "Completed Views", line[3], {'double': true});
  this.renderStockChart(id, "CPI ($)", line[4], {'axis': 3, 'double': true});
  this.renderStockChart(id, "CTR (%)", line[5], {'axis': 5, 'double': true});
  this.renderStockChart(id, "Completion Rate (%)", line[6], {'axis': 5, 'double': true});
  this.renderStockChart(id, "Conversion Rate (%)", line[7], {'axis': 5, 'double': true});
}

if (typeof module !== 'undefined') {
  module.exports = VungleCharts;
}