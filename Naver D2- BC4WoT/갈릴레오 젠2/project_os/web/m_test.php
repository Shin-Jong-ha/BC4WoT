<meta http-equiv="content-type" content="text/html; charset=euc-kr">

<!-- 모바일 페이지 -->
<head>

<meta name = "viewport" content = "width=device-width, initial-scale=1"/>
<style>
	A:link 
	{
		COLOR: black; TEXT-DECORATION: none
	}
	A:visited 
	{
		COLOR: blue; TEXT-DECORATION: none
	}
	A:hover
	{	
		COLOR: orange; TEXT-DECORATION: underline 
	}
</style>
</head>


<?
	$i = 0;
	$imageLocation		=	"/var/www/project_os/DB/homepage/image";

// DB에 저장되어 있는 바탕화면 그림을 불러온다.
	$fpImageDown		=	fopen("/var/www/project_os/DB/homepage/image/imageWhere/down", "r");
	$fpImageResult		=	fread($fpImageDown, 255);
	$imageResult		=	$imageLocation . "/" . $fpImageResult;

	$imageSearch		=	strpos($imageResult, ".jpg");
	$imageResult		=	substr($imageResult, 0, $imageSearch + 4);

	echo "<body background	=	'$imageResult'>";

// DB에 저장되어있는 새로고침 간격 시간을 읽어온다.
	$fpRefreshTime		=	fopen("/var/www/project_os/DB/homepage/refreshTime", "r");
	$refreshTimeResult	=	fread($fpRefreshTime, 255);

// DB에 저장되어있는 표 색깔을 읽어온다.
	$fpTableColor		=	fopen("/var/www/project_os/DB/homepage/tableColor", "r");
	$TableColorResult	=	fread($fpTableColor, 255);

// 아날로그 설정
	$analogLocation		=	"/var/www/project_os/DB/gpio_analog/";

// 변수(표)가 출력되는 php를 가져온다.
	require('./functionList.php');

	echo "<p align = center>";
	echo "<form action=modify_ok.php?no=$no method=post> ";

// 메뉴 표 시작
	echo "<table width = 330 height = 40 align = center><tr bgcolor= $TableColorResult>";
	echo "<td><p align = center><a href = 'setting.php'  width = 10>	<font color = white size = 2>Setting<br></a></font></p></td>";
	echo "<td><p align = center><a href = 'new.php'  width = 10>		<font color = white size = 2>New<br></a></font></p></td></tr><tr  bgcolor= $TableColorResult>";
	echo "<td><p align = center><a href = 'new_numorus.php'>			<font color = white size = 2>GroupControl<br></a></p></font></td>";
	echo "<td><p align = center><a href = 'sourceSelect.html'><font color = white size = 2>Source Code Modify<br></a></p></font></td></tr></table>";

	tableList(330);
// GPIO 표 시작
	echo "<table width = 330  border = 1 cellpadding='3' cellspacing='1' frame = 'void' align = center><tr bgcolor=$TableColorResult >";
	echo "<td align = center >	<font color = white size = 2><b>#</b></font></td>";
	echo "<td align = center>	<font color = white size = 2><b>PinData</b></font></td>";
	echo "<td align = center>	<font color = white size = 2><b>Control</b></font></td></tr><tr>";		
	echo "<br>";

// while문 0번 부터 13번 까지 돈다 (추가해야됨 : 아날로그)
	while($i < 20)
	{

// GPIO 설정 값을 읽어온다.
		$fp_config		= "/var/www/project_os/DB/gpio_config_DB/";
		$fp_config	   .= $i;

// GPIO 설정 텍스트 파일 fopen
		$config_open	= fopen($fp_config, "r+");
		$config_index	= fread($config_open , 1);

// 설정 값이 0이면 사용하지 않는 상태 : 출력되지 않음
		if($config_index == 0)
		{
			$i++;
			continue;
		}

// GPIO 데이터 값을 읽어온다.
		$fp_gpio		= "/var/www/project_os/DB/gpio/";
		$fp_gpio	   .= $i;

		$gpio_open		= fopen($fp_gpio, "r+");
		$gpio_index		= fread($gpio_open, 255);

// 수정 링크
		echo "<td bgcolor=white><p align = center><a href = 'modify.php?number=$i'>$i</a></p></td>";
		echo "<td bgcolor=white><p align = center>";
		
// 설정 값이 5면 PWM 임
		if($i <= 13)
		{
			if($config_index == 4)
			{
				$config_open	= fopen($fp_gpio, "r+");
				$gpio_index_temp = $gpio_index / 200000; // 1v = 1000000

				echo "$gpio_index_temp V</p></td>";
			}
			else
			{
				echo "$gpio_index</p></td>";
			}
		}
		else
		{
			$analogLocationResult = $analogLocation . $i;

			$analogSetting	= fopen($analogLocationResult, "r");
			$test			= fread($analogSetting, 255);

			echo "$gpio_index $test</p></td>";
		}

		echo "<td bgcolor=white>";

// 출력일 경우
		if($config_index == 1)
		{

// GPIO 상태가 HIGH 인 경우
			if($gpio_index == 1)
			{
				echo "<p align = center><a href = 'control.php?power=off&number=$i'>OFF</a></p>";
			}
// GPIO 상태가 LOW 인 경우
			else
			{
				echo "<p align = center><a href = 'control.php?power=on&number=$i'>ON</a></p>";
			}
		}

// 설정 값이 5면 PWM 임
		else if($config_index == 4)
		{
			echo "<p align = center><a href = 'modify_pwm.php?condition=sub&number=$i'>Decrease</a>  <a href = 'modify_pwm.php?condition=plus&number=$i'>Increase</a></p>";
		}
		echo "</td></tr>";
		$i++;
	}
	echo "</table>";
	echo "<p align = center>";

	fileList("/usr/local/apache/htdocs/project_os/DB/web_variable", 330);

	echo "</body>";
	echo "<meta http-equiv = 'Refresh' content=5; URL=test.php'>";
?>