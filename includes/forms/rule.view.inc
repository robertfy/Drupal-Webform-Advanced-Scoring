<?php

/**
 * @file
 */

/**
 * Form callback for the main form.
 *
 * @param type $form
 * @param type $form_state
 * @param type $node
 *
 * @return string
 */

/**
 *
 */



function webform_advanced_scoring_dependent_dropdown($form, &$form_state) {
  
 $options_first = _ajax_example_get_first_dropdown_options2();

  // If we have a value for the first dropdown from $form_state['values'] we use
  // this both as the default value for the first dropdown and also as a
  // parameter to pass to the function that retrieves the options for the
  // second dropdown.
  $selected2 = isset($form_state['values']['dropdown_first']) ? $form_state['values']['dropdown_first'] : key($options_first);
  $form['dropdown_first'] = array(
    '#type' => 'select',
    '#title' => 'Instrument Type',
    '#options' => $options_first,
    '#default_value' => $selected2,
    // Bind an ajax callback to the change event (which is the default for the
    // select form type) of the first dropdown. It will replace the second
    // dropdown when rebuilt.
    '#ajax' => array(
      // When 'event' occurs, Drupal will perform an ajax request in the
      // background. Usually the default value is sufficient (eg. change for
      // select elements), but valid values include any jQuery event,
      // most notably 'mousedown', 'blur', and 'submit'.
      // 'event' => 'change',
      'callback' => 'webform_advanced_scoring_dependent_dropdown_callback',
      'wrapper' => 'dropdown-second-replace',
    ),
  );
  $form['dropdown_second'] = array(
    '#type' => 'select',
    '#title' => $options_first[$selected2] . ' ' . t('Instruments'),
    // The entire enclosing div created here gets replaced when dropdown_first
    // is changed.
    '#prefix' => '<div id="dropdown-second-replace">',
    '#suffix' => '</div>',
    // When the form is rebuilt during ajax processing, the $selected variable
    // will now have the new value and so the options will change.
    '#options' => _ajax_example_get_second_dropdown_options2($selected2),
    '#default_value' => isset($form_state['values']['dropdown_second']) ? $form_state['values']['dropdown_second'] : '',
  );
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $label_value,
  );

  $form['actions'] = array(
    '#weight' => 10,
    '#type' => 'actions',
  );

  if ($score_enabled) {
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
    $form['actions']['disable'] = array(
      '#type' => 'submit',
      '#value' => t('Disable'),
      '#submit' => array('webform_advanced_scoring_manage_rules_submit_disable'),
    );
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Enable'),
  );
  return $form;
}



function _ajax_example_get_first_dropdown_options2() {
  // drupal_map_assoc() just makes an array('String' => 'String'...).
  return drupal_map_assoc(array(
    t('String'),
    t('Woodwind'),
    t('Brass'),
    t('Percussion'),
  ));
}


function webform_advanced_scoring_dependent_dropdown_callback($form, $form_state) {
  print "abdfsadsad";exit;
  return $form['dropdown_second'];
}

function _ajax_example_get_second_dropdown_options2($key = '') {
  $options = array(
    t('String') => drupal_map_assoc(array(
      t('Violin'),
      t('Viola'),
      t('Cello'),
      t('Double Bass'),
    )),
    t('Woodwind') => drupal_map_assoc(array(
      t('Flute'),
      t('Clarinet'),
      t('Oboe'),
      t('Bassoon'),
    )),
    t('Brass') => drupal_map_assoc(array(
      t('Trumpet'),
      t('Trombone'),
      t('French Horn'),
      t('Euphonium'),
    )),
    t('Percussion') => drupal_map_assoc(array(
      t('Bass Drum'),
      t('Timpani'),
      t('Snare Drum'),
      t('Tambourine'),
    )),
  );
  if (isset($options[$key])) {
    return $options[$key];
  }
  else {
    return array();
  }
}




