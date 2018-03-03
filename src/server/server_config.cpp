#include "server/server_config.h"

#include <json-c/json_object.h>

#include <common/protocols/json_rpc/json_rpc.h>

#include <common/system_info/system_info.h>  // for SystemInfo, etc

// methods
#define GET_VERSION_METHOD "version"
#define SEND_STATISTIC_METHOD "statistic"
#define IS_SUBSCRIBED_METHOD "is_subscribed"

#define SUBSCRIBED_LOGIN_FIELD "email"
#define SUBSCRIBED_PASSWORD_FIELD "password"

#define STATISTIC_OS_FIELD "os"
#define STATISTIC_OS_NAME_FIELD "name"
#define STATISTIC_OS_VERSION_FIELD "version"
#define STATISTIC_OS_ARCH_FIELD "arch"

#define STATISTIC_PROJECT_FIELD "project"
#define STATISTIC_PROJECT_NAME_FIELD "name"
#define STATISTIC_PROJECT_VERSION_FIELD "version"
#define STATISTIC_PROJECT_ARCH_FIELD "arch"
#define STATISTIC_OWNER_FIELD "owner"
#define STATISTIC_PROJECT_EXEC_COUNT_FIELD "exec_count"

namespace fastonosql {
namespace server {

#ifndef IS_PUBLIC_BUILD
common::Error GenSubscriptionStateRequest(const std::string& login, const std::string& password, std::string* request) {
  if (login.empty() || password.empty() || !request) {
    return common::make_error_inval();
  }

  json_object* cred_json = json_object_new_object();
  json_object_object_add(cred_json, SUBSCRIBED_LOGIN_FIELD, json_object_new_string(login.c_str()));
  json_object_object_add(cred_json, SUBSCRIBED_PASSWORD_FIELD, json_object_new_string(password.c_str()));

  json_object* is_subscribed_json = NULL;
  common::protocols::json_rpc::JsonRPCRequest req;
  req.id = common::protocols::json_rpc::null_json_rpc_id;
  req.method = IS_SUBSCRIBED_METHOD;
  req.params = std::string(json_object_get_string(cred_json));
  common::Error err = common::protocols::json_rpc::MakeJsonRPCRequest(req, &is_subscribed_json);
  if (err) {
    json_object_put(cred_json);
    return err;
  }
  const char* command_json_string = json_object_get_string(is_subscribed_json);
  *request = command_json_string;
  json_object_put(is_subscribed_json);
  return common::Error();
}

common::Error ParseSubscriptionStateResponce(const std::string& data,
                                             common::protocols::json_rpc::JsonRPCResponce* result) {
  if (data.empty()) {
    return common::make_error_inval();
  }

  return common::protocols::json_rpc::ParseJsonRPCResponce(data, result);
}
#endif

common::Error GenVersionRequest(std::string* request) {
  if (!request) {
    return common::make_error_inval();
  }

  json_object* command_json = NULL;
  common::protocols::json_rpc::JsonRPCRequest req;
  req.id = common::protocols::json_rpc::null_json_rpc_id;
  req.method = GET_VERSION_METHOD;
  common::Error err = common::protocols::json_rpc::MakeJsonRPCRequest(req, &command_json);
  if (err) {
    return err;
  }

  const char* command_json_string = json_object_get_string(command_json);
  *request = command_json_string;
  json_object_put(command_json);
  return common::Error();
}

common::Error ParseVersionResponce(const std::string& data, common::protocols::json_rpc::JsonRPCResponce* result) {
  if (data.empty() || !result) {
    return common::make_error_inval();
  }

  return common::protocols::json_rpc::ParseJsonRPCResponce(data, result);
}

common::Error GenStatisticRequest(const std::string& login, uint32_t exec_count, std::string* request) {
  if (!request || login.empty()) {
    return common::make_error_inval();
  }

  json_object* stats_json = json_object_new_object();
  common::system_info::SystemInfo inf = common::system_info::currentSystemInfo();

  json_object* os_json = json_object_new_object();
  const std::string os_name = inf.GetName();
  json_object_object_add(os_json, STATISTIC_OS_NAME_FIELD, json_object_new_string(os_name.c_str()));
  const std::string os_version = inf.GetVersion();
  json_object_object_add(os_json, STATISTIC_OS_VERSION_FIELD, json_object_new_string(os_version.c_str()));
  const std::string os_arch = inf.GetArch();
  json_object_object_add(os_json, STATISTIC_OS_ARCH_FIELD, json_object_new_string(os_arch.c_str()));
  json_object_object_add(stats_json, STATISTIC_OS_FIELD, os_json);

  json_object* project_json = json_object_new_object();
  json_object_object_add(project_json, STATISTIC_PROJECT_NAME_FIELD, json_object_new_string(PROJECT_NAME));
  json_object_object_add(project_json, STATISTIC_PROJECT_VERSION_FIELD, json_object_new_string(PROJECT_VERSION));
  json_object_object_add(project_json, STATISTIC_PROJECT_ARCH_FIELD, json_object_new_string(PROJECT_ARCH));
#ifndef IS_PUBLIC_BUILD
  json_object_object_add(project_json, STATISTIC_OWNER_FIELD, json_object_new_string(login.c_str()));
#endif
  json_object_object_add(project_json, STATISTIC_PROJECT_EXEC_COUNT_FIELD,
                         json_object_new_int64(static_cast<int64_t>(exec_count)));
  json_object_object_add(stats_json, STATISTIC_PROJECT_FIELD, project_json);

  json_object* command_json = NULL;
  common::protocols::json_rpc::JsonRPCRequest req;
  req.id = common::protocols::json_rpc::null_json_rpc_id;
  req.method = SEND_STATISTIC_METHOD;
  req.params = std::string(json_object_get_string(stats_json));
  common::Error err = common::protocols::json_rpc::MakeJsonRPCRequest(req, &command_json);
  if (err) {
    json_object_put(stats_json);
    return err;
  }

  const char* command_json_string = json_object_get_string(command_json);
  *request = command_json_string;
  json_object_put(command_json);
  return common::Error();
}

common::Error ParseSendStatisticResponce(const std::string& data,
                                         common::protocols::json_rpc::JsonRPCResponce* result) {
  if (data.empty()) {
    return common::make_error_inval();
  }

  return common::protocols::json_rpc::ParseJsonRPCResponce(data, result);
}

}  // namespace server
}  // namespace fastonosql
