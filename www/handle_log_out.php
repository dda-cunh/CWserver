<?php
	include_once("inc.php");

	$CURR_ENV = parse_args(array_slice($argv, 1));
	if (isset($CURR_ENV['REQUEST_METHOD']) && $CURR_ENV['REQUEST_METHOD'] == "POST"
		&& isset($CURR_ENV['BTN_LOGOUT']) && isset($CURR_ENV['SESSION_HASH'])
		&& $CURR_ENV['SESSION_HASH'] != 0)
		set_cookie($CURR_ENV['CLIENT_FD'], "SESSION_HASH", 0, 7);
	include_once("index.php");
?>
