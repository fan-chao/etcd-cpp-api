#ifndef __ETCD_VECTOR_HPP__
#define __ETCD_VECTOR_HPP__

#include <cpprest/http_client.h>
#include "macros.h"
#include <string>
#include <vector>

namespace etcd
{
  /**
   * Represents a value object received from the etcd server
   */
  class Value
  {
  public:
    /**
     * Returns true if this value represents a directory on the server. If true the as_string()
     * method is meaningless.
     */
      ETCD_API bool is_dir() const;

    /**
     * Returns the key of this value as an "absolute path".
     */
      ETCD_API std::string const & key() const;

    /**
     * Returns the string representation of the value
     */
      ETCD_API std::string const & as_string() const;

    /**
     * Returns the creation index of this value.
     */
      ETCD_API int created_index() const;

    /**
     * Returns the last modification's index of this value.
     */
      ETCD_API int modified_index() const;

  protected:
    friend class Response;
    Value();
    Value(web::json::value const & json_value);
    std::string _key;
    bool        dir;
    std::string value;
    int         created;
    int         modified;
  };

  typedef std::vector<Value> Values;
}

#endif
