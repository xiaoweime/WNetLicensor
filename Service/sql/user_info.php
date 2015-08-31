<?php
   #include '../def.inc';
   class wprotect_sql
   {
      private $db;
      private $log;
      function __construct($sql_host,$sql_user,$sql_pass,$sql_db)
      { 
         $this->db = new mysqli($sql_host,$sql_user,$sql_pass,$sql_db);
         if (mysqli_connect_errno())
         {
            //printf("link sql object error,check you input\n");
            printf("%s",LINK_SQL_ERROR);
            $log = -1;
            exit;
         }     
         $query = "show tables like 'UserInfo'";
         $result = $this->db->query($query);
         if (mysqli_fetch_row($result) == 0)
         {
            $this->create_table();
         }
         $log = 1;
      }
      function sql_query($query)
      {
         $result = $this->db->query($query) ;
         return $result;
      }
      function unique_test($user_name)
      {
         //$query = "select UserName from UserInfo where UserName = '".$user_name."'";
         $query = sprintf("select UserName from UserInfo where UserName = '%s'",$user_name);
         //echo $query;
         if ($result = $this->db->query($query))
         {
            $row_cnt = mysqli_num_rows($result); 
            //printf("username repeat:%s,count:%d\n",$user_name,$row_cnt);
            if ($row_cnt)
              printf("%s",UNIQUE_USER_ERROR);
            $result->free();
            return $row_cnt;
         }
         return -1;
      }

      function insert_user($user_name,$pass_word,$secure_question,$secure_pass_word,$machine_id)
      {/*{{{*/
         if ($this->unique_test($user_name) != 0)
         {  
            return false;
         }
         $query = "insert into UserInfo values(?,?,?,?,?,?)";
         $cur_date = date('y-m-d h:i:s',time());
         $stmt = $this->db->prepare($query);
         if ($stmt == false)
         {
            $this->print_error();
            if (mysqli_errno())
            return false;
         }

         $stmt->bind_param(
            "ssssss",
            $user_name,
            $pass_word,
            $secure_question,
            $secure_pass_word,
            $machine_id,
            $cur_date);
         $stmt->execute();
         if ($stmt == false)
         {
            $this->print_error();
            return false;
         }
         
         if ($this->db->affected_rows)
         {
            //printf("Add User Success\n");
            printf("%s",ADD_USER_SUCCESS);
         }
         else
         {
            //printf("Add User Fail\n");
            printf("%s",ADD_USER_FAIL);
         }        
         return true;
      }/*}}}*/

      function create_table()
      {/*{{{*/   
         $query = 
         "create table UserInfo
          (
            UserName char(255) not null check(UserName != ''),
            PassWord char(255) not null check(PassWord != ''),
            SecureQuestion char(255) not null check(SecureQuestion != ''),
            SecurePassWord char(255) not null check(SecurePassWord != ''),
            MachineID char(255),
            Date datetime not null,

            primary key(UserName)
          )";

         if ($this->db->query($query))
         {
            printf ("create sql table done \n");
         }
         else
         {
            //printf("create sql table error:%d,%s\n",mysqli_errno($this->db),mysqli_error($this->db));
         }

      }/*}}}*/

      function delete_table()
      {
         $query = "drop table UserInfo";
         if ($this->db->query($query))
         {
            printf ("delete sql table success\n");
         }
         else
         {

         } 
      }
      //安全问题重新设置密码
      function reset_password($user_name,$secure_question,$secure_pass_word,$new_pass_word)
      {
         $query = sprintf(
            "update UserInfo set PassWord = '%s' where UserName = '%s' and SecureQuestion = '%s' and SecurePassWord = '%s'",
            $new_pass_word,
            $user_name,
            $secure_question,
            $secure_pass_word);
         //printf("%s\n",$query);
         if ($result = $this->db->query($query))
         {
            //$row_cnt = mysqli_num_rows($result); 
            //mysqli_affected_rows($result); 
            if ($this->db->affected_rows)
            {
               //printf("reset password success:%s,count:%d\n",$user_name,$this->db->affected_rows); 
               //printf("reset password success\n");
               printf("%s",RESET_PASSWORD_SUCCESS);
               return $this->db->affected_rows;
            }
            else
            {
               //printf("reset password error\n");
               printf("%s",RESET_PASSWORD_FAIL);
               return 0;
            }
            return $row_cnt;
         }
         return 0;    
          
      }
      //修改密码
      function modify_password($user_name,$pass_word,$new_pass_word)
      {
         $query = sprintf(
            "update UserInfo set PassWord = '%s' where UserName = '%s' and PassWord = '%s'",
            $new_pass_word,
            $user_name,
            $pass_word);
         //printf("%s\n",$query);
         if ($result = $this->db->query($query))
         {
            //$row_cnt = mysqli_num_rows($result); 
            //mysqli_affected_rows($result); 
            if ($this->db->affected_rows)
            {
               //printf("modify password success:%s,count:%d\n",$user_name,$this->db->affected_rows); 
               printf("%s",MODIFY_PASSWORD_SUCCESS);
               return $this->db->affected_rows; 
            }
            else
            {
               //printf("modify password error\n");
               printf("%s",MODIFY_PASSWORD_FAIL);
               return 0;
            }
         }
         return 0;   
      }
      function print_error()
      {
         //printf("error code:%d,%s\n",mysqli_errno($this->db),mysqli_error($this->db)); 
         printf("%s%d,%s\n",ERROR_PREFIX,mysqli_errno($this->db),mysqli_error($this->db));  
      }
      function __destruct()
      {
         $this->db->close();
      }
      function hash($data)
      {
         $result = hash(WPROTECT_HASH_TYPE,$data);
         return $result;
      }
   }
   /*include '../def.php';
   $user = new wprotect_sql(SQL_HOST,SQL_NAME,SQL_PASS,SQL_DB);
   $user->create_table();
   $user->insert_user("XiaoWei","Test","hello","world","abc");
   $user->modify_password("XiaoWei","Test","new_pass");
   $user->reset_password("XiaoWei","hello","world1","Test");*/
   //$user->delete_table();
?>

