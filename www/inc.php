<?php
	if (!function_exists("tryLogin"))
	{
		function	tryLogin($_username, $_password)
		{
			global	$defaultUserName;
			global	$defaultError;
			global	$dataBase;
			global	$password;
			global	$host;
			global	$user;

			$connection = getDatabaseConnection();
			$_password = passHash($_password);
			$stmt = $connection->prepare("SELECT * FROM users WHERE name = :username AND password = :password");
			$stmt->bindParam(':username', $_username, PDO::PARAM_STR);
			$stmt->bindParam(':password', $_password, PDO::PARAM_STR);
			$stmt->execute();
			if ($stmt->rowCount() == 1)
				$_SESSION['user'] = $_username;
			else
			$_SESSION['error'] = "Invalid username or password.";
		}
	}

	if (!function_exists("tryRegister"))
	{
		function	tryRegister($_username, $_password)
		{
			global	$defaultUsername;
			global	$defaultError;
			global	$dataBase;
			global	$password;
			global	$host;
			global	$user;

			$userExists = getUser($_username);
			if ($userExists)
			{
				$_SESSION['error'] = 'Username already exists.';
				return ;
			}
			newUser($_username, $_password);
			$_SESSION['user'] = $_username;
		}
	}

	if (!function_exists("getDatabaseConnection"))
	{
		function getDatabaseConnection()
		{
			global $host;
			global $user;
			global $password;
			global $dataBase;

			$connection = new PDO("sqlite:../data/" . $dataBase);
			return($connection);
		}
	}

	if (!class_exists('ServerVars'))
	{
		class	ServerVars
		{
			public string	$_host;
			public string	$_user;
			public string	$_password;
			public string	$_dataBase;

			function __construct(string $host, string $user, string $password, string $dataBase)
			{
				$this->_host = $host;
				$this->_user = $user;
				$this->_password = $password;
				$this->_dataBase = $dataBase;
			}
		}
	}
?>
