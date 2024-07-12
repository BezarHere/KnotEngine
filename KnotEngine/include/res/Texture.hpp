#pragma once
#include "Resource.hpp"

namespace kt
{

  class Texture : public Resource
  {
  public:
    struct LocalData
    {

    };

    Texture();

  private:
    GLID m_id;
  };

}
