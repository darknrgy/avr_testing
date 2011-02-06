<?PHP

define('RANGE', 512);
$color = array(0,0,0);
$offsets = array(0, RANGE * (2/3), RANGE * (4/3));

for (;;){

	$a++;
	
	
	for ($phase = 0; $phase < 3; $phase ++){
		$color[$phase] =  abs(( ($a + $offsets[$phase]) % (RANGE * 2)) - RANGE) - RANGE/3;
		if ($color[$phase] < 0) $color[$phase] = 0;
		echo floor($color[$phase]) . "\t";
	}
	echo "\n";
	
	
	
	usleep(20000);	
}

