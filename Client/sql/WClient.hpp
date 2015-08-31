#ifndef CLIENT_HPP
#define CLIENT_HPP

//#define SERVICE_ROOT "http://xiaoweime.s957.com/sql"
#include <WClientConfig.h>
#include <string>

class NetLicensorClient
{
    public:
        NetLicensorClient();
        explicit NetLicensorClient(const char * _service_root);
        ~NetLicensorClient();
        bool register_user(const char * user_name,
                const char * pass_word, 
                const char * secure_question,
                const char * secure_password,
                const char * machine_id);
        bool reset_password(const char * user_name,
                const char * secure_question,
                const char * secure_password,
                const char * new_password);
        bool login_user(const char * username, const char * password);
        bool modify_password(const char * username,
                const char * password,
                const char * new_password);
        void set_service_root(const char * url);
        const char * get_result();
    private:
        bool post(const char * url,const char * post_info);
        std::string response;
        const char * service_root;
};

#endif /* end of include guard: CLIENT_HPP */
