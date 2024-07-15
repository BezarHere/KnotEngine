#pragma once
#include "math/Rect.hpp"
#include "math/Transform.hpp"

#include "res/Texture.hpp"

#include "res/Mesh.hpp"

namespace kt
{

  struct RenderCommand
  {
    enum class CmdType
    {
      Transform,
      Rect,
      NinePatch,
      Box,
      Primitive2D,
      Primitive3D,
      Polygon2D,
      Polygon3D,

      Mesh
    };

    struct BaseData
    {
      // empty means the entire texture
      RectU texture_source;
      Texture *texture;
    };

    struct BaseData2D : public BaseData
    {
      uint32_t zindex;
    };

    struct BaseData3D : public BaseData
    {
    };

    struct TransformData
    {
      bool mode_2d;
      Transform2D transform_2d;
      Transform3D transform_3d;
    };

    struct RectData : public BaseData2D
    {
      Rect rect;
      Color tint;
      uint16_t flags;
    };

    struct NinePatchData : public BaseData2D
    {
      Rect rect;
      Color color;
      uint16_t flags;
      real_t margin[4];
      bool draw_center;
    };

    struct BoxData : public BaseData3D
    {
      Vec3 start;
      Vec3 end;
      Color tint;
    };

    struct Primitive2DData : public BaseData2D
    {
      Vec2 positions[4];
      Vec2us uvs[4];
      Color colors[4];
    };

    struct Primitive3DData : public BaseData3D
    {
      Vec2 positions[4];
      Vec2us uvs[4];
      Color colors[4];
    };

    struct BasePolygon
    {
      PrimitiveType type;
      size_t count;

      VertexBuffer *vertices; // required
      IndexBuffer *indicies; // optional
    };

    struct Polygon2D : public BaseData2D, public BasePolygon
    {
      Transform2D transform;
    };

    struct Polygon3D : public BaseData3D, public BasePolygon
    {
      Transform3D transform;
    };

    struct MeshData : public BaseData
    {
      Mesh *mesh;
    };

    RenderCommand(CmdType _type = CmdType::Transform);

    RenderCommand(RenderCommand &&move) noexcept;
    RenderCommand(const RenderCommand &copy);

    RenderCommand &operator=(const RenderCommand &copy);
    RenderCommand &operator=(RenderCommand &&move) noexcept;

    ~RenderCommand();

    template <typename _Proc>
    inline decltype(auto) __apply(_Proc &&proc);

    template <typename _Proc>
    inline decltype(auto) __apply() { return this->__apply(_Proc()); }

    union
    {
      BaseData2D base_2d;
      BaseData3D base_3d;

      TransformData transform;

      RectData rect;
      NinePatchData nine_patch;
      BoxData box;

      Primitive2DData primitive_2d;
      Primitive3DData primitive_3d;

      BasePolygon base_polygon;
      Polygon2D polygon_2d;
      Polygon3D polygon_3d;

      MeshData mesh;
    };

    CmdType type;
    RenderCommand *_next;
  };

  template<typename _Proc>
  inline decltype(auto) RenderCommand::__apply(_Proc &&proc) {
    switch (this->type)
    {
    case CmdType::Transform:
      return proc(this->transform);
    case CmdType::Rect:
      return proc(this->rect);
    case CmdType::NinePatch:
      return proc(this->nine_patch);
    case CmdType::Box:
      return proc(this->box);
    case CmdType::Primitive2D:
      return proc(this->primitive_2d);
    case CmdType::Primitive3D:
      return proc(this->primitive_3d);
    case CmdType::Polygon2D:
      return proc(this->polygon_2d);
    case CmdType::Polygon3D:
      return proc(this->polygon_3d);
    case CmdType::Mesh:
      return proc(this->mesh);
    default:
      throw std::bad_variant_access();
      //return proc(this->transform);
    }
  }
}
