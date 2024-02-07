<?php
	include_once("top.php");

	if (isset($CURR_ENV['REQUEST_METHOD']) && $CURR_ENV['REQUEST_METHOD'] == "POST"
		&& isset($CURR_ENV['BTN_LOGOUT']))
	{
		$connection = getDatabaseConnection();
		if ($connection == null)
		{
			put_error("Failed to connect to database\n");
			goto end;
		}
		$sql = "DELETE FROM session WHERE session_hash = :session_hash";
		$stmt = $connection->prepare($sql);
		$stmt->bindParam(":session_hash", $CURR_ENV['SESSION_HASH'], SQLITE3_TEXT);
		$stmt->execute();
		set_cookie($CURR_ENV['CLIENT_FD'], "SESSION_HASH", 0, 7);
		end:
		$connection = null;
	}
?>
