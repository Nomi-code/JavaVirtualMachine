#pragma once

#include "java_base.hpp"
#include "klass.hpp"
#include "oop.hpp"
#include "gc.hpp"
#include "runtime/gc.hpp"


namespace jvm {
    class BytecodeEngine {
      public:
        using Handler = void (*)(StackFrame&);

        // 主解释循环
        static void interpret(StackFrame& frame);

        // 方便调试/反汇编：根据 opcode 获取名字
        static const char* opcode_name(std::uint8_t opcode);

#define X(code, name) static void op_##name(StackFrame& frame);
#include "byte_code_engine.def"
#undef X

      private:
        static std::array<Handler, 256> handlers;
    };
};

