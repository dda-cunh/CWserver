<?php
	include_once("top.php");

	echo '<main class="container mt-5 mb-5">
				<div class="row align-items-center">
					<div class="col-md-6 pt-4 pb-4">
						<h1>Welcome to our furniture Store</h1>
					</div>
					<div class="col-md-6">
						<img src="img/banner.png" alt="Hero banner image" class="img-fluid">
					</div>
				</div>
				<br>
				<div class="row">
					<div class="col-md-4">
						<div class="card">
							<img src="img/category_bedroom.png" alt="Bedroom image" class="card-img-top">
							<div class="card-body">
								<h5 class="card-title">Bedroom</h5>
								<a href="' . $g_address . 'cat_bedroom' . '" class="btn btn-primary">Shop Now</a>
							</div>
						</div>
					</div>
					<div class="col-md-4">
						<div class="card">
							<img src="img/category_livingroom.png" alt="Living room image" class="card-img-top">
							<div class="card-body">
								<h5 class="card-title">Living room</h5>
								<a href="' . $g_address . 'cat_livingroom' . '" class="btn btn-primary">Shop Now</a>
							</div>
						</div>
					</div>
					<div class="col-md-4">
						<div class="card">
							<img src="img/category_kitchen.png" alt="Kitchen image" class="card-img-top">
							<div class="card-body">
								<h5 class="card-title">Kitchen</h5>
								<a href="' . $g_address . 'cat_kitchen' . '" class="btn btn-primary">Shop Now</a>
							</div>
						</div>
					</div>
				</div>
			</main>';

	include_once("bottom.php");
?>
