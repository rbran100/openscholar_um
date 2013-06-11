<?php
/**
 * @file
 * ucm_openscholar_configuration.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ucm_openscholar_configuration_user_default_permissions() {
  $permissions = array();

  // Exported permission: create openscholar site.
  $permissions['create openscholar site'] = array(
    'name' => 'create openscholar site',
    'roles' => array(
      0 => 'administrator',
      1 => 'faculty',
    ),
    'module' => 'vsite_register',
  );

  return $permissions;
}
