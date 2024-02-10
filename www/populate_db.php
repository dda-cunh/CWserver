<?php
	include_once("init_db.php");

    $connection = getDatabaseConnection();
    if ($connection == null)
    {
        echo "Failed to connect to database\n";
        return ;
    }
    $connection->exec("INSERT INTO item_type (name) VALUES ('bedroom')");
	$connection->exec("INSERT INTO item_type (name) VALUES ('livingroom')");
	$connection->exec("INSERT INTO item_type (name) VALUES ('kitchen')");
    $connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (1, 'Single-bed', 100, 'img/sbed.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (1, 'Double-bed', 200, 'img/dbed.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (1, 'King-bed', 300, 'img/kbed.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (2, 'Sofa', 400, 'img/sofa.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (2, 'Armchair', 500, 'img/armchair.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (2, 'Coffee table', 600, 'img/ctable.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (3, 'Dining table', 700, 'img/dtable.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (3, 'Dining chair', 800, 'img/dchair.jpg')");
	$connection->exec("INSERT INTO items (type, name, price, img_path) VALUES (3, 'Cupboard', 900, 'img/cupboard.jpg')");
    $connection = null;
?>