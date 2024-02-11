<?php
	include_once("inc.php");

	$CURR_ENV = parse_args(array_slice($argv, 1));
	if (isset($CURR_ENV['REQUEST_METHOD']) && $CURR_ENV['REQUEST_METHOD'] == "POST"
		&& (isset($CURR_ENV['BTN_LOGIN']) || isset($CURR_ENV['BTN_REGISTER'])))
	{
		$connection = getDatabaseConnection();
		if ($connection == null)
		{
			put_error("Failed to connect to database\n");
			goto end;
		}
		$SESSION_HASH = pass_hash(get_random_n_string(10));
		$PASSWORD = pass_hash($CURR_ENV['password']);
		$USERNAME = $CURR_ENV['username'];
		$sql = "SELECT * FROM users WHERE name = :name";
		$stmt = $connection->prepare($sql);
		$stmt->bindParam(":name", $USERNAME, PDO::PARAM_STR);
		$stmt->execute();
		$user_by_name = $stmt->fetch();
		if (isset($CURR_ENV['BTN_LOGIN']))
		{
			if (isset($user_by_name['id']) == false || $user_by_name['password'] != $PASSWORD)
			{
				put_error("Failed to login\n");
				goto end;
			}
			else
				$SESSION_HASH = $user_by_name['session_hash'];
		}
		else
		{
			if (isset($user_by_name['id']))
			{
				put_error("Username already exists\n");
				goto end;
			}
			$sql = "INSERT INTO users (name, password, session_hash) VALUES (:name, :password, :session_hash)";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $USERNAME, PDO::PARAM_STR);
			$stmt->bindParam(":password", $PASSWORD, PDO::PARAM_STR);
			$stmt->bindParam(":session_hash", $SESSION_HASH, PDO::PARAM_STR);
			$stmt->execute();
		}
		set_cookie($CURR_ENV['CLIENT_FD'], "SESSION_HASH", $SESSION_HASH, 30);
		end:
		$connection = null;
	}
	
	include_once("index.php");
?>
