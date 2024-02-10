<?php
	include_once("top.php");

	echo "<main class='container mt-5 mb-5'>
			<div class='row'>
				<div class='col-md-12'>
					<h1>Kitchen</h1>
				</div>
			</div>
			<div class='row'>";
	$items = get_items_by_type("kitchen");
	if ($items == null)
	{
		echo "<h1>Failed to get items</h1>";
		goto dc;
	}
	foreach ($items as $item)
	{
		echo "	<div class='card' style='width: 18rem;'>
					<img src='" . $item['img_path'] . "' class='card-img-top' alt='" . $item['name'] . "'>
					<div class='card-body'>
						<h5 class='card-title text-center'>" . $item['name'] . "</h5>
						<p class='card-text'>$" . $item['price'] . "</p>
						<a onclick='addToCart(" . $item['id'] . ")' class='btn btn-primary'>Add to cart</a>
					</div>
				</div>";
	}
	dc:
	$conn = null;
	echo "	</div>
		</main>";
	include_once("bottom.php");
?>
