<?php

$servername = "localhost";
$username = "root";
$password = "mysqlt0st";
$dbname = "shop_sense";

function mysqlConnect()
{
   global $servername, $username, $password, $dbname;

   // Create connection
   $conn = new mysqli($servername, $username, $password, $dbname);

   // Check connection
   if ($conn->connect_error)
   {
      die("Connection failed: " . $conn->connect_error);
   }

   return ($conn);
}

function getSensorReading($sensorId)
{
	$reading = 0;

	// Connect to database.
	$conn = mysqlConnect();

	// Construct the query.
	$sql = "SELECT sensor_reading FROM data ORDER BY id DESC LIMIT 1";

	// Retrieve from database.
	$result = $conn->query($sql);

	if ($result->num_rows > 0)
	{
		$row = $result->fetch_assoc();
      $reading = $row['sensor_reading'];
	}

   // Close the connection.
   $conn->close();

	return (intval($reading));
}

function is_ajax()
{
	return (isset($_SERVER['HTTP_X_REQUESTED_WITH']) &&
			  (strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest'));
}

if (is_ajax())
{
	$return = array("status" => getSensorReading("vibration_01"));

	echo json_encode($return);
}
else
{
	echo ("Only AJAX requests accepted.");
}

?>
