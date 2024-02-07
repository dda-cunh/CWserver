<?php
	include_once("top.php");

	if (isset($CURR_ENV['REQUEST_METHOD']) && $CURR_ENV['REQUEST_METHOD'] == "POST"
		&&( isset($CURR_ENV['BTN_LOGIN']) || isset($CURR_ENV['BTN_REGISTER'])))
	{
		$connection = getDatabaseConnection();
		$user_id = 0;
		if ($connection == null)
		{
			put_error("Failed to connect to database\n");
			goto end;
		}
		if (isset($CURR_ENV['BTN_LOGIN']))
		{
			$password = passHash($CURR_ENV['password']);
			$sql = "SELECT * FROM users WHERE name = :name AND password = :password";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $CURR_ENV['username'], SQLITE3_TEXT);
			$stmt->bindParam(":password", $password, SQLITE3_TEXT);
			$set = $stmt->execute();
			$result = $set->fetchArray();
			if (isset($result['id']) == false)
			{
				put_error("Failed to login\n");
				goto end;
			}
			$user_id = $result['id'];
		}
		else
		{
			$password = passHash($CURR_ENV['password']);
			$sql = "SELECT * FROM users WHERE name = :name";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $CURR_ENV['username'], SQLITE3_TEXT);
			$set = $stmt->execute();
			$result = $set->fetchArray();
			if (isset($result['id']) == true)
			{
				put_error("Username already exists\n");
				goto end;
			}
			$sql = "INSERT INTO users (name, password) VALUES (:name, :password)";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $CURR_ENV['username'], SQLITE3_TEXT);
			$stmt->bindParam(":password", $password, SQLITE3_TEXT);
			$stmt->execute();
			$user_id = $connection->lastInsertRowID();
		}
		$sql = "INSERT INTO session (user_id, session_hash) VALUES (:user_id, :session_hash)";
		$stmt = $connection->prepare($sql);
		$stmt->bindParam(":user_id", $user_id, SQLITE3_INTEGER);
		$SESSION_HASH = passHash(random_bytes(64));
		$stmt->bindParam(":session_hash", $CURR_ENV['SESSION_HASH'], SQLITE3_TEXT);
		$stmt->execute();
		set_cookie($CURR_ENV['CLIENT_FD'], "SESSION_HASH", $SESSION_HASH, 30);
		end:
		$connection = null;
	}
	set_header($CURR_ENV['CLIENT_FD'], "Location", "/index");
	exit();
?>