function webform_advanced_scoring_manage_rules($form, &$form_state, $node) {

  $score_record = _webform_advanced_score_enabled($node->nid);
  $form_state['score_record'] = $score_record;
  $form_state['node'] = $node;

  $label_value = "Webform Quiz - Score";

  if ($score_record) {
    $has_score = _webform_advanced_score_has_cid($node, $score_record->cid);
    if (!$has_score) {
      $score_enabled = FALSE;
    }
    else {
      $score_enabled = $score_record->enabled;
      $label_value = $has_score;
    }
  }
  else {
    $score_enabled = FALSE;
  }

  $components = _webform_advanced_score_allowed_components($node);
  $components2 = array('-') +_webform_advanced_score_allowed_components($node);

  $selected = isset($form_state['values']['cid']) ? $form_state['values']['cid'] : key($components);
  $selected2 = isset($form_state['values']['cond_cid']) ? $form_state['values']['cond_cid'] : key($components2);

  if ($components) {
    $form['new'] = array(
      '#type' => 'fieldset',
      '#title' => t("New rule"),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'cid' => array(
        '#type' => 'select',
        '#title' => 'Component',
        '#options' => $components,
        '#attributes' => array(
          'class' => array(
            'enabled-for-ajax',
          ),
        ),
        '#ajax' => array(
          'callback' => 'webform_advanced_scoring_manage_rules_callback',
          'wrapper' => 'view-display-dropdown',
        ),
      ),
      'value' => array(
        '#type' => 'select',
        '#title' => t('Value'),
        '#default_value' => '',
        '#prefix' => '<div id="view-display-dropdown">',
        '#suffix' => '</div>',
        '#attributes' => array(
          'class' => array(
            'enabled-for-ajax',
          ),
        ),
        '#options' => webform_advanced_scoreing_ajax_callback_query_builder($node, $selected),//$components[2]['extra']['items'],
      ),
      'score' => array(
        '#type' => 'textfield',
        '#title' => t('Score'),
        '#default_value' => '',
      ),
      'score_range' => array(
        '#type' => 'textfield',
        '#title' => t('Score Range'),
        '#default_value' => '0',
      ),
      'cond_cid' => array(
        '#type' => 'select',
        '#title' => 'Conditinal Component',
        '#options' => $components2,
        '#ajax' => array(
          'callback' => 'webform_advanced_scoring_manage_rules2_callback',
          'wrapper' => 'view-display2-dropdown',
        ),
      ),
      'cond_value' => array(
        '#type' => 'select',
        '#title' => t('Conditonal Component Value'),
        '#default_value' => '',
        '#prefix' => '<div id="view-display2-dropdown">',
        '#suffix' => '</div>',
        '#attributes' => array(
          'class' => array(
            'enabled-for-ajax',
          ),
        ),
        '#options' => webform_advanced_scoreing_ajax_callback2_query_builder($node, $selected2),//$components[2]['extra']['items'],
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#submit' => array('webform_advanced_scoring_manage_rules_submit_add'),
      ),
    );
  }
 

  $components = _webform_advanced_score_used_components($node);
//print_r($components);
//exit;
  $component_table_body = array();
  $count = 0;
  $options = array('query' => drupal_get_destination());
  foreach ($components as $item) {

    $p_item['name'] = $node->webform['components'][$item->cid]['name'];
    $p_item['type'] = $node->webform['components'][$item->cid]['type'];
    $p_item['value'] = check_plain($item->value);
    $p_item['score'] = check_plain($item->score);
    $p_item['score_range'] = check_plain($item->score_range);
    if($item->cond_cid > 0) {
      $p_item['cond_cid'] = $node->webform['components'][$item->cond_cid]['name'];
      $p_item['cond_value'] = check_plain($item->cond_value);
    }
    else {
      $p_item['cond_cid'] = "";
      $p_item['cond_value'] = "";
    }
    $p_item['actions'] = implode('<br>', array(
      //l(t('edit'), "node/{$node->nid}/webform/advanced-scoring-rules/edit/{$item->id}", $options),
      l(t('delete'), "node/{$node->nid}/webform/advanced-scoring-rules/delete/{$item->id}", $options),
    ));

    $component_table_body[] = $p_item;
  }

  $headers = array("Component", "Type", "Value", "Score", "Score Range", "Conditional Component", "Conditional Component value", "Actions");
  $form['items'] = array(
    '#weight' => 100,
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $component_table_body,
    '#attributes' => array('class' => array('mytable')),
  );

  return $form;
}






/**
 * On enable/disable.
 *
 * @param mixed $form
 * @param array $form_state
 */
