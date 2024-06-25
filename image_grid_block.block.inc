<?php 

/**
 * BlockIIV extends Block.
 *
 * This class allows us to create blocks with image and text.
 */
class ImageGridBlock extends Block {

  /**
   * Constructor for an IIV Block.
   */
  public function __construct($plugin_name, array $data) {
    parent::__construct($plugin_name, $data);

    $this->settings += array(
      'label' => '',
      'machine_name' => '',
    );
  }

    /**
   * Sets title text on draggable block panel in Layout builder.
   */
  public function getAdminTitle() {
    return !empty($this->settings['label']) ? check_plain($this->settings['label']) : t('Image Grid Block');
  }

	/**
	* Returns the rendered content of this block.
	*
	* @return string
	*   Rendered markup of an IIV Block content.
	*/
	public function getContent() {
    backdrop_set_message("starting Get Content Function");
		$elements = array(
		  'content_1' => '',
      'content_2' => '',
      'content_3' => '',
      'image_1' => '',
      'image_2' => '',
		  'image_3' => '',
		  'wrapper_classes' => 'image-grid-block-inner ' . $this->settings['machine_name'],
		  'machine_name' => $this->settings['machine_name'],
		);
    $block = config_get('image_grid_block.block.' . $this->settings['machine_name']);

    if (!empty($block['content_1'])) {
      $content_1 = token_replace($block['content_1']);
      $elements['content_1'] = check_markup($content_1, $block['format'], '', TRUE);
    }
    if (!empty($block['content_2'])) {
      $content_2 = token_replace($block['content_2']);
      $elements['content_2'] = check_markup($content_2, $block['format'], '', TRUE);
    }
    if (!empty($block['content_3'])) {
      $content_3 = token_replace($block['content_3']);
      $elements['content_3'] = check_markup($content_3, $block['format'], '', TRUE);
    }

    if (!empty($block['image_1'])) {
      $markup = '';
      $image_style = FALSE;
      if (!empty($block['image_style'])) {
        $image_style = image_style_load($block['image_style']);
      }
      $file = file_load($block['image_1']);
      if (is_a($file, 'File')) {
        $elements['img_url'] = file_create_url($file->uri);
        $variables = array(
          'style_name' => 'thumbnail',
          'uri' => $file->uri,
          'width' => isset($file->width) ? $file->width : NULL,
          'height' => isset($file->height) ? $file->height : NULL,
          'attributes' => array(),
        );
      }
      $markup = '<img src="' . $elements['img_url'] . '">';
      $elements['image_1'] = $markup;
    }



    if (!empty($block['image_2'])) {
      $markup = '';
      $image_style = FALSE;
      if (!empty($block['image_style'])) {
        $image_style = image_style_load($block['image_style']);
      }
      $file = file_load($block['image_2']);
      if (is_a($file, 'File')) {
        $elements['img_url'] = file_create_url($file->uri);
        $variables = array(
          'style_name' => 'thumbnail',
          'uri' => $file->uri,
          'width' => isset($file->width) ? $file->width : NULL,
          'height' => isset($file->height) ? $file->height : NULL,
          'attributes' => array(),
        );
      }
      $markup = '<img src="' . $elements['img_url'] . '">';
      $elements['image_2'] = $markup;
    }

    if (!empty($block['image_3'])) {
      $markup = '';
      $image_style = FALSE;
      if (!empty($block['image_style'])) {
        $image_style = image_style_load($block['image_style']);
      }
      $file = file_load($block['image_3']);
      if (is_a($file, 'File')) {
        $elements['img_url'] = file_create_url($file->uri);
        $variables = array(
          'style_name' => 'thumbnail',
          'uri' => $file->uri,
          'width' => isset($file->width) ? $file->width : NULL,
          'height' => isset($file->height) ? $file->height : NULL,
          'attributes' => array(),
        );
      }
      $markup = '<img src="' . $elements['img_url'] . '">';
      $elements['image_3'] = $markup;
    }

    return theme('image_grid_block_content', $elements);
  }

