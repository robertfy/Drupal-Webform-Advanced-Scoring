<?php

/**
 * @file
 */
?>
<form method="post" >
    <input type="hidden" name="nid" value="<?php echo $node->webform['nid']; ?>" />
    <input type="hidden" name="form_token" value="<?php echo md5('enable_advanced_scoring'); ?>" />

    <div>
        <!--<input type="checkbox" name="check_enable" id="check_enable"/>
         <label for="check_enable">Enable Scoring.</label></div> -->
    <div>
        <input type="submit" value="Enable Advanced Scoring" />
    </div>
</form>
