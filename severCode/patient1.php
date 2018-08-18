<meta name="viewport" content="width=device-width, initial-scale=0.75, maximum-scale=0.8, minimum-scale=0, user-scalable=no, target-densitydpi=medium-dpi" />
<?php

$mysql_host = 'localhost';
$mysql_user = 'doctor';
$mysql_password = 'doctor';
$mysql_db = 'hospital';

// 접속
$conn = mysqli_connect($mysql_host, $mysql_user, $mysql_password, $mysql_db);

$sql="SELECT * from (SELECT * FROM patient1 ORDER BY time DESC LIMIT 15) as a order by time ASC";

$result = mysqli_query($conn, $sql) ;

$str_time="";
$str_atemper="";
$str_heart="";
while ($row = mysqli_fetch_array($result)) {
// echo($row['time']."--------------".$row['temperature']."<br>");
 $str_time .="'".$row['time']."',";
 $str_atemper .="".$row['temp'].",";
 $str_heart .="".$row['heart_rate'].",";
}
$str_time= substr($str_time,0,-1);
$str_atemper= substr($str_atemper,0,-1);
$str_heart= substr($str_heart,0,-1);
//echo $str_atemper;

include_once('./alarm_patient1.php');

?>
<!DOCTYPE HTML>
<html>
<title> Patient1 </title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<style=font-weight:bold;> 
<hr size="3px" color="black"><br>
<font size="4" align="center"><p><p><p><p><p><p> 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 환자 1 님의 개인 데이터<br><br>

&nbsp;&nbsp;&nbsp;&nbsp; NAME : KIM MAN DUK <br>
&nbsp;&nbsp;&nbsp;&nbsp; AGE  : 50<br>
&nbsp;&nbsp;&nbsp;&nbsp; BLOOD TYPE : A <br>
<div align="center"><img src="man.png" width="80px" height="120px" align="center"></div>
<hr size="3px" color="black"><br>
<pre size="10px"><span style=font-weight:bold;> REAL-TIME STATE <br>

&nbsp;&nbsp;&nbsp;&nbsp;
 HEART RATE<br>
<div id="container" style="min-width: 400px; height: 400px; margin: 0 auto"></div>

  <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
  <style type="text/css">
	${highcharts.css}
  </style>

  <link rel="stylesheet" type="text/css" href="./highchart/code/css/highcharts.css"/>
  <script type="text/javascript">
$(function () {
    $('#container').highcharts({
        chart: {
            type: 'line'
//	    backgroundColor: 'black'
        },
        title: {
            text: 'Heart Rate'
        },
        subtitle: {
            text: 'Source: ilikesan.com'
        },
        xAxis: {
            categories: [<?php echo $str_time?>]
        },
        yAxis: {
            title: {
                text: ' 실시간 심박수 '
            }
        },
        plotOptions: {
            line: {
                dataLabels: {
                    enabled: true
                },
                enableMouseTracking: false
            }
        },
        series: [{
            name: '환자1',

            data: [<?php echo $str_heart?>]
        }
  ]
    });
});
  </script>

 
BODY TEMPERATURE <br>

<div id="container2" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
 <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>

 <style type="text/css">
        ${highcharts.css}
  </style>

  <link rel="stylesheet" type="text/css" href="./highchart/code/css/highcharts.css"/>
  <script type="text/javascript">
$(function () {
    $('#container2').highcharts({
        chart: {
            type: 'line'
//          backgroundColor: 'black'
        },
        title: {
            text: 'Body Temperature'
        },
        subtitle: {
            text: 'Source: ilikesan.com'
        },
        xAxis: {
            categories: [<?php echo $str_time?>]
        },
        yAxis: {
            title: {
                text: ' 실시간 Temperature (°C)'
            }
        },
        plotOptions: {
            line: {
                dataLabels: {
                    enabled: true
                },
                enableMouseTracking: false
            }
        },
        series: [{
            name: '환자1',
            data: [<?php echo $str_atemper?>]
        }
  ]
    });
});
  </script>


<script src="./highchart/code/js/highcharts.js"></script>
<script src="./highchart/code/js/modules/exporting.js"></script>


</html>

<?php
$refresh_time="3";// 여기에 몇초마다 refresh 할지를 지정하세요^^*
echo "<script language=\"javascript\">setTimeout(\"location.reload()\",".($refresh_time*1000).");</script>";
?>

