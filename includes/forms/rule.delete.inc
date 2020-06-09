<?php

/**
 * @file
 */

/**
 * Form to delete a rule.
 *
 * @param type $form
 * @param type $form_state
 * @param type $node
 * @param type $rid
 *
 * @return boolean
 */

/**
 *
 */
function webform_advanced_scoring_delete_rule($form, &$form_state, $node, $rid) {
  $form_state['rid'] = $rid;
  $form_state['node'] = $node;

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Yes'),
      '#submit' => array('webform_advanced_scoring_delete_rule_yes'),
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    ),
  );

  $rule = _webform_advanced_score_get_rule($node, $rid);
  $component = NULL;
  if ($rule && !empty($node->webform['components'][$rule->cid])) {
    $component = $node->webform['components'][$rule->cid];
  }

  if ($rule) {
    $form['info'] = array(
      '#markup' => t('Remove the rule at component %component?', array('%component' => $component['name'])),
      '#weight' => -100,
    );
  }
  else {
    drupal_set_message(t("Component not found"), 'error', FALSE);
    $form['actions']['submit']['#access'] = FALSE;
  }

  return $form;
}

/**
 * Remove rule at confirmation.
 *
 * @param mixed $form
 * @param mixed $form_state
 */
function webform_advanced_scoring_delete_rule_yes($form, &$form_state) {
  db_delete('webform_advanced_scoring_values_scores_pair')
    ->condition('id', $form_state['rid'])
    ->condition('nid', $form_state['node']->nid)
    ->execute();
}
