if(isset($_GET['php']))
	die(phpinfo());
$microtime_start=microtime(true);
$sid=session_id();
$_SESSION['microtime_start']=$microtime_start;
##
$types['toggle']="V&auml;xla";
$types['flash']="Blinka";
$types['run']="K&ouml;r";
##
$commands[3]=array('image'=>"slanggunga",'title'=>"Rudolfs mule",'toggle'=>TRUE);
$commands[5]=array('image'=>"entre",'title'=>"V&auml;nster lastbilsframljus",'toggle'=>TRUE);
$commands[4]=array('image'=>"slanggunga",'title'=>"H&ouml;ger lastbilsframljus",'toggle'=>TRUE);
$commands[6]=array('image'=>"slanggunga",'title'=>"Belysning logistikcentret",'toggle'=>TRUE);
$commands[8]=array('image'=>"korv",'title'=>"Snurra p&aring; proppellern",'toggle'=>TRUE);
$commands[9]=array('image'=>"korv",'title'=>"Str&aring;lkastare 1",'toggle'=>TRUE);
$commands[10]=array('image'=>"korv",'title'=>"Str&aring;lkastare 2",'toggle'=>TRUE);
$commands[12]=array('image'=>"korv",'title'=>"Snurra radar &aring;t v&auml;nster",'toggle'=>TRUE);
$commands[11]=array('image'=>"korv",'title'=>"Snurra radar &aring;t h&ouml;ger",'toggle'=>TRUE);
$commands[12]=array('image'=>"korv",'title'=>"Belysning lilla huset",'toggle'=>TRUE);
$commands[26]=array('image'=>"glass",'title'=>"Inflygningsljus",'toggle'=>TRUE);
$commands[29]=array('image'=>"shoppar",'title'=>"Landningsbana",'toggle'=>TRUE);
##
if(isset($_GET['toggle']) OR isset($_GET['flash']) OR isset($_GET['run'])) {
	$address="37.123.157.142";
	$port=5555;
	if(isset($port) AND ($socket=socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) AND @(socket_connect($socket, $address, $port))) {
		$text="Connection successful on IP $address, port $port<BR>";
		$buf="LOGIN;".$sid.";PASSWORD\n";
		echo "socket_write($socket,$buf,strlen($buf));";
		socket_write($socket,$buf,strlen($buf));
		if(isset($_GET['run']) AND isset($commands[$_GET['run']]) AND isset($commands[$_GET['run']]['run'])) {
			$buf="RELAY;LED".$_GET['run']."\n";
			echo "socket_write($socket,$buf,strlen($buf));";
			socket_write($socket,$buf,strlen($buf));
		}
		if(isset($_GET['toggle']) AND isset($commands[$_GET['toggle']]) AND isset($commands[$_GET['toggle']]['toggle'])) {
			$buf="RELAY;LED".$_GET['toggle']."\n";
			echo "socket_write($socket,$buf,strlen($buf));";
			socket_write($socket,$buf,strlen($buf));
		}
		if(isset($_GET['flash']) AND isset($commands[$_GET['flash']]) AND isset($commands[$_GET['flash']]['flash'])) {
			$buf="RELAY;LED".$_GET['flash']."\n";
			for($i=0;$i<=11;$i++) {
				echo "socket_write($socket,$buf,strlen($buf));";
				socket_write($socket,$buf,strlen($buf));
				usleep(400000);
			}
		}
		$buf="LOGOUT;".$sid."\n";
		echo "socket_write($socket,$buf,strlen($buf));";
		socket_write($socket,$buf,strlen($buf));
		socket_close($socket);
	} else {
		die("<H1>Server offline</H1>");
	}
	die();
}
##
if(isset($_GET['new'])) {
	mysql_delete("ips",array('ip'=>$_SERVER['REMOTE_ADDR']));
}
$exists=find_in_db("ips",array('ip'=>$_SERVER['REMOTE_ADDR'],'country'=>TRUE),"*");
if(!$exists) {
	$data=save_ips($_SERVER['REMOTE_ADDR']);
	$txt="Ny IP ".$_SERVER['REMOTE_ADDR'];
	$txt.="\n".$data[0]['city'].", ".$data[0]['county'].", ".$data[0]['country'];
	if(isset($_GET['ref'])) {
		$txt.="\n".$_GET['ref'];
	}
	$txt.="\nhttp://maps.google.com/?q=".$data[0]['lat'].",".$data[0]['lng'];
#send_sms("0739066811",$txt);
}
if(isset($_GET['phone']) AND $_GET['phone']!="") {
	$no=preg_replace("/[^0-9+]/","",$_GET['phone']);
	$file1="F:\\www\\tivoli\\temp\\phone\\".$no.".txt";
	$file2="F:\\www\\tivoli\\temp\\phone\\".$_SERVER['REMOTE_ADDR'].".txt";
}
##
header("Cache-Control: max-age=60"); //1day (60sec)
header("Expires: ".gmdate("D, d M Y H:i:s", time() + 60)." GMT");
header("Content-Type: text/html; charset=UTF-8");
echo("<!DOCTYPE html>");
html_helper("html",array('xmlns'=>"http://www.w3.org/1999/xhtml",'xml:lang'=>"sv",'lang'=>"sv"));
html_helper("head");
html_helper("link",array('rel'=>"canonical",'href'=>"http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF'].""));
html_helper("link",array("rel"=>"icon","type"=>"image/png","href"=>"http://".$_SERVER['HTTP_HOST']."/".$GLOBALS['site']."/fav.jpg"));
html_helper("link",array("rel"=>"stylesheet","type"=>"text/css","href"=>"http://".$_SERVER['HTTP_HOST']."/".$GLOBALS['site']."/default.css"));
html_helper("meta",array("http-equiv"=>"Content-Type","content"=>"text/html;charset=UTF-8"));
html_helper("meta",array("name"=>"keywords","content"=>implode(", ",array("Tivoli","Pepparkakshus"))));
html_helper("meta",array("name"=>"description","content"=>"?rets pepparkaksbak! TIVOLI"));
html_helper("meta",array('name'=>"viewport",'content'=>"width=898"));
html_helper("title");
html_print("TOMTELOGISTIK");
html_helper("title",FALSE);
html_print("<!--[if lt IE 9]>");
html_helper("script",array('src'=>"http://html5shim.googlecode.com/svn/trunk/html5.js"));
html_helper("script",FALSE);
html_print("<![endif]-->");
//html_helper("script",array("type"=>"text/javascript","src"=>"../500/js/jscolor/jscolor.js"));
html_helper("script",FALSE);
html_helper("head",FALSE);
html_helper("body");
##
html_helper("div");
html_helper("h2",array('style'=>"font-size:28px;"));
html_print("TOMTELOGISTIK");
html_helper("h2",FALSE);
html_helper("p",array('style'=>"font-size:14px;"));
if(isset($_GET['phone'])) {
	if($no=="" OR strlen($no)<10) {
		html_print("Ogiltigt nummer har angetts.");
	} else if(file_exists($file1) AND abs(time()-filemtime($file1))<60*60) {
		html_print("Din ?nskan har redan framf?rts och det g&aring;r inte att skicka igen inom 1 timme (".ceil(24-abs(time()-filemtime($file1))/60)." minuter kvar).");
	} else if(file_exists($file2) AND abs(time()-filemtime($file2))<60*60) {
		html_print("En &ouml;nskan fr&aring;n din IP-adress har redan framf&ouml;rts och det g&aring;r inte att skicka igen inom 1 timme (".ceil(24-abs(time()-filemtime($file2))/60)." minuter kvar).");
	} else {
		$txt="K?r tivoli\n".$_SERVER['REMOTE_ADDR']."\n".date("H:i:s")."!\n".$_GET['phone']."\n".$no;
		if(isset($_GET['ref'])) {
			$txt.="\nRef: ".$_GET['ref'];
		}
		send_sms("0739066811",$txt);
		file_put_contents($file1,date("Y-m-d H:i:s"));
		file_put_contents($file2,date("Y-m-d H:i:s"));
		html_print("Din ?nskan har framf?rts.");
	}
	html_helper("p",FALSE);
} else if(FALSE) {
	html_helper("p",array('style'=>"font-size:14px;"));
	html_print("Sl&auml;nggungorna, karusellen och pariserhjulet k&ouml;rs endast manuellt. Skicka &ouml;nskem&aring;l om att vi ska k&ouml;ra ig&aring;ng dem genom att ange ditt telefonnummer h&auml;r:");
	html_helper("p",FALSE);
	form_start("Request start");
	html_helper("input",array('name'=>"phone"));
	form_submit("?nska");
	form_end();
}
html_helper("div",FALSE);
#
html_helper("table");
html_helper("tr");
html_helper("td");
#
html_helper("iframe",array('src'=>"http://37.123.157.142:8080/jsfs.html",'sandbox'=>"allow-scripts",'seamless'=>TRUE,'height'=>"527",'width'=>"878"));
html_helper("iframe",FALSE);
#
html_helper("td",FALSE);
html_helper("tr",FALSE);
html_helper("tr");
html_helper("td");
html_helper("table");
foreach($commands AS $pin => $command) {
	html_helper("tr",array('style'=>"background:".tr().";"));
	html_helper("th");
	html_helper("a",array('href'=>"http://".$_SERVER['HTTP_HOST']."/".$GLOBALS['site']."/images/".$command['image'].".jpg"));
	html_helper("img",array('alt'=>$command['title'],'src'=>"http://".$_SERVER['HTTP_HOST']."/".$GLOBALS['site']."/images/".$command['image'].".png",'style'=>"height:50px;width:50px;"));
	html_helper("a",FALSE);
	html_helper("th",FALSE);
	html_helper("th",array('style'=>"padding:18px;font-size:16px;"));
	html_print($command['title']);
	html_helper("th",FALSE);
	foreach($types AS $type => $title) {
		if(isset($command[$type])) {
			html_helper("td",array('style'=>"padding:0px;"));
			html_helper("a",array('href'=>"javascript:void(0)",'onclick'=>"socket_command(".$pin.",'".$type."')"));
			html_helper("button",array('style'=>"padding:16px;width:100px;height:50px;font-size:16px;"));
			html_print($title);
			html_helper("button",FALSE);
			html_helper("a",FALSE);
			html_helper("td",FALSE);
		}
	}
	html_helper("tr",FALSE);
}
html_helper("table",FALSE);
html_helper("td",FALSE);
html_helper("tr",FALSE);
html_helper("table",FALSE);
html_helper("div",FALSE);
#
js_jquery();
js_jquery(TRUE);
html_js("
function socket_command(pin,type) {
	var url='http://".$_SERVER['HTTP_HOST'].$_SERVER['PHP_SELF']."?' + type + '=' + pin;
	$.ajax({url: url, dataType:'json', success: function(data) {
		if(data.url!='') {
			alert(url+' 1 '+data);
		} else {
			alert(url+' 2 '+data);
		}
	},beforeSend:function(x){if(x && x.overrideMimeType) {x.overrideMimeType('application/j-son; charset=UTF-8');}},
	error: function(jqXHR, textStatus, errorThrown) {'error';}});
}
");
#
html_helper("body",FALSE);
html_helper("html",FALSE);
