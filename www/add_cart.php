<?php
	include_once("inc.php");

	$CURR_ENV = parse_args(array_slice($argv, 1));
	if (!isset($CURR_ENV['SESSION_HASH']) || $CURR_ENV['SESSION_HASH'] == 0)
		exit();
	if (isset($CURR_ENV['REQUEST_METHOD'])
		&& $CURR_ENV['REQUEST_METHOD'] == 'POST' && isset($CURR_ENV['item_id']))
	{
		$user = get_user_by_hash($CURR_ENV['SESSION_HASH']);
		if (!$user)
			exit();
		$connection = getDatabaseConnection();
		if (!$connection)
			exit();
		$sql = "SELECT * FROM cart WHERE user_id = :user_id AND item_id = :item_id";
		$stmt = $connection->prepare($sql);
		$stmt->bindParam(":user_id", $user['id'], PDO::PARAM_INT);
		$stmt->bindParam(":item_id", $CURR_ENV['item_id'], PDO::PARAM_INT);
		$stmt->execute();
		$result = $stmt->fetchAll();
		if ($result)
		{
			$sql = "UPDATE cart SET quantity = quantity + 1 WHERE user_id = :user_id AND item_id = :item_id";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":user_id", $user['id'], PDO::PARAM_INT);
			$stmt->bindParam(":item_id", $CURR_ENV['item_id'], PDO::PARAM_INT);
			$stmt->execute();
		}
		else
		{
			$sql = "INSERT INTO cart (user_id, item_id, quantity) VALUES (:user_id, :item_id, 1)";
			$stmt = $connection->prepare($sql);
			$stmt->bindParam(":user_id", $user['id'], PDO::PARAM_INT);
			$stmt->bindParam(":item_id", $CURR_ENV['item_id'], PDO::PARAM_INT);
			$stmt->execute();
		}
		$connection = null;
	}
?>