 	/**
   * Builds the block's configuration form.
   */
  public function form(&$form, &$form_state) {
    parent::form($form, $form_state);
    if ($this->is_new) {
      $args = arg();
      if (isset($args[8])) {
        $block_info = explode(':', $args[8]);
        if (isset($block_info[1]) && $block_info[1] != 'default') {
          // Copy original data (image/text).
          $block = config_get('image_grid_block.block.' . $block_info[1]);
          // Force new machine name.
          $this->settings['machine_name'] = '';
          $this->settings['title'] = $block['title'];
        }
      }
    }
    else {
      // Edit existing block, no clone.
      $block = config_get('image_grid_block.block.' . $this->settings['machine_name']);
    }
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Admin label'),
      '#default_value' => $this->settings['label'],
      '#maxlength' => 64,
      '#description' => t('This text is used only in administrative interfaces. It will not be shown to site visitors.'),
      '#required' => TRUE,
      '#id' => 'image_grid_block-label',
      '#weight' => -1,
    );
    $form['machine_name'] = array(
      '#type' => 'machine_name',
      '#default_value' => $this->settings['machine_name'],
      '#maxlength' => 128,
      '#machine_name' => array(
        'exists' => 'image_grid_block_load',
        'source' => array('label'),
      ),
      '#description' => t('A unique machine-readable name for this Block. It must only contain lowercase letters, numbers, and underscores.'),
      '#disabled' => !empty($this->settings['machine_name']),
    );

    $form['title_display']['title_display']['#default_value'] = LAYOUT_TITLE_CUSTOM;
    $form['title_display']['title_display']['#access'] = FALSE;
    $form['title_display']['title']['#default_value'] = $this->settings['title'];
    $form['title_display']['title']['#description'] = t('Leave empty for no block title.');

    // Image.
    $extensions = 'jpg jpeg png gif svg';
    if (config_get('system.core', 'image_toolkit') == 'gd' && defined('IMAGETYPE_WEBP')) {
      $gd_info = gd_info();
      if (isset($gd_info['WebP Support']) && $gd_info['WebP Support'] == TRUE) {
        $extensions .= ' webp';
      }
    }
    $upload_validators = array('file_validate_extensions' => array($extensions));
    $config = config_get('image_grid_block.settings');
    if (!empty($config['image_grid_block_max_file_size'])) {
      $max_bytes = parse_size($config['image_grid_block_max_file_size']);
      $upload_validators['file_validate_size'] = array($max_bytes);
    }
    if (!empty($config['image_grid_block_max_dimensions'])) {
      $upload_validators['file_validate_image_resolution'] = array($config['image_grid_block_max_dimensions'], '');
    }
    $upload_description = theme('file_upload_help', array(
      'upload_validators' => $upload_validators,
    ));
    $upload_location = 'public://';
    if (!empty($config['image_grid_block_image_path'])) {
      $upload_location .= $config['image_grid_block_image_path'];
    }

    // $form['one'] = array(
    //   '#type' => 'fieldset',
    //   '#title' => t('Image 1'),
    //   '#collapsible' => TRUE,
    //   '#collapsed' => TRUE,
    // );

    $form['image_1'] = array( 
      '#type' => 'managed_file',
      '#title' => t('Image 1'),
      '#description' => $upload_description,
      '#default_value' => isset($block['image_1']) ? $block['image_1'] : '',
      '#upload_location' => $upload_location,
      '#upload_validators' => $upload_validators,
      '#browser_view' => 'image_library',
    );

