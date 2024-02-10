<?php
	include_once("top.php");
	if ($CURR_ENV['SESSION_HASH'] != 0)
	{
		echo '
			<main class="container mt-5 mb-5">
			<form method="post" action="' . $g_address . "handle_log_out" . '">
				<button type="submit" name="BTN_LOGOUT" value="logout">logout</button>
			</form>
		</main>';
	}
	else 
		echo '
			<main class="container mt-5 mb-5">
			<form method="post" action="' . $g_address . "handle_log" . '">
				<label for="username">Username:</label>
					<input type="text" id="username" name="username" required><br>
				<label for="password">Password:</label>
					<input type="password" id="password" name="password" required><br>
				<button type="submit" name="BTN_LOGIN" value="login">Login</button>
				<button type="submit" name="BTN_REGISTER" value="register">Register</button>
			</form>
		</main>';
	include_once("bottom.php");
?>
