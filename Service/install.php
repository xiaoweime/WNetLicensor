
<?php 
   $sqlcfg_file = "sql.inc";
?>

<?php
   function create_password($pw_length = 8)
   {
      $randpwd = '';
      for ($i = 0; $i < $pw_length; $i++) 
      {
         $randpwd .= chr(mt_rand(33, 126));
      }
      return $randpwd;
   }
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html>
   <head>
      <meta charset="utf-8">
      <title>Installation Wizard</title>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <meta name="description" content="">
      <meta name="author" content="">
      <link rel="stylesheet" href="assets/css/reset.css">
      <link rel="stylesheet" href="assets/css/supersized.css">
      <link rel="stylesheet" href="assets/css/style.css">

   </head>
   <body>
      <div class="page-container">
         <h1>Install NetLicensorService</h1>       
         <?php 
            if (!is_writable($sqlcfg_file))
            {
               printf("<br><br><font color=red>配置文件不可写</font><br><br>");
            }
            else
            {
               printf("<br> <br><font color=green>配置文件可写</font><br> <br>");
            }
         ?>
         <?php 
            if (isset($_POST['install']))
            {
               $config = sprintf("<?php\n
                  define(\"SQL_HOST\",\"%s\");\n
                  define(\"SQL_NAME\",\"%s\");\n
                  define(\"SQL_PASS\",\"%s\");\n
                  define(\"SQL_DB\",\"%s\");\n
                  ?>",
                  $_POST['db_host'],
                  $_POST['db_user'],
                  $_POST['db_pass'],
                  $_POST['db_dbname']);
               $pfile = fopen($sqlcfg_file,"w+");
               fwrite($pfile,$config);
               fclose($pfile);
               //include 'sql/user_info.hpp';
               include 'sql.inc';
               $db = new mysqli(SQL_HOST,SQL_NAME,SQL_PASS,SQL_DB);
               if (mysqli_connect_errno())
               {
                  printf("数据库连接失败! 请返回上一页检查连接参数<a href=install.php>返回修改</a>"); 
               }
               else
               {
                  $new_install_script_name = sprintf("install.php.%s",create_password(16));
                  printf("数据库连接成功!<br>install.php重命名为%s",$new_install_script_name);
                  rename("install.php",$new_install_script_name);
               }
            }
            else {
               printf("<form action=\"\" method=\"POST\"> \n
                  填写主机：<input type=\"text\"  class=\"username\" name=\"db_host\" 
                  placeholder=\"请输入服务器地址！\" value=\"localhost\"/><br> <br> \n
                  用 户 名：<input type=\"text\"  class=\"username\"  name=\"db_user\" 
                  placeholder=\"请输入数据库用户名！\" value=\"root\"/><br> <br> \n 
                  密　　码：<input type=\"text\"  class=\"username\" name=\"db_pass\" 
                  placeholder=\"请输入数据库密码！\" value=\"\"/><br> <br> \n
                  数据库名：<input type=\"text\"  class=\"username\" name=\"db_dbname\" 
                  placeholder=\"请输入数据库名称！\" value=\"WPUSERDATA\"/><br> <br>  \n
                  <button type=submit name=install>下一步</button> \n
                  </form>");
            }  
         ?>
 		 <div class="error">
			<span>+</span>
		 </div>

	  </div>
	  <div class="connect">
         <p>By:WProtect NetLicensorService</p>
	  </div>
	  <script type="text/javascript" src="./js/jquery-1.7.2.min.js"></script>
	  <script src="assets/js/supersized.3.2.7.min.js"></script>
	  <script src="assets/js/supersized-init.js"></script>
	  <script src="assets/js/scripts.js"></script>
   </body>
</html>


 

