<?php

   // Prevent mysql injection!  Use 
   // $name_bad = mysql_real_escape_string($name_bad); to remove quotes in the arguments passed to this URL!

   header('Content-Type: text/plain; charset=utf-8');

//   $header = $_GET['header'];
$timestp = $_GET["timestamp"];

echo $timestp;

        $conn = new mysqli('localhost','vmail','vmail','magma');

        //How's the connection?
	if($conn->connect_error)
	{
		trigger_error('Connection to DB failed: ' . $conn->connect_error, E_USER_ERROR);
	}


	$sql="SELECT msgid,headerhash FROM msg;"; 

	$result = mysqli_query($conn, $sql);
 
        if($conn->query($sql) === false)
	{
		trigger_error('Wrong SQL: ' . $sql . ' ERROR: ' . $conn->error, E_USER_ERROR);
	}
	else while ($row = mysqli_fetch_array($result, MYSQLI_ASSOC))
	{
		//Report data back to client
		//remove inbox/ part
		$hdr = substr($row['headerhash'],6);
		$mid = $row['msgid'];
		echo $mid,",",$hdr;
		echo "\n";
	}
	
?>
