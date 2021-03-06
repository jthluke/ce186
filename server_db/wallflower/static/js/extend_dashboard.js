var ul = $('ul#side-menu');
$.ajax({
    url: '/static/extend_dashboard_links.html',
    type: "get", success: function(response){console.log("Load /static/extend_dashboard_links.html");
        ul.append(response);
    }
});

var wrapper = $('div#wrapper');
$.ajax({
    url: '/static/extend_dashboard_pages.html',
    type: "get",
    success: function(response){
        console.log("Load /static/extend_dashboard_pages.html");
        wrapper.append(response);

        // For submit call
        $("form#form-input").submit(onInputFormSubmit);
    }
});

/*
    Add functionality to the input page form 
*/ 
function onInputFormSubmit(e){
    e.preventDefault();
    var network_id = "local";
    var object_id = "userTripData";
    var stream_rating_id = "rating";
    var stream_comment_id = "comments";

    //Gather the data 
    //and remove any undefined keys
    var data = {};
    $('input',this).each(function(i,v){
        var input=$(v);
        data[input.attr("name")] = input.val();
        });
    delete data["undefined"];

    console.log(data["rating"]);

    var url='/networks/'+network_id+'/objects/';
    url = url + object_id + '/streams/'+stream_rating_id+'/points';
    var query = {
        "points-value": JSON.stringify(parseFloat(data["rating"]))
        };
    // console.log(url)
    // console.log(query)
    //Send request to Pico server
    $.ajax({
        url: url+'?'+$.param(query),
        type: "post",
        success: function(response){
        var this_form = $("form#form-input");

        if(response['points-code'] == 200){
            console.log("Success");
            //Clear the form
            this_form.trigger("reset");
            }
            //log the response to the console
            console.log(response);
            },
            error: function(jqXHR, textStatus, errorThrown){
                //Do nothing
            }
        });

    var url='/networks/'+network_id+'/objects/';
    url = url + object_id + '/streams/'+stream_comment_id+'/points';
    var query = {
        "points-value": JSON.stringify(data["comments"])
        };
    console.log(url)
    console.log(data)
    //Send request to Pico server
    $.ajax({
        url: url+'?'+$.param(query),
        type: "post",
        success: function(response){
        var this_form = $("form#form-input");

        if(response['points-code'] == 200){
            console.log("Success");
            //Clear the form
            this_form.trigger("reset");
            }
            //log the response to the console
            console.log(response);
            },
            error: function(jqXHR, textStatus, errorThrown){
                //Do nothing
            }
        });
    };


function getPoints(the_network_id, the_object_id, the_stream_id, callback){
    var query_data = {};
    var query_string = "?" + $.param(query_data);
    var url = '/networks/'+the_network_id+'/objects/'+the_object_id;
    url += '/streams/'+the_stream_id+'/points'+query_string;

    $.ajax({
        url: url,
        success: function(response) {
        console.log(response);

        if(response['points-code'] == 200) {
            var num_points = response.points.length
            var most_recent_value = response.points[0].value
            console.log("Most recent value: "+most_recent_value);
            console.log("Number of points retrieved: "+num_points);
            callback(response.points);
            }
        },
        error: function(jqXHR, textStatus, errorThrown){
        }
    });
}

custom_sidebar_link_callback = function( select ){
    if (select == 'input') {
    }
    else if (select == 'report'){
        var plotTimer = setInterval( function(){
            getPoints('local','userTripData','proximity', function(points){ 
                console.log( "Proximity points request was successful!" );
            loadPlot( points ); });
        }, 1000);
    }
    else if (select == 'trip') {
        console.log("loading..")
        window.open("http://127.0.0.1:8000/static/showmap.html", 'newwin');

    }
}

function loadPlot( points ){
    var plot = $('#content-report');
    // Check if plot has a Highcharts element
    if( plot.highcharts() === undefined ){
        // Create a Highcharts element
        plot.highcharts( report_plot_options );
    }
        // Iterate over points to place in Highcharts format
    var datapoints = [];
    for ( var i = 0; i < points.length; i++){
        var at_date = new Date(points[i].at);
        var at = at_date.getTime() - at_date.getTimezoneOffset()*60*1000;
        datapoints.unshift( [ at, points[i].value] );
    }
    // Update Highcharts plot
    if( plot.highcharts().series.length > 0 ){
        plot.highcharts().series[0].setData( datapoints );
    }else{
        plot.highcharts().addSeries({
            name: "Proximity (m)",
            data: datapoints
        });
    }
}
var report_plot_options = { 
    chart: { 
        type: 'spline',
        },
    title: {
            text: 'How close are you to the automobiles while you are biking?'
        },
    yAxis: {
        title: {
            text: 'Distance (m)',
            },
        type: 'category', 
        },
    xAxis: { 
        type: 'datetime',  
        title: {
            text: 'Time during trip'
            },
        dateTimeLabelFormats: { // don't display the dummy year
            month: '%e. %b',
            year: '%b' 
            },
        }
    };