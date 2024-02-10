<?php
	include_once("top.php");

	echo '<main class="container mt-5 mb-5">
				<div class="row>';
	if (!isset($CURR_ENV['SESSION_HASH']) || $CURR_ENV['SESSION_HASH'] == 0)
	{
		put_error("You are not logged in\n");
		goto end;
	}
	$items = get_user_cart_session($CURR_ENV['SESSION_HASH']);
	if ($items == null)
	{
		put_error("Failed to get cart\n");
		goto end;
	}
	while (($item = $items->fetchAll()))
	{
		echo '<div class="container">
				<div class="col-md-4">
					<img src="' . $item['img_path'] . '" alt="Item image" class="img-fluid">
				</div>
				<div class="col-md-4">
					<h5>' . $item['name'] . '</h5>
				</div>
				<div class="col-md-4">
					<h5>' . $item['price'] . '</h5>
				</div>
				<div class="col-md-4">
					<h5>' . $item['quantity'] . '</h5>
			</div>';
	}
	end:
	echo '	</div>
		</main>';
	include_once("bottom.php");
?>