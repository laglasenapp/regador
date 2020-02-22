<?php

require '/home/storage/5/bd/6f/smarthomecontrol1/vendor/autoload.php';

use Monolog\Logger;
use Monolog\Handler\StreamHandler;


$logger = new Logger('SimpleLogger');

$logger->pushHandler(new StreamHandler('/home/storage/5/bd/6f/smarthomecontrol1/public_html/regador/logs/scheduler_monitoring.log', Logger::DEBUG));

$username = null;
$password = null;


// Mщtodo para mod_php (Apache)
if ( isset( $_SERVER['PHP_AUTH_USER'] ) ):
    $username = $_SERVER['PHP_AUTH_USER'];
    $password = $_SERVER['PHP_AUTH_PW'];
    
// Mщtodo para demais servers
elseif ( isset( $_SERVER['HTTP_AUTHORIZATION'] ) ):

    if ( preg_match( '/^basic/i', $_SERVER['HTTP_AUTHORIZATION'] ) )
      list( $username, $password ) = explode( ':', base64_decode( substr( $_SERVER['HTTP_AUTHORIZATION'], 6 ) ) );
endif;

// Se a autenticaчуo nуo foi enviada
if (strcmp($username, 'reg0') != 0 || strcmp($password, 'reg0pw') != 0):
    header('WWW-Authenticate: Basic realm="Smart home control restricted access');
    header('HTTP/1.0 401 Unauthorized');
    die('Acesso negado.');
endif;


$date = new DateTime();
$current_time_str = $date->format('Y;n;j;G;i;s');

$current_time_array =  explode(";", $current_time_str);

require 'scheduler.php';

use \scheduler as A;

$scheduler = new A;
$scheduler->enabled=1;
$scheduler->s1=2000;
$scheduler->s2=9000;
$scheduler->s3=8000;
$scheduler->s4=7000;
$scheduler->s5=6000;
$scheduler->s6=5000;
$scheduler->s7=4000;
$scheduler->s8=3000;
$json = json_encode($scheduler, JSON_PRETTY_PRINT);
echo($json);

$logger->info("User: " . $username . "; JSON: " . $json);
?>