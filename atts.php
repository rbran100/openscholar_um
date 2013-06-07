<?php


//path to phpCAS library
//Download from https://wiki.jasig.org/display/CASC/phpCAS
require_once "CAS.php";

//configure cas connection
phpCAS::client(SAML_VERSION_1_1,"castest.ucmerced.edu",443,"/cas");

//force cas auth
phpCAS::setNoCasServerValidation();
phpCAS::forceAuthentication();

//get returned attributes and dump to page
$attributes=phpCAS::getAttributes();
var_dump($attributes);

?>

