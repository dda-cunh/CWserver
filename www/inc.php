<?php
	include_once("env.php");

	if (!function_exists("pass_hash"))
	{
		function pass_hash($password)
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

	if (!function_exists("get_random_n_string"))
	{
		function	get_random_n_string($n)
		{
			$rand = "";
			for ($i = 0; $i < $n; $i++)
				$rand .= chr(rand(33, 126));
			return $rand;
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

	if (!function_exists("getDatabaseConnection"))
	{
		function	getDatabaseConnection()
		{
			global	$g_www_path;
			global	$g_dbName;

			return (new PDO("sqlite:" . $g_www_path . "data/" . $g_dbName . ".sqlite"));
		}
	}

	if (!function_exists("get_user_by_hash"))
	{
		function	get_user_by_hash($session_hash)
		{
			$conn = getDatabaseConnection();
			if (!$conn)
				return (null);
			$sql = 'SELECT id FROM users WHERE session_hash = :session_hash';
			$stmt = $conn->prepare($sql);
			$stmt->bindParam(":session_hash", $session_hash, PDO::PARAM_STR);
			$stmt->execute();
			$conn = null;
			return ($stmt->fetch());
		}
	}

	if (!function_exists("get_items_by_type"))
	{
		function	get_items_by_type($type)
		{
			$conn = getDatabaseConnection();
			if (!$conn)
				return (null);
			$sql = "SELECT items.id, items.name, price, img_path FROM items INNER JOIN item_type ON items.type = item_type.id WHERE item_type.name = :type";
			$stmt = $conn->prepare($sql);
			$stmt->bindParam(":type", $type, PDO::PARAM_STR);
			$stmt->execute();
			$conn = null;
			return ($stmt->fetchAll());
		}
	}

	if (!function_exists("get_user_cart_session"))
	{
		function	get_user_cart_session($session_hash)
		{
			$user = get_user_by_hash($session_hash);
			if ($user == null)
				return (null);
			$conn = getDatabaseConnection();
			if (!$conn)
				return (null);
			$sql = "SELECT items.id, items.name, price, img_path, quantity FROM items INNER JOIN cart ON items.id = cart.item_id WHERE cart.user_id = :user_id";
			$stmt = $conn->prepare($sql);
			$stmt->bindParam(":user_id", $user['id'], PDO::PARAM_INT);
			$stmt->execute();
			$conn = null;
			return ($stmt->fetchAll());
		}
	}
?>
