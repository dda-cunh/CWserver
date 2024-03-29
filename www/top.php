<?php
	include_once("inc.php");

	$CURR_ENV = parse_args(array_slice($argv, 1));
	if (!isset($CURR_ENV['SESSION_HASH']))
		$CURR_ENV['SESSION_HASH'] = 0;

	echo '<!DOCTYPE html>
	<html lang="en">
		<head>
			<meta charset="UTF-8">
			<meta name="viewport" content="width=device-width, initial-scale=1.0">
			<meta name="description" content="Furniture store">
			<meta name="keywords" content="furniture, store, bedroom, living room, kitchen">
			<meta name="author" content="Denilson Moutinho">
			<title>Store</title>
			<link rel="icon" href="img/favicon.ico">
			<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css">
			<link rel="stylesheet" href="css/main.css">
		</head>
		<body>
			<header class="container-fluid bg-light py-3">
				<nav class="navbar navbar-expand-lg navbar-light container" aria-label="Main navigation">
					<a class="navbar-brand" href="/">
						<img src="img/favicon.ico" alt="Brand logo" width="50" height="50">
					</a>
					<button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
						<span class="navbar-toggler-icon"></span>
					</button>
					<div class="collapse navbar-collapse" id="navbarSupportedContent">
						<ul class="navbar-nav me-auto ms-lg-auto">
							<li class="nav-item">
								<a class="nav-link active" aria-current="page" href="/">Home</a>
							</li>
							<li class="nav-item dropdown">
								<a class="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-bs-toggle="dropdown" aria-expanded="false">
									Categories
								</a>
								<ul class="dropdown-menu" aria-labelledby="navbarDropdown">
									<li><a class="dropdown-item" href="' . $g_address . 'cat_bedroom' . '">Bedroom</a></li>
									<li><a class="dropdown-item" href="' . $g_address . 'cat_livingroom' . '">Living Room</a></li>
									<li><a class="dropdown-item" href="' . $g_address . 'cat_kitchen' . '">Kitchen</a></li>
								</ul>
							</li>
						</ul>
						<ul class="navbar-nav ms-auto">
							<li class="nav-item">
								<a class="nav-link" href="' . $g_address . 'cart' . '"><i class="fas fa-shopping-cart"></i> Cart</a>
							</li>
							<li class="nav-item">
								<a class="nav-link" href="' . $g_address . 'account' . '"><i class="fas fa-user"></i> Account</a>
							</li>
						</ul>
					</div>
				</nav>
			</header>';

?>

	
