<?php
    include_once("env.php");
		echo '<form method="post" action="' . $address . 'index">
				<label for="username">Username:</label>
				<input type="text" id="username" name="username" required><br>
				<label for="password">Password:</label>
				<input type="password" id="password" name="password" required><br>
				<button type="submit" name="loginButton" value="login">Login</button>
				<button type="submit" name="registerButton" value="register">Register</button>
			</form>';

?>
