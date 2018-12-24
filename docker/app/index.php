<?php

error_reporting(E_ALL);
ob_implicit_flush();
set_time_limit(2);

include_once( "config/config.inc.php" );

include( "classes/controller.php" );

$app = new Controller( );
$app->run();
