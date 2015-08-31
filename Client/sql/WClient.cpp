#include <curl/curl.h>
#include "WClient.hpp" 
#include <string.h>
#include <iostream>
#include <functional>
#include "sha256.h"

NetLicensorClient::NetLicensorClient(const char * _service_root)
    :service_root(_service_root)
{

}

NetLicensorClient::NetLicensorClient()
    :service_root(SERVICE_ROOT)
{

}

NetLicensorClient::~NetLicensorClient()
{

}

void NetLicensorClient::set_service_root(const char * url)
{
    //service_root = url;
}

bool NetLicensorClient::register_user(const char * user_name,
                const char * pass_word, 
                const char * secure_question,
                const char * secure_password,
                const char * machine_id)
{
    std::string post_arg;
    post_arg = "&UserName=";
    post_arg += user_name;
    post_arg += "&PassWord=";
    post_arg += pass_word;
    post_arg += "&SecureQuestion=";
    post_arg += secure_question;
    post_arg += "&SecurePassWord=";
    post_arg += secure_password;
    post_arg += "&MachineID=";
    post_arg += machine_id;
    //std::cout << post_arg << std::endl;
    return post("register.php",post_arg.c_str());
}

const char * NetLicensorClient::get_result()
{
    if (response.c_str() == nullptr)
        return "";
    return response.c_str();
}

bool NetLicensorClient::reset_password(const char * user_name,
                const char * secure_question,
                const char * secure_password,
                const char * new_password)
{
    if (
            strcmp(user_name,"") == 0 || 
            strcmp(secure_question,"") == 0 || 
            strcmp(secure_password,"") == 0 ||  
            strcmp(new_password,"") == 0
            )
    {
        response = "用户名密码安全问题安全答案不能为空\n";
        return false;
    }    
    std::string post_arg;
    post_arg = "&UserName=";
    post_arg += user_name;
    post_arg += "&SecureQuestion=";
    post_arg += secure_question;
    post_arg += "&SecurePassWord=";
    post_arg += secure_password; 
    post_arg += "&NewPassWord=";
    post_arg += new_password;   
    return post("reset_password.php",post_arg.c_str());
}

//

bool NetLicensorClient::login_user(const char * username,
                const char * password)
{
    
    unsigned char hash_buffer[33] = "\0";  
    std::string hash_password;
    if (strcmp(username,"") == 0 || strcmp(password,"") == 0)
    {
        response = "用户名密码不能为空\n"; 
        return false;
    }
    if (!SHA256((const unsigned char *)password,strlen(password),hash_buffer))
    {
        return false;
    }
    else
    {
        for (int i = 0;i < 32;i++)
        {
            char sz[3] = "\0";
            sprintf(sz,"%02x",hash_buffer[i]);
            hash_password += sz;
        }
    }
    std::string post_arg;
    post_arg = "&UserName=";
    post_arg += username;
    post_arg += "&PassWord=";
    post_arg += hash_password;
    //std::cout << post_arg << std::endl;
    return post("login.php",post_arg.c_str()); 
}

bool NetLicensorClient::modify_password(const char * username,
                const char * password,
                const char * new_password)
{
    unsigned char hash_buffer[33] = "\0";  
    std::string hash_password;
    if (!SHA256((const unsigned char *)password,strlen(password),hash_buffer))
    {
        return false;
    }
    else
    {
        for (int i = 0;i < 32;i++)
        {
            char sz[3] = "\0";
            sprintf(sz,"%02x",hash_buffer[i]);
            hash_password += sz;
        }
    }      
    if (strcmp(username,"") == 0 || strcmp(password,"") == 0 || strcmp(new_password,"") == 0)
    {
        response = "用户名密码和新密码不能为空\n";
        return false;
    }
    if (strcmp(new_password,password) == 0)
    {
        response = "原来的密码不能和新密码相等\n";
        return false;
    }

    std::string post_arg;
    post_arg = "&UserName=";
    post_arg += username;
    post_arg += "&PassWord=";
    post_arg += hash_password;//password;
    post_arg += "&NewPassWord=";
    post_arg += new_password; 
    return post("modify_password.php",post_arg.c_str()); 
}




static size_t my_curl_write_callback(void *ptr,size_t size,size_t nmemb,void * mydata)
{   
    std::string * str = dynamic_cast<std::string*>((std::string*)mydata);
    if (str == nullptr || ptr == nullptr )
    {
        return -1;
    }
     
    str->append(static_cast<char*>(ptr),size * nmemb);  
    return (nmemb);
} 


bool NetLicensorClient::post(const char * url,const char *post_arg)
{
    /*auto fuc = 
        [](void *ptr,size_t size,size_t nmemb,void * mydata)
        {
            std::string * str = dynamic_cast<std::string*>((std::string*)mydata);
            if (str == nullptr || ptr == nullptr )
            {
               return -1;
            }
            std::cout << nmemb << "size" << size << std::endl; 
            str->append(static_cast<char*>(ptr),size * nmemb);  
            return static_cast<int>(nmemb);
        };
    */
    //auto fuc = curl_write_callback;
    std::function<size_t(void*,size_t,size_t,void*)> callback = my_curl_write_callback;// = curl_write_callback;
    CURLcode res;
    CURL *curl = curl_easy_init();
    if (nullptr == curl)
    {
        return false;
    }
    std::string str_addr = service_root;
    response = "";
    str_addr += url;
    //std::cout << str_addr << std::endl;
    //std::cout << post_arg << std::endl;
    curl_easy_setopt(curl,CURLOPT_URL,str_addr.c_str());
    curl_easy_setopt(curl,CURLOPT_POST,1);
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post_arg);
    curl_easy_setopt(curl,CURLOPT_READFUNCTION,NULL);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,my_curl_write_callback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,static_cast<void *>(&response));
    curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);
    curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,5);
    curl_easy_setopt(curl,CURLOPT_TIMEOUT,5);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;
}
