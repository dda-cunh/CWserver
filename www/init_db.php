<?php
	include_once("inc.php");

	$connection = getDatabaseConnection();
	if ($connection == null)
	{
		echo "Failed to connect to database\n";
		return ;
	}
	$sql = "CREATE TABLE IF NOT EXISTS users (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		name TEXT NOT NULL,
		password TEXT NOT NULL,
		session_hash TEXT NOT NULL,
		unique (name),
		unique (session_hash)
	)";
	$connection->exec($sql);
	$sql = "CREATE TABLE IF NOT EXISTS item_type (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		name TEXT NOT NULL,
		unique (name)
	)";
	$connection->exec($sql);
	$sql = "CREATE TABLE IF NOT EXISTS items (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		type integer NOT NULL,
		name TEXT NOT NULL,
		price INTEGER NOT NULL,
		img_path TEXT NOT NULL,
		unique (name, img_path),
		foreign key (type) references item_type(id)
	)";
	$connection->exec($sql);
	$sql = "CREATE TABLE IF NOT EXISTS cart (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		user_id INTEGER NOT NULL,
		item_id INTEGER NOT NULL,
		quantity INTEGER NOT NULL,
		unique (user_id, item_id),
		foreign key (user_id) references users(id),
		foreign key (item_id) references items(id)
	)";
	$connection->exec($sql);
	$connection = null;
?>