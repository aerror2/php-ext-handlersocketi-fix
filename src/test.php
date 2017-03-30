<?php



$ret =hsi_startswith("player_items","xs"); 
var_dump($ret);

$ret =hsi_startswith("player_items","player_i");
var_dump($ret);

$testsql = array(
"`user_id` = '\\'xx204'",
"`user_id`='1022404'",
"col_1  in (2.1,'\\'xxdfs') or   c IN ( 1, 2 , '3' ) AND d=3 and ef > 4 limit 2,1"
,"col_1  <= 1 AND  c IN ( 1, 2 , 3 ) "
,"col_1  <= '1' AND  c IN ( 1, 2 , '3' ) "
,"col_1 <= '1' AND  c IN ( 1, 2 , '3' ) "
,"col_1  in (1.1) "
,"col_1   = 1.1 "
,"col_1   = -1.1 "
,"col_1   = 0X1000 "
,"col_1   = '1,1' "
,"col_1  in ('1','\\'xxdfs') AND   c IN ( 1, 2 , '3' )"
,"col_1  in (2.1,'\\'xxdfs') AND   c IN ( 1, 2 , '3' )"
,"col_1  in (2.1,'\\'xxdfs') AND   c IN ( 1, 2 , '3' ) AND d=3 and ef > 4"
,"col_1  in (2.1,'\\'xxdfs') AND   c IN ( 1, 2 , '3' ) AND d=3 and ef > 4 limit 1"
,"col_1  in (2.1,'\\'xxdfs') AND   c IN ( 1, 2 , '3' ) AND d=3 and ef > 4 limit 2,1"
,"((`user_id` = '1724752' or `user_id` = 0) and `expire_time` >= '1415251088') and (server_id = 0 or server_id = '7') and `mail_time` > 1000"
);

foreach($testsql as $sql)
{
	echo $sql."\n";
	$start = microtime(true);
	$arr = hsi_parse_sql_where($sql);
	$cost = microtime(true)-$start;
	$result = is_array($arr)?print_r($arr,true):$arr;
	print("parse $result cost $cost\n");
}



