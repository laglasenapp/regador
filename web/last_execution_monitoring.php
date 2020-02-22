<?php

require '/home/storage/5/bd/6f/smarthomecontrol1/vendor/autoload.php';

use Monolog\Logger;
use Monolog\Handler\StreamHandler;

$ultima_execucao = date('Y-m-d H:i:s');

$logger = new Logger('SimpleLogger');

$logger->pushHandler(new StreamHandler('/home/storage/5/bd/6f/smarthomecontrol1/public_html/regador/logs/last_execution_monitoring.log', Logger::DEBUG));

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

$device_id_param = "id";
$register_id_param = "register_id";


$device_id_valor = $_GET[$device_id_param];
$register_id_valor = $_GET[$register_id_param];


$logger->info("Device id: " . $device_id_valor);
$logger->info("Register id: " . $register_id_valor);


if (isNotBlank($device_id_valor) and isNotBlank($register_id_valor)){
	$servername = "reglas.mysql.dbaas.com.br";
	$username = "reglas";
	$password = "regador#2019!";
	$dbname = "reglas";
	
	try {
		$conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$stmt = $conn->prepare("UPDATE reglas.registros SET ultima_execucao = :ultima_execucao WHERE nome_registro = :id and id_regador = :id_regador");
		$stmt->bindParam(':ultima_execucao',$ultima_execucao);
		$stmt->bindParam(':id', $register_id_valor);
		$stmt->bindParam(':id_regador', $device_id_valor);
		$stmt->execute();
		$stmt->closeCursor();
		$conn = null;
		
    }
	catch(PDOException $e)
    {	
		echo ($e->getMessage());
		$logger->error("DB error: " . $e->getMessage());
    }

}


function isNotBlank($value)
{
    return isset($value) and !empty(value);
}

?>