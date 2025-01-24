#include "../../include/runtime/klass.hpp"
#include "classFile/class_file.hpp"

using raw_jvm_data::ClassFile;
using raw_jvm_data::ConstantInfo;
using raw_jvm_data::ConstantUtf8;

void Klass::reinterpret(const ClassFile& classfile) {
    for (size_t index = 0; index < classfile.constant_pool_count; index++) {
        ConstantInfo* constant_info = classfile.constant_pool[index];
        switch (constant_info->tag) {
            case raw_jvm_data::CONSTANT_Utf8: {
                ConstantUtf8* utf8 = static_cast<ConstantUtf8*>(constant_info);
                StringPool::intern(reinterpret_cast<char*>(utf8->bytes));
                break;
            }
            case raw_jvm_data::CONSTANT_Methodref: {
                
                break;
            }
            case raw_jvm_data::CONSTANT_MethodHandle: {
                break;
            }
            case raw_jvm_data::CONSTANT_MethodType: {
                break;
            }
        }
    }
    for (size_t index = 0; index < classfile.methods_count; index++) {
    }
}
