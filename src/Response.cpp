#include "etcd/Response.hpp"
#include "json_constants.hpp"

pplx::task<etcd::Response> etcd::Response::create(pplx::task<web::http::http_response> response_task)
{
  return pplx::task<etcd::Response> ([response_task](){
      web::http::http_response resp;
      web::json::value json_value;

      try {
          resp = response_task.get();
          json_value = resp.extract_json().get();
          return etcd::Response(resp, json_value);
      }
      catch (std::exception const& ex) {
          etcd::Response resp;
          resp._error_code = -1;
          resp._error_message = ex.what();
          return resp;
      }
    });
}

etcd::Response::Response()
  : _error_code(0),
    _index(0)
{
}

etcd::Response::Response(web::http::http_response http_response, web::json::value json_value)
  : _error_code(0),
    _index(0)
{
  if (http_response.headers().has(utility::conversions::to_string_t(JSON_ETCD_INDEX)))
    _index = atoi(utility::conversions::to_utf8string(http_response.headers()[utility::conversions::to_string_t(JSON_ETCD_INDEX)]).c_str());

  if (json_value.has_field(utility::conversions::to_string_t(JSON_ERROR_CODE)))
  {
    _error_code = json_value[utility::conversions::to_string_t(JSON_ERROR_CODE)].as_number().to_int64();
    _error_message = utility::conversions::to_utf8string(json_value[utility::conversions::to_string_t(JSON_MESSAGE)].as_string());
  }

  if (json_value.has_field(utility::conversions::to_string_t(JSON_ACTION)))
    _action = utility::conversions::to_utf8string(json_value[utility::conversions::to_string_t(JSON_ACTION)].as_string());

  if (json_value.has_field(utility::conversions::to_string_t(JSON_NODE)))
  {
    if (json_value[utility::conversions::to_string_t(JSON_NODE)].has_field(utility::conversions::to_string_t(JSON_NODES)))
    {
      std::string prefix = utility::conversions::to_utf8string(json_value[utility::conversions::to_string_t(JSON_NODE)][utility::conversions::to_string_t(JSON_KEY)].as_string());
      for (auto & node : json_value[utility::conversions::to_string_t(JSON_NODE)][utility::conversions::to_string_t(JSON_NODES)].as_array())
      {
        _values.push_back(Value(node));
        _keys.push_back(utility::conversions::to_utf8string(node[utility::conversions::to_string_t(JSON_KEY)].as_string().substr(prefix.length() + 1)));
      }
    }
    else
      _value = Value(json_value.at(utility::conversions::to_string_t(JSON_NODE)));
  }

  if (json_value.has_field(utility::conversions::to_string_t(JSON_PREV_NODE)))
    _prev_value = Value(json_value.at(utility::conversions::to_string_t(JSON_PREV_NODE)));
}

int etcd::Response::error_code() const
{
  return _error_code;
}

std::string const & etcd::Response::error_message() const
{
  return _error_message;
}

int etcd::Response::index() const
{
  return _index;
}

std::string const & etcd::Response::action() const
{
  return _action;
}

bool etcd::Response::is_ok() const
{
  return error_code() == 0;
}

etcd::Value const & etcd::Response::value() const
{
  return _value;
}

etcd::Value const & etcd::Response::prev_value() const
{
  return _prev_value;
}

etcd::Values const & etcd::Response::values() const
{
  return _values;
}

etcd::Value const & etcd::Response::value(int index) const
{
  return _values[index];
}

etcd::Keys const & etcd::Response::keys() const
{
  return _keys;
}

std::string const & etcd::Response::key(int index) const
{
  return _keys[index];
}
