<?php
/**
 * @file
 * ucm_openscholar_configuration.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function ucm_openscholar_configuration_user_default_roles() {
  $roles = array();

  // Exported role: faculty.
  $roles['faculty'] = array(
    'name' => 'faculty',
    'weight' => '0',
  );

  return $roles;
}
