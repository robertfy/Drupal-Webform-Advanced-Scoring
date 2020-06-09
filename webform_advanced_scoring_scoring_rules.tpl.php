<?php

    /**
     * @file
     * Dsm($node);.
     */
?>
<h1>Scoring Rules</h1>
<?php if (count($scoring_count) == 0) { ?>
    <p>Hi, This is a Scoring Rules page. \n There are no rules created. Please create one.</p>
<?php }?>

<?php if(count($components) > 0){ ?>
    <form method="post" >
        <input type="hidden" name="nid" value="<?php echo $node->webform['nid']; ?>" />
        <input type="hidden" name="form_token" value="<?php echo md5('insert_new_rule'); ?>" />
        <div>
            <select name="cid">
                <?php foreach($components as $component){
                    print "<option value=\"{$component['cid']}\">" . $component['name'] . "</option>";
                } ?>
            </select>
        </div>
        <div>
            <textarea name='vs_pair' style="border:1px solid #000;">value_a|10</textarea>
        </div>
        <div>
            <input type="submit" value="Add Scoring Rule" />
        </div>
    </form>
<?php } ?>

<form method="post" >
    <input type="hidden" name="nid" value="<?php echo $node->webform['nid']; ?>" />
    <input type="hidden" name="form_token" value="<?php echo md5('disable_scoring'); ?>" />
    <div>
        <input type="submit" value="Disable Scoring" />
    </div>
</form>
