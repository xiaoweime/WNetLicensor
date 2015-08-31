<?php 
   include 'user_info.php';
   include '../def.inc';
   include '../sql.inc';
   class modify_password extends wprotect_sql
   {
      function __construct($username,$password,$new_password)
      {
         parent::__construct(SQL_HOST,SQL_NAME,SQL_PASS,SQL_DB);
         $hash_password = parent::hash($new_password);
         parent::modify_password($username,$password,$hash_password);
      }
   }
   $username = $_POST['UserName'];
   $password = $_POST['PassWord'];
   $new_pass = $_POST['NewPassWord'];
   $user_modify_password = new modify_password($username,$password,$new_pass);
?>
