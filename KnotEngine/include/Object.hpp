#pragma once
#include "utility/FieldVar.hpp"

#include "utility/SpaceTraits.hpp"


namespace kt
{
  enum class ObjectSpaceType : uint8_t
  {
    None,
    Space2D,
    Space3D,
  };

  class Object
  {
  public:
    enum _ObjFunctionalityFlags : uint8_t
    {
      eObjFnFlag_None = 0x00,

      eObjFnFlag_Drawable = 0x01,
      eObjFnFlag_Updatable = 0x02,

      eObjFnFlag_Physics = 0x04,
      eObjFnFlag_Invocable = 0x08,
      eObjFnFlag_HandlesInput = 0x10,

      eObjFnFlag_Global = 0x20,
    };

    typedef _ObjFunctionalityFlags ObjFnFlags;

    typedef FieldVar::Dict VarDict;
    typedef FieldVar::Array VarList;

    typedef Transform3D transform_type;
    typedef traits::SpaceTraits<transform_type> space_traits;

    typedef typename space_traits::direction_type direction_type;
    typedef typename space_traits::rotation_type rotation_type;

    struct Data
    {
      transform_type transform;

      int32_t render_priority = 0;
      int32_t update_priority = 0;
      int32_t physics_priority = 0;

      int32_t zindex = 0;
    };

    inline Object(ObjFnFlags fn_flags = eObjFnFlag_Updatable) : m_fn_flags{ fn_flags } {
    }

    inline virtual ~Object() {
    }

    inline virtual void draw() const {
    }
    inline virtual void update() {
    }

    inline virtual void physics() {
    }

    inline virtual void invoke(const VarDict &data) {
    }

    inline bool is_transform_dirty() const {
      return m_transform_dirty;
    }

    inline void mark_transform_dirty() {
      m_transform_dirty = true;
    }

    inline const transform_type &get_transform() const {
      return m_transform;
    }

    inline void set_transform(const transform_type &value) {
      m_transform = value;
      mark_transform_dirty();
    }

    inline ObjFnFlags get_function_flags() const noexcept {
      return m_fn_flags;
    }

  protected:
    VarDict m_variables;

    inline void mark_transform_clean() {
      m_transform_dirty = false;
    }

    transform_type m_transform = {};
  private:
    inline Object &operator=(const Object &) = delete;

    // // can't allocate an array of objects
    // static inline void *operator new[](size_t) = delete;
    // // can't deallocate an array of objects
    // static inline void operator delete[](void *) = delete;

  private:
    ObjectSpaceType m_space_type;
    const _ObjFunctionalityFlags m_fn_flags = eObjFnFlag_Updatable;
    bool m_transform_dirty = true;
  };
}