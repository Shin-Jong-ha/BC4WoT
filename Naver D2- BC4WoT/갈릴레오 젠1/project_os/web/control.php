<?

/****************** ������(0, 1)�� �����ϱ� ���� ������ ********************/


	$power		=		$_GET['power'];						// ���� ����, ���� ���� ���� �о�´�.
	$number		=		$_GET['number'];					// GPIO #
	$fp			=		"/usr/local/apache/htdocs/project_os/DB/gpio/";		// GPIO ���
	$fp			.=		$number;							// GPIO ��ο� #�� ���Ѵ�.

// ���� ���°� ���� ������ ���
	if($power == "off")
	{
			$fp = fopen($fp, "r+");
			fputs($fp , "0"); // ���� 0�� �־ LOW�� �����.
	}
// ���� ���°� ���� ������ ���
	else
	{
			$fp = fopen($fp, "w+");
			fputs($fp, "1"); // ���� 1�� �־ HIGH�� �����.
	}

	echo "<meta http-equiv = 'Refresh' content='0.3; URL=test.php'>";
?>
