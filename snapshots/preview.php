<?php
/**
* List for previewing daily images
* @author: Jodaille
* https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php
*/
$aDatas = [];

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
elseif(isset($_GET['date']))
{
    $date = new DateTime($_GET['date']);
}
else
{
    $date = new DateTime('now');
}

$ymd = $date->format('Y-m-d');
$aImages = glob("$ymd*.jpg");

$iNbImages = count($aImages);

//echo '<pre>';var_dump($ymd,$aImages);die();

if($iNbImages == 0)
{
    $aImages = glob("$ymd/$ymd*.jpg");
}

// time in image filename
$aSearch = ["$ymd/","$ymd-",'.jpg'];


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
        <p>Daily images ( <?php echo $iNbImages; ?> ): <?php echo $ymd; ?>
            <a href="/snapshots/preview.php?day=yesterday">yesterday</a>
        </p>

<?php
echo '<img id="imgPreview" src="' . $firstImage['src'] . '" />'
. '<span id="time">' . $firstImage['time'] . "</span>\n"
. '<span id="selectedImage"><a id="currentImgLink" target="_blank">' . "</a></span>\n"
;
?>
    </div>
    <div name="gitcode">
        <p>see: https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php</p>
    </div>
</body>


<script type="text/javascript">
window.onload = function() {
    var imagesDatas           = <?php echo $jsonData; ?>;
    var imagePreview          = document.getElementById('imgPreview');
    var spanTime              = document.getElementById("time");
    var spanSelectedImage     = document.getElementById("selectedImage");
    var currentImgLink        = document.getElementById("currentImgLink");


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
            currentImgLink.innerHTML = value.time;
            currentImgLink.setAttribute('href', imagePreview.src);

            imageID++;
        }

    }
    setInterval(changeImage, 200);

    /*imagePreview.onclick = function() {

        console.log('src:' + imagePreview.src);
    };*/
}


</script>
</html>
