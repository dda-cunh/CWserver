<?php
	include_once("inc.php");

	$CURR_ENV = parse_args(array_slice($argv, 1));
	if (isset($CURR_ENV['REQUEST_METHOD']) && $CURR_ENV['REQUEST_METHOD'] == "POST"
		&& isset($CURR_ENV['BTN_LOGOUT']) && isset($CURR_ENV['SESSION_HASH'])
		&& $CURR_ENV['SESSION_HASH'] != 0)
	{
		$user = get_user_by_hash($CURR_ENV['SESSION_HASH']);
		if ($user == null)
			goto end;
		$connection = getDatabaseConnection();
		if ($connection != null)
		{
			$sql = "UPDATE users SET session_hash = :session_hash WHERE id = :user_id";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":user_id", $user['id'], PDO::PARAM_INT);
			$stmt->bindValue(":session_hash", "-1", PDO::PARAM_STR);
			$stmt->execute();
			set_cookie($CURR_ENV['CLIENT_FD'], "SESSION_HASH", 0, 7);
			$connection = null;
		}
		else
			put_error("Failed to connect to database\n");
	}

	end:
	include_once("index.php");
?>
