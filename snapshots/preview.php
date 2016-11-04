<?php
/**
* List for previewing daily images
* @author: Jodaille
* https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php
*/
if(isset($_GET['debug']) && $_GET['debug'])
{
    ini_set('error_reporting', E_ALL);ini_set('display_errors',true);
}

if(isset($_GET['day']))
{
    $day = 'now';
    if($_GET['day'] == 'yesterday')
    {
        $day = "$day -1 day";
    }
    $date = new DateTime($day);

}
else
{
    $date = new DateTime('now');
}

$ymd = $date->format('Y-m-d');
$aImages = glob("$ymd*.jpg");

//echo '<pre>';var_dump($ymd,$aImages);die();

$iNbImages = count($aImages);

if($iNbImages == 0)
{
    $date = new DateTime('now -2 days');
    $ymd = $date->format('Y-m-d');
    $aImages = glob("$ymd/$ymd*.jpg");
}

$aSearch = ["$ymd/","$ymd-",'.jpg'];

$aDatas = [];

foreach($aImages as $img)
{
    $time = str_replace($aSearch, '', $img);
    $aDatas[] = [
        'time' => $time,
        'src'  => $img,
        ];
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
        <p>Daily images ( <?php echo $iNbImages; ?> ): <?php echo $ymd; ?></p>
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
        if(imagesDatas[prop] !== undefined)
        {
            imagePreview.src     = value.src;
            spanTime.textContent = value.time;

            imageID++;
        }

    }
    setInterval(changeImage, 200);
}


</script>
</html>
