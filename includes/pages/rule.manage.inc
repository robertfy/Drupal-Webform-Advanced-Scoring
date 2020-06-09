<?php

/**
 * @file
 */

/**
 * Page callback - choose which form to render.
 *
 * @todo create the delete rule callback
 * @todo create the edit rule callback
 *
 * @param type $node
 * @param type $action
 * @param type $sid
 *
 * @return type
 */

/**
 *
 */
function webform_advanced_scoring_scoring_rules($node, $action = NULL, $rid = NULL) {
  switch ($action) {
    case 'delete':
      module_load_include('inc', 'webform_advanced_scoring', 'includes/forms/rule.delete');
      return drupal_get_form('webform_advanced_scoring_delete_rule', $node, $rid);

    case 'edit':
      module_load_include('inc', 'webform_advanced_scoring', 'includes/forms/rule.edit');
      return drupal_get_form('webform_advanced_scoring_edit_rule', $node, $rid);

    default:
      //module_load_include('inc', 'webform_advanced_scoring', 'includes/forms/rule.view');
      return drupal_get_form('webform_advanced_scoring_manage_rules', $node);
  }
}

/**
 * Get a list of all used components.
 *
 * @param mixed $node
 *
 * @return mixed
 */
function _webform_advanced_score_used_components($node) {
  $components = &drupal_static(__FUNCTION__ . '_' . $node->nid);

  if (!$components) {
    $components = array();
    $result = db_select('webform_advanced_scoring_values_scores_pair', 'a')
      ->fields('a')
      ->condition('a.nid', $node->nid)
      ->execute();
    foreach ($result as $component) {
      $components[] = $component;
    }
  }
  return $components;
}

/**
 * Load a single rule.
 *
 * @param mixed $rid
 *
 * @return mixed
 */
function _webform_advanced_score_get_rule($node, $rid) {
  $rule = &drupal_static(__FUNCTION__ . '_' . $rid);
  if (!$rule) {
    $query = db_select('webform_advanced_scoring_values_scores_pair', 'p');
    $query->fields('p');
    $query->condition('id', $rid);
    $query->condition('nid', $node->nid);
    $rule = $query->execute()->fetch();
  }

  return $rule;
}

/**
 * Get a list of all possible components to create a rule for.
 *
 * @param mixed $node
 *
 * @return mixed
 */
function _webform_advanced_score_allowed_components($node) {
  $components = &drupal_static(__FUNCTION__ . '_' . $node->nid);

  if (!$components) {
    $used = []; //_webform_advanced_score_used_components($node);
    foreach ($node->webform['components'] as $cid => $component) {
      if (!empty($used[$cid])) {
        continue;
      }

      switch ($component['type']) {
        case 'markup':
        case 'hidden':
        case 'pagebreak':
          continue 2;

        default:
          break;
      }
      $components[$cid] = $component['name'];
    }
  }

  return $components;
}





