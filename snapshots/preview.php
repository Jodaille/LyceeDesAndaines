<?php
/**
* List for previewing daily images
* @author: Jodaille
* https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php
*/

$date = new DateTime('now');
$ymd = $date->format('Y-m-d');

//echo "<h1>$ymd</h1>\n";

$aImages = glob("$ymd*.jpg");

$aSearch = ["$ymd-",'.jpg'];

$aDatas = [];

foreach($aImages as $img)
{
    $time = str_replace($aSearch, '', $img);
    $aDatas[] = [
        'time' => $time,
        'src'  => $img,
        ];
    //echo "<img src=\"$img\" width=\"400\" height=\"220\"/>$time<hr/>";
    //echo "<img src=\"$img\" /><hr />";

}
$firstImage = $aDatas[0];
$jsonData = json_encode($aDatas);
?>
<!doctype html>
    <html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Preview of beehive entry snapshots</title>
    </head>
<body>
    <div class="preview">
        <p>Daily images: <?php echo $ymd; ?></p>
        <p>see: https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php</p>

<?php
echo '<img id="imgPreview" src="' . $firstImage['src'] . '" />'
. '<span id="time">' . $firstImage['time'] . "</span>\n";
?>
    </div>
</body>
<script type="text/javascript">
window.onload = function() {
    var imagesDatas  = <?php echo $jsonData; ?>;
    var imagePreview = document.getElementById('imgPreview');
    var spanTime     = document.getElementById("time");


      var keys = Object.keys(imagesDatas),
          len = keys.length,
          imageID = 0,
          prop,
          value;

      function changeImage(){
        prop                 = keys[imageID];
        value                = imagesDatas[prop];
        imagePreview.src     = value.src;
        spanTime.textContent = value.time;
        imageID++;
      }
      setInterval(changeImage, 200);
}


</script>
</html>
