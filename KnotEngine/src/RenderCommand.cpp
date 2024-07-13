#include "pch.h"
#include "RenderCommand.hpp"
#include "utility/Indirection.hpp"

namespace kt
{
  RenderCommand::RenderCommand(CmdType _type)
    : type{ _type } {
    this->__apply<indir::EmptyCTor>();
  }

  RenderCommand::RenderCommand(RenderCommand &&move) noexcept
    : type{ move.type }, _next{ move._next } {
    this->__apply(indir::MoveCTor(&this->base_2d));
  }

  RenderCommand::RenderCommand(const RenderCommand &copy)
    : type{ copy.type }, _next{ nullptr } {
    this->__apply(indir::CopyCTor(&this->base_2d));
  }

  RenderCommand &RenderCommand::operator=(const RenderCommand &copy) {
    this->__apply<indir::DTor>();
    type = copy.type;
    this->__apply(indir::CopyAssign(&this->base_2d));
    return *this;
  }

  RenderCommand &RenderCommand::operator=(RenderCommand &&move) noexcept {
    this->__apply<indir::DTor>();
    type = move.type;
    this->__apply(indir::MoveAssign(&this->base_2d));
    return *this;
  }

  RenderCommand::~RenderCommand() {
    this->__apply<indir::DTor>();
  }

}
