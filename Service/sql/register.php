<?php 
   include 'user_info.php';
   include '../def.inc';
   include '../sql.inc';
   class register extends wprotect_sql
   {
      function __construct($username,$password,$secure_question,$secure_password,$machine_id)
      {
         parent::__construct(SQL_HOST,SQL_NAME,SQL_PASS,SQL_DB);
         $hash_password = parent::hash($password);
         parent::insert_user($username,$hash_password,$secure_question,$secure_password,$machine_id); 
      }
   }
   
   $username = $_POST['UserName'];
   $secure_question = $_POST['SecureQuestion'];
   $secure_password = $_POST['SecurePassWord'];
   $password = $_POST['PassWord'];
   $machine_id = $_POST['MachineID'];

   $username = trim($username);
   //$password = trim($password);
   if ($username == "" || $password == "")
   {
      exit;
   }

   $user_register = new register($username,$password,$secure_question,$secure_password,$machine_id);

   //$user_register = new register("XiaoWei1","new_pass1","my name?","xiaowei","123");
   //echo hash(WPROTECT_HASH_TYPE,'test');
?> 
