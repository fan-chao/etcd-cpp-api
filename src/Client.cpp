#include "etcd/Client.hpp"

etcd::Client::Client(std::string const & address)
  : client(utility::conversions::to_string_t(address))
{
}

pplx::task<etcd::Response> etcd::Client::send_get_request(web::http::uri_builder & uri)
{
  return Response::create(client.request(web::http::methods::GET, uri.to_string()));
}

pplx::task<etcd::Response> etcd::Client::send_del_request(web::http::uri_builder & uri)
{
  return Response::create(client.request(web::http::methods::DEL, uri.to_string()));
}

pplx::task<etcd::Response> etcd::Client::send_put_request(web::http::uri_builder & uri, std::string const & key, std::string const & value)
{
  std::string data = key + "=" + value;
  std::string content_type = "application/x-www-form-urlencoded; param=" + key;
  return Response::create(client.request(web::http::methods::PUT, utility::conversions::to_utf8string(uri.to_string()), data, content_type));
}

pplx::task<etcd::Response> etcd::Client::get(std::string const & key)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  return send_get_request(uri);
}

//pplx::task<etcd::Response> etcd::Client::set(std::string const & key, std::string const & value)
//{
//  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
//  return send_put_request(uri, "value", value);
//}

pplx::task<etcd::Response> etcd::Client::set(std::string const & key, std::string const & value, int ttl, bool refresh)
{
    web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
    if (ttl > 0)
    {
        uri.append_query(utility::conversions::to_string_t("ttl=" + std::to_string(ttl)));
    }

    if (refresh) 
    {
        uri.append_query(U("refresh=true"));
    }
     
    return send_put_request(uri, "value", value);
}

pplx::task<etcd::Response> etcd::Client::add(std::string const & key, std::string const & value)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("prevExist=false"));
  return send_put_request(uri, "value", value);
}

pplx::task<etcd::Response> etcd::Client::modify(std::string const & key, std::string const & value)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("prevExist=true"));
  return send_put_request(uri, "value", value);
}

pplx::task<etcd::Response> etcd::Client::modify_if(std::string const & key, std::string const & value, std::string const & old_value)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("prevValue"), utility::conversions::to_string_t(old_value));
  return send_put_request(uri, "value", value);
}

pplx::task<etcd::Response> etcd::Client::modify_if(std::string const & key, std::string const & value, int old_index)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("prevIndex"), old_index);
  return send_put_request(uri, "value", value);
}

pplx::task<etcd::Response> etcd::Client::rm(std::string const & key)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("dir=false"));
  return Response::create(client.request(U("DELETE"), uri.to_string()));
}

pplx::task<etcd::Response> etcd::Client::rm_if(std::string const & key, std::string const & old_value)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("dir=false"));
  uri.append_query(U("prevValue"), utility::conversions::to_string_t(old_value));
  return send_del_request(uri);
}

pplx::task<etcd::Response> etcd::Client::rm_if(std::string const & key, int old_index)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("dir=false"));
  uri.append_query(U("prevIndex"), old_index);
  return send_del_request(uri);
}

pplx::task<etcd::Response> etcd::Client::mkdir(std::string const & key)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  return send_put_request(uri, "dir", "true");
}

pplx::task<etcd::Response> etcd::Client::rmdir(std::string const & key, bool recursive)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("dir=true"));
  if (recursive)
    uri.append_query(U("recursive=true"));
  return send_del_request(uri);
}

pplx::task<etcd::Response> etcd::Client::ls(std::string const & key)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("sorted=true"));
  return send_get_request(uri);
}

pplx::task<etcd::Response> etcd::Client::watch(std::string const & key, bool recursive)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("wait=true"));
  if (recursive)
    uri.append_query(U("recursive=true"));
  return send_get_request(uri);
}

pplx::task<etcd::Response> etcd::Client::watch(std::string const & key, int fromIndex, bool recursive)
{
  web::http::uri_builder uri(utility::conversions::to_string_t("/v2/keys" + key));
  uri.append_query(U("wait=true"));
  uri.append_query(U("waitIndex"), fromIndex);
  if (recursive)
    uri.append_query(U("recursive=true"));
  return send_get_request(uri);
}
