<?php
$servername = "localhost";
$username = "root";
$password = "mysqlt0st";
$dbname = "shop_sense";

handleCommand($_GET["command"]);

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

function handleCommand($command)
{
  echo "handleCommand";

  switch ($command)
  {
     case "register":
     {
        register();
        break;
     }

     case "data":
     {
        data();
        break;
     }

     case "poll":
     {
        break;
     }

     default:
     {
        break;
     }
  }
}

function register()
{
   echo "register";

   // Validate parameters.
   if (array_key_exists("sensor_id", $_GET))
   {
      $sensorId = $_GET["sensor_id"];

      if (!sensorExists($sensorId))
      {
         addSensor($sensorId);

         configureSensor($sensorId,  // sensorId
                         "unknown",  // machineId
                         1,          // sensorType
                         10,         // pollRate
                         true);     // isEnabled
      }
   }
}

function data()
{
   echo "data";

   // Validate parameters.
   if ((array_key_exists("sensor_id", $_GET)) &&
       (array_key_exists("sensor_reading", $_GET)))
   {
      // Extract parameters from GET.
      $sensorId = $_GET["sensor_id"];
      $sensorReading = intval($_GET["sensor_reading"]);

      // Connect to database.
      $conn = mysqlConnect();

      // Construct the query.
      $sql = sprintf("INSERT INTO %s (%s, %s, %s) VALUES ('%s', %s, %d)", "data", "sensor_id", "timestamp", "sensor_reading", $sensorId, "CURRENT_TIMESTAMP", $sensorReading);
      echo "\n$sql\n";

      // Insert into database.
      if ($conn->query($sql) === TRUE)
      {
         echo "Record updated successfully";
      }
      else
      {
         echo "Error updating record: " . $conn->error;
      }

      // Close the connection.
      $conn->close();
   }
}

function poll()
{
}

function sensorExists($sensorId)
{
   // Connect to database.
   $conn = mysqlConnect();

   // Construct the query.
   $sql = "SELECT 1 FROM sensor WHERE sensor_id='$sensorId' LIMIT 1";

   // Querty the database.
   $result = $conn->query($sql);

   $sensorExists = ($result->num_rows == 1);

   // Close the connection.
   $conn->close();

   return ($sensorExists);
}

function addSensor($sensorId)
{
   // Connect to database.
   $conn = mysqlConnect();

   // Construct the query.
   $sql = "INSERT INTO sensor (sensor_id) VALUES ('$sensorId')";

   // Query the database.
	if ($conn->query($sql) === TRUE)
	{
	 echo "Record updated successfully";
	}
	else
	{
	 echo "Error updating record: " . $conn->error;
	}

   // Close the connection.
   $conn->close();
}

function configureSensor($sensorId, $machineId, $sensorType, $pollRate, $isEnabled)
{
   // Connect to database.
   $conn = mysqlConnect();

   // Construct the query.
   $sql = "UPDATE sensor SET machine_id='$machineId', sensor_type=$sensorType, poll_rate=$pollRate, is_enabled=" . var_export($isEnabled, true) . " WHERE sensor_id='$sensorId'";

   // Query the database.
	if ($conn->query($sql) === TRUE)
	{
	 echo "Record updated successfully";
	}
	else
	{
	 echo "Error updating record: " . $conn->error;
	}

   // Close the connection.
   $conn->close();
}

?>