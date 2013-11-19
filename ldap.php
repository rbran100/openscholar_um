<?php

$conn = ldap_connect('ldap.ucmerced.edu', 389)
        or die("Could not connect to LDAP server");
echo "Connected to LDAP<br/>";
$bd = ldap_bind($conn, 'uid=Faculty Admin,ou=Special Users,dc=ucmerced,dc=edu', '@schebAg8')
        or die("Could not bind");
echo "LDAP bind successful<br/>";
$dn = 'ou=People,dc=ucmerced,dc=edu';
$filter = '(uid=pduffy)';
$search = ldap_search($conn, $dn, $filter)
        or die("LDAP search failed");
$info = ldap_get_entries($conn, $search)
        or die("LDAP entry retrieval failed");
echo $info[0]['ou'][0] . '<br/>';
echo $info[0]['sn'][0] . ', ' . $info[0]['givenname'][0] . '<br/>';
echo '<pre>' . print_r($info, 1) . '</pre>';
?>
