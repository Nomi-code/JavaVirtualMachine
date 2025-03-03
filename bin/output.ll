; ModuleID = 'demo'
source_filename = "demo"

define i32 @"add function"(i32 %0, i32 %1) {
entry:
  %2 = add i32 %0, %1

return:                                           ; No predecessors!
  ret i32 %2
}
