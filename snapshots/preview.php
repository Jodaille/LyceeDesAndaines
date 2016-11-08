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
    $iNbImages = count($aImages);
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
        <style>
        body {
            background-color:grey;
        }
        div[name="meteo"] {
            display: none;
        }
        iframe {
            width:300px;
            height:200px;
            border-image-repeat:stretch;
        }
        .informations{
            position: absolute;
            margin: 0px;
            top:    0px;
            left:   0px;
            width: 800px;
            height: 110px;
            background-color:white;
        }

        .preview{
            position: absolute;
            margin: 0px;
            top:    110px;
            left:   0px;
            width: 800px;
            height: 600px;
        }
        .temperatures{
            position: absolute;
            margin: 0px;
            top: 0px;
            left: 790px;
            width: 300px;
            height: 200px;
        }
        .luminosity{
            position: absolute;
            margin: 0px;
            top:  200px;
            left:   790px;
            width: 300px;
            height: 200px;
        }
        .humidity{
            position: absolute;
            margin: 0px;
            top:  400px;
            left:   790px;
            width: 300px;
            height: 200px;
        }
        .atmospheric{
            position: absolute;
            margin: 0px;
            top:    0px;
            left:   1090px;
            width: 300px;
            height: 200px;
        }
        .rainfall{
            position: absolute;
            margin: 0px;
            top:  200px;
            left:   1090px;
            width: 300px;
            height: 200px;
        }
        .rainlevel{
            position: absolute;
            margin: 0px;
            top:    400px;
            left:   1090px;
            width: 300px;
            height: 200px;
        }
        .gitcode{
            position: absolute;
            margin: 0px;
            top:    555px;
            left:   0px;
            width: 800px;
            height: 45px;
            /*background-color:white;*/
        }
        </style>
    </head>
<body style="">

    <div class="informations">
        <h1>Danvou Warré beehive</h1>

        <p>Daily images ( <?php echo $iNbImages; ?> ): <?php echo $ymd; ?>
            <span id="time"></span>
            <span id="selectedImage"><a id="currentImgLink" target="_blank"></span>
            <a href="/snapshots/preview.php?day=yesterday">(see yesterday)</a>&nbsp;
            <a href="/snapshots/preview.php?date=2016-11-01">(see 2016-11-01)</a>
            <a id="displayMeteo" href="#">Meteo</a>
        </p>
    </div>

    <div class="preview">
        <img id="imgPreview" src="/snapshots/last.jpg" />
    </div>

    <div name="meteo" id="meteo">
        <div class="temperatures">
            <iframe  frameborder="0"
                scrolling="no" marginheight="0" marginwidth="0"
                src="http://emoncms.jodaille.org/vis/multigraph?embed=1&mid=5">
            </iframe>
        </div>
        <div class="luminosity">
            <iframe frameborder="0"
                scrolling="no" marginheight="0" marginwidth="0"
                src="http://emoncms.jodaille.org/vis/multigraph?embed=1&mid=7">
            </iframe>
        </div>
        <div class="humidity">
            <iframe frameborder="0"
                scrolling="no" marginheight="0" marginwidth="0"
                src="http://emoncms.jodaille.org/vis/multigraph?embed=1&mid=9">
            </iframe>
        </div>
        <div class="atmospheric">
            <iframe frameborder="0"
                scrolling="no" marginheight="0" marginwidth="0"
                src="http://emoncms.jodaille.org/vis/multigraph?embed=1&mid=4">
            </iframe>
        </div>
        <div class="rainfall">
            <iframe frameborder="0"
                scrolling="no" marginheight="0" marginwidth="0"
                src="http://emoncms.jodaille.org/vis/multigraph?embed=1&mid=6">
            </iframe>
        </div>
        <div class="rainlevel">
            <iframe frameborder="0"
                scrolling="no" marginheight="0" marginwidth="0"
                src="http://emoncms.jodaille.org/vis/multigraph?embed=1&mid=8">
            </iframe>
        </div>
    </div>

    <div name="gitcode" class="gitcode">
        <p>see code: https://github.com/Jodaille/LyceeDesAndaines/blob/master/snapshots/preview.php</p>
    </div>
</body>


<script type="text/javascript">
window.onload = function() {
    var imagesDatas           = <?php echo $jsonData; ?>;
    var imagePreview          = document.getElementById('imgPreview');
    var spanTime              = document.getElementById("time");
    var spanSelectedImage     = document.getElementById("selectedImage");
    var currentImgLink        = document.getElementById("currentImgLink");
    var displayMeteoLink      = document.getElementById("displayMeteo");
    var meteo                 = document.getElementById("meteo");

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
            currentImgLink.innerHTML = value.src;
            currentImgLink.setAttribute('href', imagePreview.src);

            imageID++;
        }

    }
    setInterval(changeImage, 200);

    displayMeteoLink.onclick = function() {

        meteo.style.display = "inline";
        //console.log('src:' + imagePreview.src);
    };
}


</script>
</html>
