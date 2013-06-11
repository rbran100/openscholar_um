<?php
/**
 * @file
 * ucm_openscholar_configuration.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ucm_openscholar_configuration_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_domain';
  $strongarm->value = 'ucmerced.edu';
  $export['cas_domain'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_port';
  $strongarm->value = '443';
  $export['cas_port'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_server';
  $strongarm->value = 'cas.ucmerced.edu';
  $export['cas_server'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_uri';
  $strongarm->value = '/cas';
  $export['cas_uri'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cas_version';
  $strongarm->value = '2.0';
  $export['cas_version'] = $strongarm;

  return $export;
}
