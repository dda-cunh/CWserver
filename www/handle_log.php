<?php
	include_once("inc.php");

	$CURR_ENV = parse_args(array_slice($argv, 1));
	if (isset($CURR_ENV['REQUEST_METHOD']) && $CURR_ENV['REQUEST_METHOD'] == "POST"
		&&( isset($CURR_ENV['BTN_LOGIN']) || isset($CURR_ENV['BTN_REGISTER'])))
	{
		$connection = getDatabaseConnection();
		if ($connection == null)
		{
			put_error("Failed to connect to database\n");
			goto end;
		}
		$SESSION_HASH = pass_hash(get_random_n_string(10));
		$password = pass_hash($CURR_ENV['password']);
		if (isset($CURR_ENV['BTN_LOGIN']))
		{
			$sql = "SELECT * FROM users WHERE name = :name AND password = :password";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $CURR_ENV['username'], PDO::PARAM_STR);
			$stmt->bindParam(":password", $password, PDO::PARAM_STR);
			$result = $stmt->fetchAll();
			if (isset($result['id']) == false)
			{
				put_error("Failed to login\n");
				goto end;
			}
			if (isset($result['session_hash']))
			{
				if ($result['session_hash'] == "-1")
				{
					$sql = "UPDATE users SET session_hash = :session_hash WHERE id = :id";
					$stmt = $connection->prepare($sql);
					$stmt->bindParam(":session_hash", $SESSION_HASH, PDO::PARAM_STR);
					$stmt->bindParam(":id", $result['id'], PDO::PARAM_INT);
					$stmt->execute();
				}
				else
					$SESSION_HASH = $result['session_hash'];
			}
		}
		else
		{
			$sql = "SELECT * FROM users WHERE name = :name";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $CURR_ENV['username'], PDO::PARAM_STR);
			$stmt->execute();
			$result = $stmt->fetchAll();;
			if (isset($result['id']) == true)
			{
				put_error("Username already exists\n");
				goto end;
			}
			$sql = "INSERT INTO users (name, password, session_hash) VALUES (:name, :password, :session_hash)";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":name", $CURR_ENV['username'], PDO::PARAM_STR);
			$stmt->bindParam(":password", $password, PDO::PARAM_STR);
			$stmt->bindParam(":session_hash", $SESSION_HASH, PDO::PARAM_STR);
			$stmt->execute();
		}
		set_cookie($CURR_ENV['CLIENT_FD'], "SESSION_HASH", $SESSION_HASH, 30);
		end:
		$connection = null;
	}
	
	include_once("index.php");
?>
