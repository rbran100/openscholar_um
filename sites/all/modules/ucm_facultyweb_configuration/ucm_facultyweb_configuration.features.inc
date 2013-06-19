<?php
/**
 * @file
 * ucm_facultyweb_configuration.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ucm_facultyweb_configuration_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ucm_facultyweb_configuration_node_info() {
  $items = array(
    'site_info' => array(
      'name' => t('Site Info'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Site Information'),
      'help' => '',
    ),
  );
  return $items;
}
