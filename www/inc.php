<?php
	include_once("env.php");

	if (!function_exists("passHash"))
	{
		function	passHash($password)
		{
			return hash("whirlpool", $password);
		}
	}

	if (!function_exists("put_error"))
	{
		function	put_error($error)
		{
			echo "<div class=\"alert alert-danger\" role=\"alert\">" . $error . "</div>";
		}
	}

	if (!function_exists("parse_args"))
	{
		function	parse_args($args)
		{
			$arg = array();
			foreach ($args as $a)
			{
				$split = explode("=", $a);
				$arg[$split[0]] = $split[1];
			}
			return $arg;
		}
	}

	if (!function_exists("set_header"))
	{
		function	set_header($client_fd, $header, $value)
		{
			if ($client_fd < 1)
				return ;
			$fd = fopen("php://fd/" . $client_fd, "w");
			fwrite($fd, $header . ": " . $value . "\r\n");
			fclose($fd);
		}
	}

	if (!function_exists("set_cookie"))
	{
		function	set_cookie($client_fd, $cookie_name, $cookie_value, $days)
		{
			if ($client_fd < 1)
				return ;
			$cookie = $cookie_name . "=" . $cookie_value . "; max-age=" . (60 * 60 * 24 * $days) . "; path=/";
			set_header($client_fd, "Set-Cookie", $cookie);
		}
	}

	if (!class_exists("dbConnection"))
	{
		class dbConnection extends SQLite3
		{
			public string $path;

			function __construct($path)
			{
				$this->path = $path;
				$this->open($path, SQLITE3_OPEN_CREATE | SQLITE3_OPEN_READWRITE);
			}
		 }
	}

	if (!function_exists("getDatabaseConnection"))
	{
		function	getDatabaseConnection()
		{
			global	$g_www_path;
			global	$g_dbName;

			return (new dbConnection($g_www_path . "data/" . $g_dbName));
		}
	}

	if (!function_exists("get_user"))
	{
		function	get_user($session_hash)
		{
			$conn = getDatabaseConnection();
			$sql = "SELECT * FROM session WHERE session_hash = :session_hash";
			$stmt = $conn->prepare($sql);
			$stmt->bindParam(":session_hash", $session_hash, SQLITE3_TEXT);
			$set = $stmt->execute();
			$result = $set->fetchArray();
			if (isset($result['id']) == false)
				return null;
			$sql = "SELECT * FROM users WHERE id = :id";
			$stmt = $conn->prepare($sql);
			$stmt->bindParam(":id", $result['user_id'], SQLITE3_INTEGER);
			$set = $stmt->execute();
			$result = $set->fetchArray();
			$conn = null;
			return $result;
		}
	}
?>
