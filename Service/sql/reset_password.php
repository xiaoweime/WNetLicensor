<?php 
   include 'user_info.php';
   include '../def.inc';
   include '../sql.inc';
   class reset_password extends wprotect_sql
   {
      function __construct($username,$secure_question,$secure_password,$new_password)
      {
         parent::__construct(SQL_HOST,SQL_NAME,SQL_PASS,SQL_DB);
         $hash_password = parent::hash($new_password);
         parent::reset_password($username,$secure_question,$secure_password,$hash_password);
      }
   }
   $username = $_POST['UserName'];
   $secure_question = $_POST['SecureQuestion'];
   $secure_password = $_POST['SecurePassWord'];
   $new_pass = $_POST['NewPassWord'];
   $user_reset_password = new reset_password($username,$secure_question,$secure_password,$new_pass);
   //$user_reset_password = new reset_password("XiaoWei1","my name?","xiaowei","new_pass");
?>
