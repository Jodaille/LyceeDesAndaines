#!/usr/bin/env php
<?php
/**
* readLog.php
*/
$APIKEY  ='CHANGE ME WITH YOUR API KEY';
$WEBHOST = 'emoncms.jodaille.org';
$oParseLog = new ParseLog($APIKEY, $WEBHOST);


while($f = fgets(STDIN)){
    //echo "line: $f";
    $oParseLog->setLogLine($f);
}

class ParseLog
{
    protected $APIKEY    = '0d7911b21219f2d73e0a56abb164318e';
    protected $WEBHOST   = 'emoncms.jodaille.org';
    protected $APIURL    = '/input/post.json';
    protected $protocol  = 'http://';
    protected $delimiter = ';';
    protected $logLine;
    protected $data;
    protected $node = '';
    public function __construct($APIKEY, $WEBHOST, $protocol = null)
    {
        $this->APIKEY  = $APIKEY;
        $this->WEBHOST = $WEBHOST;
        if($protocol)
        {
            $this->protocol = $protocol;
        }
    }

    public function setLogLine($logLine, $send = true)
    {
        $this->logLine = $logLine;
        $datas = $this->_parseLine($logLine);
        $query = $this->_buildQuery($datas);
        if(!$send)
        {
            echo $query . "\n";
        }
        else
        {
            if($this->_sendQuery($query))
		{
	//		echo 'OK';
		}else
		{
	//		echo 'NOK';
		
		}
        }
    }

    private function _sendQuery($query)
    {
        try{
            $result = file_get_contents($query);
        }
        catch(Exception $e)
        {
            $result = false;
        }
        return $result;
    }

    private function _buildQuery($datas)
    {
        $query = $this->protocol . $this->WEBHOST . $this->APIURL;
        $query = $query . '?node=' . $this->node;
        $query = $query . '&apikey=' . $this->APIKEY;
        $query = $query . '&json=';
        $query = $query . json_encode($datas);
        return $query;
    }

    private function _parseLine($logLine)
    {
        $datas = array();
        if(false !== strpos($logLine, $this->delimiter))
        {
            $params = explode($this->delimiter, $logLine);
            foreach($params as $paramString)
            {
                $aParam = explode('=', $paramString);
                if(count($aParam) > 1)
                {
                    $name  = $aParam[0];
                    $value = $aParam[1];
                    if($name == 'id')
                    {
			$name = 'node';
                        $value = $this->_convertId($value);
			$this->node = $value;
                    }
		    else
		    {
                   	 $datas[$name] = $value;
		    }
                }
            }
        }
        return $datas;
    }

    private function _convertId($valueId)
    {
        $id = $valueId;
        if(strtolower($valueId) == strtolower('Danvou'))
        {
            $id = 5;
        }
        return $id;
    }

}

