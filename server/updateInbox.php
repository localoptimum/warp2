<?php


   // Prevent mysql injection!  Use 
   // $name_bad = mysql_real_escape_string($name_bad); to remove quotes in the arguments passed to this URL!

   header('Content-Type: text/plain; charset=utf-8');

   $header = $_GET['header'];
   $message = $_GET['message'];
   $attach = $_GET['attachment'];
//   $token  = $_GET['token'];

// Check the stuff to make sure it is sensible

   $headerFile = "inbox/".$header;
   $messageFile = "inbox/".$message;
   $attachFile = "inbox/".$attach;

   $headerExists = file_exists($headerFile);
   $messageExists  = file_exists($messageFile);
   $attachExists = file_exists($attachFile);

   if($headerExists && $messageExists)
   {
	// OK we are good to assemble MSQL
       
        $conn = new mysqli('localhost','vmail','vmail','magma');

        //How's the connection?
	if($conn->connect_error)
	{
		trigger_error('Connection to DB failed: ' . $conn->connect_error, E_USER_ERROR);
	}

	$headerSQL = "'" . mysqli_real_escape_string($conn, $headerFile) . "'";
	$messageSQL = "'" . mysqli_real_escape_string($conn, $messageFile) . "'";

	if($attachExists)
	{
		$attachSQL = "'" . mysqli_real_escape_string($conn, $attachFile) . "'";
		$sql="INSERT INTO msg (timestamp, headerhash, msghash, atthash) VALUES(NOW(), $headerSQL, $messageSQL, $attachSQL);";
	}
	else
	{
		$sql="INSERT INTO msg (timestamp, headerhash, msghash, atthash) VALUES(NOW(), $headerSQL, $messageSQL, NULL);";
 	}

        if($conn->query($sql) === false)
	{
		trigger_error('Wrong SQL: ' . $sql . ' ERROR: ' . $conn->error, E_USER_ERROR);
	}
	else
	{
		$affected_rows = $conn->affected_rows;
		echo $affected_rows;
	}
	
   }

?>
