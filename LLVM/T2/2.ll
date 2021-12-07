; ModuleID = 'test'
source_filename = "test"

declare i32 @putchar(i32)

declare i32 @getchar()

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 0, i32* %a, align 4
  store i32 1, i32* %a, align 4
  %callgetchar = call i32 @getchar()
  store i32 %callgetchar, i32* %b, align 4
  %b1 = load i32, i32* %b, align 4
  %comp = icmp eq i32 %b1, 97
  %cond = icmp ne i1 %comp, false
  br i1 %cond, label %then, label %else

then:                                             ; preds = %entry
  store i32 89, i32* %a, align 4
  br label %ifcont

else:                                             ; preds = %entry
  store i32 78, i32* %a, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %a2 = load i32, i32* %a, align 4
  %callputchar = call i32 @putchar(i32 %a2)
  %callputchar3 = call i32 @putchar(i32 10)
  ret i32 0
}
