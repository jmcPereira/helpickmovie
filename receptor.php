
<html>
	<head>
		<title>Example</title>
	</head>
	<body style="width: 960px;margin: 25px auto 0;">
	
	<?php
		function base64_to_jpeg($base64_string, $output_file) {
			$ifp = fopen($output_file, "wb");
			fwrite($ifp, base64_decode($base64_string)); 
			fclose($ifp); 

			return $output_file; 
		}
		
		$name = $_POST['imagem'];
		
		$output = 'imagem.jpg';
		$input = base64_to_jpeg($name,$output);
		$comandomr='release\MovieRecog.exe';
		$img='imagem.jpg';
		$result=exec("$comandomr $img");
		
		header("HTTP/1.1 200 $result");
	?>
	</body>
</html>