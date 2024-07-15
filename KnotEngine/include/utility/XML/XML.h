#pragma once
#include "../Defs.hpp"
#include "../ArrayString.hpp"

#include <map>
#include <vector>

namespace kt
{
  enum class XMLNodeType
  {
    Root,

    Tag, // <NAME/>
    Content, // <NAME> blah blah </NAME>

    CDATA
  };

  struct XMLNode
  {
    static constexpr size_t AttributeNameMaxLength = 255;
    static constexpr size_t AttributeValueMaxLength = 255;

    typedef ArrayString<AttributeNameMaxLength> AttributeName;
    typedef std::string AttributeValue;

    XMLNodeType type;
    std::string name;
    std::string content;

    std::map<AttributeName, AttributeValue> attributes;

    std::vector<XMLNode> children;
  };

  class XMLDoc
  {
  public:


  private:
    std::vector<XMLNode> m_nodes;
  };

}