function webform_advanced_scoring_manage_rules_submit($form, &$form_state) {
  $score_record = $form_state['score_record'];
  $node = $form_state['node'];

  if (!$score_record) {
    // Create if needed.
    $cid = create_a_new_hidden_component($node, $form_state['values']['label']);
    $result = db_insert('webform_advanced_scoring_enabled_webforms')
      ->fields(array('enabled' => 1, 'cid' => $cid, 'nid' => $node->nid))
      ->execute();
  }
  else {
    // Check if the cid is still in place.
    $score_record->cid = create_a_new_hidden_component($node, $form_state['values']['label'], $score_record->cid);

    // Update record.
    $result = db_update('webform_advanced_scoring_enabled_webforms')
      ->fields(array(
        'enabled' => 1,
        'cid' => $score_record->cid,
      ))
      ->condition('id', $score_record->id)
      ->execute();
  }
}

/**
 * On disable scoring for the selected node.
 *
 * @param mixed $form
 * @param mixed $form_state
 */
function webform_advanced_scoring_manage_rules_submit_disable($form, &$form_state) {
  $score_record = $form_state['score_record'];
  $node = $form_state['node'];

  // Update record.
  $result = db_update('webform_advanced_scoring_enabled_webforms')
    ->fields(array(
      'enabled' => 0,
      'cid' => 0,
    ))
    ->condition('id', $score_record->id)
    ->execute();

  // Delete the component.
  delete_custom_hidden_component($node->nid, $score_record->cid);
}

/**
 * On submit a new rule.
 *
 * @param mixed $form
 * @param mixed $form_state
 */
function webform_advanced_scoring_manage_rules_submit_add($form, &$form_state) {
  // Attache to this one:
  $node = $form_state['node'];

  $result = db_insert('webform_advanced_scoring_values_scores_pair')
    ->fields(array(
      'nid' => $node->nid,
      'cid' => $form_state['values']['cid'],
      'value' => $form_state['values']['value'],
      'score' => $form_state['values']['score'],
      'score_range' => $form_state['values']['score_range'],
      'cond_cid' => $form_state['values']['cond_cid'],
      'cond_value' => $form_state['values']['cond_value'],
    ))
    ->execute();
}

/**
 * Delete a specific component at the node.
 *
 * @param mixed $nid
 * @param mixed $cid
 */
function delete_custom_hidden_component($nid, $cid = 9999) {
  $node = node_load($nid);
  // dsm($node);
  unset($node->webform['components'][$cid]);
  node_save($node);
}

/**
 * Create a hidden custom component to store the result.
 *
 * @param mixed $nid
 * @param mixed $new_component
 * @param mixed $cid
 * @param mixed $pid
 *
 * @return mixed
 */
function create_a_new_hidden_component($node, $label, $cid = NULL) {

  $old_label = _webform_advanced_score_has_cid($node, $cid);
  if ($old_label) {
    // Update.
    $node->webform['components'][$cid]['name'] = $label;
  }
  else {
    // Insert.
    $cid = max(array_keys($node->webform['components'])) + 1;
    $safe_label = drupal_html_class($label);
    $processed_name = str_replace('-', '_', strtolower($safe_label));
    // Create the webform components array. Not sure if we need all these
    // values, but let's be sure.
    $component = array(
      'cid' => $cid,
    // (int)$pid,.
      'pid' => 0,
      'nid' => (int) $node->nid,
      // I don't trust the admin to make a key based on input :)
      'form_key' => $processed_name,
      'name' => $label,
      // I want all lines to be numeric type component.
      'type' => 'hidden',
      'value' => '0',
      'extra' => array(),
      'mandatory' => '0',
      'weight' => -1,
      'page_num' => 1,
    );
    $node->webform['components'][$cid] = $component;
  }

  node_save($node);
  return $cid;
}

function _webform_advanced_score_allowed_values_per_component($node, $cid) {
  $componentss = $node->webform['components'];
  $options= array();

  $lines = explode("\n", trim($componentss[$cid]['extra']['items']));
  //$lines = explode("\n", trim($componentss[6]['extra']['items']));
  foreach ($lines as $com_val) {
    $item = explode("|", $com_val);
    $options[$item[0]] = $item[1];
  }
  return $options;
}

function webform_advanced_scoring_manage_rules_callback($form, $form_state) {
  return $form['new']['value'];
}

function webform_advanced_scoreing_ajax_callback_query_builder($node, $cid) {
  $options = _webform_advanced_score_allowed_values_per_component($node, $cid);
  
  return $options;
}

function webform_advanced_scoring_manage_rules2_callback($form, $form_state) {
  return $form['new']['cond_value'];
}

function webform_advanced_scoreing_ajax_callback2_query_builder($node, $cid) {
  $options = _webform_advanced_score_allowed_values_per_component($node, $cid);
  
  return $options;
}