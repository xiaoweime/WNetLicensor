<?php 
   include 'user_info.php';
   include '../def.inc';
   include '../sql.inc';
   class login extends wprotect_sql
   {
      function __construct($username,$password)
      {
         parent::__construct(SQL_HOST,SQL_NAME,SQL_PASS,SQL_DB);
         $query = sprintf("select UserName,PassWord from UserInfo where UserName = '%s' and PassWord = '%s'",
                         $username,
                         $password);
         if ($result = parent::sql_query($query))
         {
            $row_cnt = mysqli_num_rows($result);
            if ($row_cnt != 0)
            {
               //printf("login success\n"); 
               setcookie("UserName",$username);
               setcookie("PassWord",$password);
               printf("%s\n",LOGIN_SUCCESS);
            }
            else
            {
               //printf("login fail\n");
               printf("%s\n",LOGIN_FAIL);
            }
         }

      }
   }

   $username = $_POST['UserName'];
   $password = $_POST['PassWord'];
   $user_login = new login($username,$password);
   //$hash_password = hash(WPROTECT_HASH_TYPE,"new_pass1");
   //$user_login = new login("XiaoWei1",$hash_password);
   //echo hash(WPROTECT_HASH_TYPE,'test');
?>
