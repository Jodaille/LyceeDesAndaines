<?php
/**
* List for previewing daily images
* @author: Jodaille
* https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php
*/
$aDatas = [];
$currentImage = 0;
if(isset($_GET['current']))
{
    $currentImage = $_GET['current'];
}
$date = new DateTime('now');

$ymd = $date->format('Y-m-d');
$aImages = glob("$ymd*.jpg");

$iNbImages = count($aImages);
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
        <style>
        body {
            background-color:grey;
        }
        .informations{
            position: absolute;
            margin: 0px;
            top:    0px;
            left:   0px;
            width: 800px;
            height: 125px;
            background-color:white;
        }

        .preview{
            position: absolute;
            margin: 0px;
            top:    125px;
            left:   0px;
            width: 800px;
            height: 600px;
        }

        </style>
    </head>
<body style="">

    <div class="informations">
        <h1>Danvou Warré beehive</h1>

        <p>Daily images ( <?php echo $iNbImages; ?> ): <?php echo $ymd; ?>


            <span id="time"></span>
            <span id="selectedImage"><a id="currentImgLink" target="_blank"></span>
                <span id="imgRange">0</span>
                <input type="range" id="selectImage"
                min="0" max="<?php echo $iNbImages; ?>"
                step="1" style="width:90%"
                value="<?php echo $currentImage; ?>"
                oninput="displayImg(this.value)" onchange="displayImg(this.value)"
                autofocus tabindex="1" />
                <br >
        </p>
    </div>

    <div class="preview">
        <img id="imgPreview" src="/snapshots/last.jpg" />
    </div>


</body>


<script type="text/javascript">

window.onload = function() {

    var selectImage              = document.getElementById("selectImage");
    selectImage.autofocus = true;
    displayImg(selectImage.value);
}
var imagesDatas           = <?php echo $jsonData; ?>;
var keys = Object.keys(imagesDatas),
    len = keys.length,
    imageID = 0,
    prop,
    value;

function displayImg(imageID) {
    var imagePreview          = document.getElementById('imgPreview');
    var spanTime              = document.getElementById("time");
    var imgRange              = document.getElementById("imgRange");
    var spanSelectedImage     = document.getElementById("selectedImage");
    var currentImgLink        = document.getElementById("currentImgLink");

    prop                 = keys[imageID];
    value                = imagesDatas[prop];
    if(imagesDatas[prop] !== undefined)
    {
        imagePreview.src     = value.src;
        spanTime.textContent = value.time;
        imgRange.textContent = imageID;
        currentImgLink.innerHTML = value.src;
        currentImgLink.setAttribute('href', imagePreview.src);
    }

}

</script>
</html>
