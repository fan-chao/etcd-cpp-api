#include "etcd/Value.hpp"
#include "json_constants.hpp"

etcd::Value::Value()
  : dir(false),
    created(0),
    modified(0)
{
}

etcd::Value::Value(web::json::value const & json_value)
  : _key(utility::conversions::to_utf8string(json_value.has_field(utility::conversions::to_string_t(JSON_KEY)) ? json_value.at(utility::conversions::to_string_t(JSON_KEY)).as_string() : U(""))),
    dir(json_value.has_field(U("dir"))),
    value(utility::conversions::to_utf8string(json_value.has_field(utility::conversions::to_string_t(JSON_VALUE)) ? json_value.at(utility::conversions::to_string_t(JSON_VALUE)).as_string() : U(""))),
    created(json_value.has_field(utility::conversions::to_string_t(JSON_CREATED)) ? json_value.at(utility::conversions::to_string_t(JSON_CREATED)).as_number().to_int64() : 0),
    modified(json_value.has_field(utility::conversions::to_string_t(JSON_MODIFIED)) ? json_value.at(utility::conversions::to_string_t(JSON_MODIFIED)).as_number().to_int64() : 0)
{
}

std::string const & etcd::Value::key() const
{
  return _key;
}

bool etcd::Value::is_dir() const
{
  return dir;
}

std::string const & etcd::Value::as_string() const
{
  return value;
}

int etcd::Value::created_index() const
{
  return created;
}

int etcd::Value::modified_index() const
{
  return modified;
}
