#include "runtime/byte_code_engine.hpp"

#include <stdexcept>
#include <string>

namespace jvm {

    // === bytecode -> handler 表 ===
    std::array<BytecodeEngine::Handler, 256> BytecodeEngine::handlers = [] {
        std::array<BytecodeEngine::Handler, 256> a{}; // 全部初始化为 nullptr
#define X(code, name) a[code] = &BytecodeEngine::op_##name;
#include "runtime/byte_code_engine.def"
#undef X
        return a;
    }();

    // === bytecode -> 名字（调试用）===
    const char* BytecodeEngine::opcode_name(std::uint8_t opcode) {
        switch (opcode) {
#define X(code, name)                                                                              \
    case code:                                                                                     \
        return #name;
#include "runtime/byte_code_engine.def"
#undef X
            default:
                return "unknown";
        }
    }

    // === 解释循环实现 ===

    void BytecodeEngine::interpret(StackFrame& frame) {
        // TODO
        // while (!frame.halted) {
        //     std::uint8_t opcode = fetch_u1(frame);
        //     Handler h = handlers[opcode];
        //     if (!h) {
        //         throw std::runtime_error("Unknown opcode: " + std::to_string(opcode));
        //     }
        //     h(frame);
        // }
    }

    // === 各个字节码对应的 handler 实现 ===

    void BytecodeEngine::op_nop(StackFrame&) {
        // 什么也不做
    }

    void BytecodeEngine::op_iconst_m1(StackFrame& frame) {
    }
    void BytecodeEngine::op_iconst_0(StackFrame& frame) {
    }
    void BytecodeEngine::op_iconst_1(StackFrame& frame) {
    }
    void BytecodeEngine::op_iconst_2(StackFrame& frame) {
    }
    void BytecodeEngine::op_iconst_3(StackFrame& frame) {
    }
    void BytecodeEngine::op_iconst_4(StackFrame& frame) {
    }
    void BytecodeEngine::op_iconst_5(StackFrame& frame) {
    }

    void BytecodeEngine::op_iadd(StackFrame& frame) {
    }

    void BytecodeEngine::op_ireturn(StackFrame& frame) {
    }

} // namespace jvm
