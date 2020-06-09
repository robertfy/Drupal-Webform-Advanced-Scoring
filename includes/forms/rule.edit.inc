<?php

/**
 * @file
 */

/**
 * Form callback to edit a rule.
 *
 * @param type $form
 * @param array $form_state
 * @param type $node
 * @param type $rid
 *
 * @return boolean
 */

/**
 *
 */
function webform_advanced_scoring_edit_rule($form, &$form_state, $node, $rid) {

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('webform_advanced_scoring_edit_rule_save'),
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    ),
  );

  $rule = _webform_advanced_score_get_rule($node, $rid);
  $component = NULL;

  if ($rule && $node->webform['components'][$rule->cid]) {
    $component = $node->webform['components'][$rule->cid];
  }

  $form_state['rid'] = $rule->id;

  if (!$rule || !$component) {
    drupal_set_message(t('Component not found'), 'error', FALSE);
    $form['actions']['submit']['#access'] = FALSE;
  }
  else {
    $form['info'] = array(
      '#markup' => t('Score map for %component', array('%component' => $component['name'])),
    );
    if ($component['type'] == 'select') {
     // $form['info']['#suffix'] = '<pre>' . $component['extra']['items'] . '</pre>';
    }
//print_r($rule);
//exit;
    /*$form['value_score_pair'] = array(
      '#type' => 'textarea',
      '#default_value' => $rule->value_score_pair,
      '#required' => TRUE,
      '#title' => t('Data'),
    );*/

    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#default_value' => $rule->value,
      '#required' => TRUE,
    );
    $form['score'] = array(
      '#type' => 'textfield',
      '#title' => t('Score'),
      '#default_value' => $rule->score,
      '#required' => TRUE,
    );
    $form['score_range'] = array(
      '#type' => 'textfield',
      '#title' => t('Score Range'),
      '#default_value' => $rule->score_range,
      '#required' => TRUE,
    );
    $form['cond_cid'] = array(
      '#type' => 'select',
      '#title' => 'Conditinal Component',
      '#options' => array('-') + _webform_advanced_score_allowed_components($node),
      '#default_value' => $rule->cond_cid,
    );
    $form['cond_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Conditonal Component Value'),
      '#default_value' => $rule->cond_value,
    );


  }

  return $form;
}

/**
 *
 */
function webform_advanced_scoring_edit_rule_save($form, &$form_state) {
  db_update('webform_advanced_scoring_values_scores_pair')
    ->fields(array(
      //'value_score_pair' => $form_state['values']['value_score_pair'],
      'value' => $form_state['values']['value'],
      'score' => $form_state['values']['score'],
      'score_range' => $form_state['values']['score_range'],
      'cond_cid' => $form_state['values']['cond_cid'],
      'cond_value' => $form_state['values']['cond_value'],
    ))
    ->condition('id', $form_state['rid'])
    ->execute();
}