    $form['content_1'] = array(
      '#type' => 'text_format',
      '#title' => t('Block content'),
      '#default_value' => isset($block['content_1']) ? $block['content_1'] : '',
      '#format' => isset($block['format']) ? $block['format'] : NULL,
      '#editor_uploads' => TRUE,
      '#rows' => 5,
    );

    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Admin label'),
      '#default_value' => $this->settings['label'],
      '#maxlength' => 64,
      '#description' => t('This text is used only in administrative interfaces. It will not be shown to site visitors.'),
      '#required' => TRUE,
      '#id' => 'image_grid_block-label',
      '#weight' => -1,
    );


    // $form['two'] = array(
    //   '#type' => 'fieldset',
    //   '#title' => t('Image 2'),
    //   '#collapsible' => TRUE,
    //   '#collapsed' => TRUE,
    // );

    $form['image_2'] = array( 
      '#type' => 'managed_file',
      '#title' => t('Image 2'),
      '#description' => $upload_description,
      '#default_value' => isset($block['image_2']) ? $block['image_2'] : '',
      '#upload_location' => $upload_location,
      '#upload_validators' => $upload_validators,
      '#browser_view' => 'image_library',
    );

    $form['content_2'] = array( 
      '#type' => 'text_format',
      '#title' => t('Block content'),
      '#default_value' => isset($block['content_2']) ? $block['content_2'] : '',
      '#format' => isset($block['format']) ? $block['format'] : NULL,
      '#editor_uploads' => TRUE,
      '#rows' => 5,
    );

    // $form['three'] = array(
    //   '#type' => 'fieldset',
    //   '#title' => t('Image 3'),
    //   '#collapsible' => TRUE,
    //   '#collapsed' => TRUE,
    // );

    $form['image_3'] = array( 
      '#type' => 'managed_file',
      '#title' => t('Image 3'),
      '#description' => $upload_description,
      '#default_value' => isset($block['image_3']) ? $block['image_3'] : '',
      '#upload_location' => $upload_location,
      '#upload_validators' => $upload_validators,
      '#browser_view' => 'image_library',
    );

    $form['content_3'] = array( 
      '#type' => 'text_format',
      '#title' => t('Block content'),
      '#default_value' => isset($block['content_3']) ? $block['content_3'] : '',
      '#format' => isset($block['format']) ? $block['format'] : NULL,
      '#editor_uploads' => TRUE,
      '#rows' => 5,
    );

    // Additional image settings and attributes.
    $form['extended'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional image settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $options = array();
    foreach (image_styles() as $key => $preset) {
      $options[$key] = $preset['name'];
    }
    if (!empty($options)) {
      $form['extended']['image_style'] = array(
        '#type' => 'select',
        '#title' => t('Image style'),
        '#options' => array('' => t('<none>')) + $options,
        '#default_value' => isset($block['image_style']) ? $block['image_style'] : '',
      );
    }

  }

  /**
   * Validates the form settings.
   */
  public function formValidate($form, &$form_state) {
    parent::formValidate($form, $form_state);

    // Check that one of title, text, or image, is provided.
    if (empty($form_state['values']['image_1']) && empty($form_state['values']['image_2']) && empty($form_state['values']['image_3']) && empty($form_state['values']['title']) && empty($form_state['values']['content_1']['value']) && empty($form_state['values']['content_2']['value']) && empty($form_state['values']['content_3']['value'])) {
      form_error($form['image'], t('Three images, a Display title, and Block content must be provided.'));
    }
  }
 
  /**
   * Submit handler to save the form settings.
   */
  public function formSubmit($form, &$form_state) {
    parent::formSubmit($form, $form_state); 
    $this->settings['label'] = $form_state['values']['label'];
    $this->settings['machine_name'] = $form_state['values']['machine_name'];

    // Additional content goes to a different config.
    $block = array();
    $block['parent'] = $this->delta;
    $block['label'] = $form_state['values']['label'];
    $block['machine_name'] = $form_state['values']['machine_name'];
    $block['title'] = $form_state['values']['title'];
    $block['content_1'] = $form_state['values']['content_1']['value'];
    $block['content_2'] = $form_state['values']['content_2']['value'];
    $block['content_3'] = $form_state['values']['content_3']['value'];
    $block['format'] = $form_state['values']['content_1']['format'];

    // Image 1

    if (!empty($form_state['values']['image_1'])) {
      $current_file = file_load($form_state['values']['image_1']);
      if (!$this->is_new) {
        // Edit an existing block.
        if ($form_state['values']['image_1'] != $form['image_1']['#default_value']) {
          // Set file usage on new file, also marks permanent.
          file_usage_add($current_file, 'image_grid_block', 'block', $current_file->fid);
          if (!empty($form['image_1']['#default_value'])) {
            // Remove usage from old (replaced) file.
            $old_file = file_load($form['image_1']['#default_value']);
            if ($old_file) {
              file_usage_delete($old_file, 'image_grid_block');
            }
          }
        }
      }
      else {
        // New block or copy of an existing block, increase file usage.
        file_usage_add($current_file, 'image_grid_block', 'block', $current_file->fid);
      }
    }
    else {
      if (!empty($form['image_1']['#default_value'])) {
        // Old image was removed, no new one added.
        $old_file = file_load($form['image_1']['#default_value']);
        if ($old_file) {
          file_usage_delete($old_file, 'image_grid_block');
        }
      }
    }
    $block['image_1'] = $form_state['values']['image_1'];

    // Image 2

    if (!empty($form_state['values']['image_2'])) {
      $current_file = file_load($form_state['values']['image_2']);
      if (!$this->is_new) {
        // Edit an existing block.
        if ($form_state['values']['image_2'] != $form['image_2']['#default_value']) {
          // Set file usage on new file, also marks permanent.
          file_usage_add($current_file, 'image_grid_block', 'block', $current_file->fid);
          if (!empty($form['image_2']['#default_value'])) {
            // Remove usage from old (replaced) file.
            $old_file = file_load($form['image_2']['#default_value']);
            if ($old_file) {
              file_usage_delete($old_file, 'image_grid_block');
            }
          }
        }
      }
      else {
        // New block or copy of an existing block, increase file usage.
        file_usage_add($current_file, 'image_grid_block', 'block', $current_file->fid);
      }
    }
    else {
      if (!empty($form['image_2']['#default_value'])) {
        // Old image was removed, no new one added.
        $old_file = file_load($form['image_2']['#default_value']);
        if ($old_file) {
          file_usage_delete($old_file, 'image_grid_block');
        }
      }
    }
    $block['image_2'] = $form_state['values']['image_2'];

    // Image 3

    if (!empty($form_state['values']['image_3'])) {
      $current_file = file_load($form_state['values']['image_3']);
      if (!$this->is_new) {
        // Edit an existing block.
        if ($form_state['values']['image_3'] != $form['image_3']['#default_value']) {
          // Set file usage on new file, also marks permanent.
          file_usage_add($current_file, 'image_grid_block', 'block', $current_file->fid);
          if (!empty($form['image_3']['#default_value'])) {
            // Remove usage from old (replaced) file.
            $old_file = file_load($form['image_3']['#default_value']);
            if ($old_file) {
              file_usage_delete($old_file, 'image_grid_block');
            }
          }
        }
      }
      else {
        // New block or copy of an existing block, increase file usage.
        file_usage_add($current_file, 'image_grid_block', 'block', $current_file->fid);
      }
    }
    else {
      if (!empty($form['image_3']['#default_value'])) {
        // Old image was removed, no new one added.
        $old_file = file_load($form['image_3']['#default_value']);
        if ($old_file) {
          file_usage_delete($old_file, 'image_grid_block');
        }
      }
    }
    $block['image_3'] = $form_state['values']['image_3'];

    // Extended attributes.

    $block['image_style'] = $form_state['values']['extended']['image_style'];

    $config = config('image_grid_block.block.' . $block['machine_name']);
    $config->setData($block);
    $config->save();
  }

  /**
   * Try to extract a width value from an image style.
   *
   * Image styles do not necessarily scale or crop, so a width value may exist
   * or not.
   *
   * @param array $image_style
   *   The image style definitions.
   *
   * @return
   *   Number (width in pixels) or FALSE if the style doesn't resize.
   */
  protected function extractEffectWidth($image_style) {
    $style_widths = &backdrop_static(__METHOD__, array());
    $style_name = $image_style['name'];

    if (!isset($style_widths[$style_name])) {
      $width = FALSE;
      foreach ($image_style['effects'] as $index => $values) {
        if (!empty($values['data']['width'])) {
          // The last width value wins.
          $width = $values['data']['width'];
        }
      }
      $style_widths[$style_name] = $width;
    }

    return $style_widths[$style_name];
  }

}