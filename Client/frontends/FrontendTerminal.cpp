#include <WClient.hpp>
#include <stdio.h>
#include <string.h>
#include <functional>
#include <iostream>
struct OP_STRUCT{
    const char * sz;
    size_t arg_count;
    void * fuc;
};

const OP_STRUCT Operation[] = {
    {"-login",2,(void*)&NetLicensorClient::login_user},
    {"-modify_password",3,(void*)&NetLicensorClient::modify_password}, 
    {"-reset_password",4,(void*)&NetLicensorClient::reset_password},
    {"-register",5,(void*)&NetLicensorClient::register_user} 
};

int main(int argc, char *argv[])
{
    //NetLicensorClient client(SERVICE_ROOT);
    NetLicensorClient client;
    //client.set_service_root(SERVICE_ROOT);
    if (argc > 1)
    {
        for (int i = 1;i < argc;i++)
        {

            bool find = false;
            for (int t = 0;t < 4;t++)
            {
                if (strcmp(Operation[t].sz,argv[i]) == 0)
                {/*{{{*/
                    find = true;
                    if (argc - i - 1 < Operation[t].arg_count)
                    {
                        printf("错误,指令:%s需要%d个参数\n",Operation[t].sz,Operation[t].arg_count);
                        return -1;
                    }
                    else
                    {

                        switch (Operation[t].arg_count)
                        {/*{{{*/
                         /*   case 2:
                                {
                                    typedef bool (NetLicensorClient::*Fuc2)(const char * o1,const char * o2);  
                                    //Fuc2 f = (Fuc2)Operation[i].fuc;
                                    Fuc2 *f = (Fuc2*)&Operation[t].fuc;
                                    (client.*(*f))(argv[i+1],argv[i+2]);
                                    //client.login_user(argv[i+1],argv[i+2]);
                                    i+=2; 
                                }

                                break;
                            case 3:
                                {
                                    typedef bool (NetLicensorClient::*Fuc3)(const char * o1,const char * o2,const char * o3);  
                                    //Fuc2 f = (Fuc2)Operation[i].fuc;
                                    Fuc3 * f = (Fuc3*)Operation[t].fuc;
                                    //Fuc2 * f = (Fuc2*)p;
                                    (client.*(*f))(argv[i+1],argv[i+2],argv[i+3]);  
                                    i+=3;
                                }
                                break;
                            case 4:
                                {
                                    typedef bool (NetLicensorClient::*Fuc4)(const char * o1,const char * o2,const char * o3,const char * o4);  
                                    //Fuc2 f = (Fuc2)Operation[i].fuc;
                                    Fuc4 * f = (Fuc4*)Operation[t].fuc;
                                    //Fuc2 * f = (Fuc2*)p;
                                    (client.*(*f))(argv[i+1],argv[i+2],argv[i+3],argv[i+4]);  
                                    i+=4;
                                }
                                break;
                            case 5:
                                {
                                    typedef bool (NetLicensorClient::*Fuc5)(const char * o1,const char * o2,const char * o3,const char * o4,const char * o5);  
                                    //Fuc2 f = (Fuc2)Operation[i].fuc;
                                    Fuc5 * f = (Fuc5*)Operation[t].fuc;
                                    //Fuc2 * f = (Fuc2*)p;
                                    (client.*(*f))(argv[i+1],argv[i+2],argv[i+3],argv[i+4],argv[i+5]);  
                                    i+=5;
                                }
                                break;                   */

                        }/*}}}*/

                        if (strcmp(Operation[t].sz,"-login") == 0)
                        {
                            client.login_user(argv[i+1],argv[i+2]);
                            //i+=2;
                        } else if (strcmp(Operation[t].sz,"-register") == 0)
                        {
                            client.register_user(argv[i+1],argv[i+2],argv[i+3],argv[i+4],argv[i+5]);
                            
                        } else if (strcmp(Operation[t].sz,"-modify_password") == 0)
                        {
                            client.modify_password(argv[i+1],argv[i+2],argv[i+3]);
                            //i+=;
                        }  else if (strcmp(Operation[t].sz,"-reset_password") == 0)
                        {
                            client.reset_password(argv[i+1],argv[i+2],argv[i+3],argv[i+4]);
                            //i+=4;
                        }   
                        i+=Operation[t].arg_count; 
                    }

                }/*}}}*/
            }
            if (find == false)
            {
                printf("未定义的操作 %s\n",argv[i]);
                return -1;
            }
            printf("%s",client.get_result());
        }
    }
    else
    {
        auto printf_user_arg = [](int count){
            printf(" ");
            for (int i = 0;i < count;i++)
            {
                printf("arg%d ",i+1);  
            }
            printf(" \n");
        }; 
        for (int i = 0;i < 4;i++)
        {
            printf("%s",Operation[i].sz);
            printf_user_arg(Operation[i].arg_count);
        }
    }
    //client.register_user("XiaoWei","pass_word","my name","xiaowei","0");
    //client.login_user("XiaoWei","pass_word");
    //printf(client.get_result());
    return 0;
}
