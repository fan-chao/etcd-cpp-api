#ifndef __ETCD_RESPONSE_HPP__
#define __ETCD_RESPONSE_HPP__

#include <cpprest/http_client.h>
#include <string>
#include <vector>

#include "etcd/Value.hpp"
#include "macros.h"

namespace etcd
{
  typedef std::vector<std::string> Keys;

  /**
   * The Reponse object received for the requests of etcd::Client
   */
  class Response
  {
  public:
    static pplx::task<Response> create(pplx::task<web::http::http_response> response_task);

    ETCD_API Response();

    /**
     * Returns true if this is a successful response
     */
    ETCD_API bool is_ok() const;

    /**
     * Returns the error code received from the etcd server. In case of success the error code is 0.
     */
    ETCD_API int error_code() const;

    /**
     * Returns the string representation of the error code
     */
    ETCD_API std::string const & error_message() const;

    /**
     * Returns the action type of the operation that this response belongs to.
     */
    ETCD_API std::string const & action() const;

    /**
     * Returns the current index value of etcd
     */
    ETCD_API int index() const;

    /**
     * Returns the value object of the response to a get/set/modify operation.
     */
    ETCD_API Value const & value() const;

    /**
     * Returns the previous value object of the response to a set/modify/rm operation.
     */
    ETCD_API Value const & prev_value() const;

    /**
     * Returns the index-th value of the response to an 'ls' operation. Equivalent to values()[index]
     */
    ETCD_API Value const & value(int index) const;

    /**
     * Returns the vector of values in a directory in response to an 'ls' operation.
     */
    ETCD_API Values const & values() const;

    /**
     * Returns the vector of keys in a directory in response to an 'ls' operation.
     */
    ETCD_API Keys const & keys() const;

    /**
     * Returns the index-th key in a directory listing. Same as keys()[index]
     */
    ETCD_API std::string const & key(int index) const;

  protected:
    Response(web::http::http_response http_response, web::json::value json_value);

    int         _error_code;
    std::string _error_message;
    int         _index;
    std::string _action;
    Value       _value;
    Value       _prev_value;
    Values      _values;
    Keys        _keys;
  };
}

#endif
