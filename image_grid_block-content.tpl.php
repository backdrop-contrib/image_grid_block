<?php

/**
 * @file
 * Default theme implementation to display IIV Block content.
 *
 * Available variables:
 * - $image: Block image.
 * - $img_url: Image url (for background-image).
 * - $content: Block content.
 * - $wrapper_classes: Block content wrapper class(es).
 *
 * @see template_preprocess_block()
 */
?>
<div class="image_grid_block_wrapper <?php print $wrapper_classes;?>">
  <div class="image_grid_block image-one item">
    <?php print $image_1 ?>
    <div class="image-overlay">
      <span class="text-container">
        <?php print $content_1 ?>
      </span>
    </div>
  </div>
  <div class="image_grid_block image-two item">
    <?php print $image_2 ?>
    <div class="image-overlay">
      <span class="text-container">
        <?php print $content_2 ?>
      </span>
    </div>
  </div>
  <div class="image_grid_block image-three item">
    <?php print $image_3 ?>
    <div class="image-overlay">
      <span class="text-container">
        <?php print $content_3 ?>
      </span>
    </div>
  </div>
</div>