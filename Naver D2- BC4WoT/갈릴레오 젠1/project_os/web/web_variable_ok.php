<?



	$name			=		$_GET['name'];
	$variableName	=		array();
	$variableIndex	=		array();
	$variableTemp	=		0;
	$i				=		0;
	$fpGpio			=		"/usr/local/apache/htdocs/project_os/DB/gpio/";

	$fp				=		"/usr/local/apache/htdocs/project_os/DB/web_variable/";
	
	$fp			   .=		$name;
	$fp				=		fopen($fp, "r");

// ó������ ������ �����͸� �о�´�. variableTemp���� 0�� 1�� �־ �����ư��鼭 �����ϵ�����.
	while(($test	= fread($fp , 1)) != NULL)
	{
		if($variableTemp == 0)
		{
			$variableName[$i] .= $test;
// ������Ⱑ �� ���
			if($test == ' ')
			{
				$fpGpioResult = $fpGpio;
				$fpGpioResult .= $variableName[$i];
				$fpGpioResult = trim($fpGpioResult);
				$fpGpioResult = fopen($fpGpioResult, "r+");

				$variableTemp = 1;
			}
		}
		else if($variableTemp == 1)
		{
// ������ ���̰ų� ���๮�� �� ���
			if($test == "\n" | $test == "\0")
			{
				$t = fwrite($fpGpioResult, $variableIndex[$i]);

				$variableTemp = 0;
				$i++;
			}
			else
			{
				$variableIndex[$i] .= $test;
			}
		}
	}
	fclose($fp);

	echo "<meta http-equiv = 'Refresh' content='0; URL=test.php'>";

?>
