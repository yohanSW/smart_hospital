<?php
	
	$mysql_host = 'localhost';
	$mysql_user = 'doctor';
	$mysql_password = 'doctor';
	$mysql_db = 'hospital';

// ����
	$conn = mysqli_connect($mysql_host, $mysql_user, $mysql_password, $mysql_db);


// charset ����, �������� ������ �⺻ mysql �������� ��, ��ü������ euc-kr�� ���� ���
//mysql_query("set names utf8");

session_start();
$sql="SELECT alarm from (SELECT * FROM patient2 ORDER BY time DESC LIMIT 1) as a order by time ASC";

$result = mysqli_query($conn, $sql) ;

while($arr = mysqli_fetch_array($result)){
	if($arr2[0]=='0')
		echo" GOOD <img src=\"green.png\" width=\"10\"> ";
	else
		echo" EMERGENCY <img src=\"emergency.png\" width=\"10\"> ";
}


<?php
$refresh_time="3";
echo "<script language=\"javascript\">setTimeout(\"location.reload()\",".($refresh_time*1000).");</script>";
?>
