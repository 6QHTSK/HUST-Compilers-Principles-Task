; ModuleID = 'test'
source_filename = "test"

declare i32 @putchar(i32)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 72, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %callputchar = call i32 @putchar(i32 %a1)
  store i32 85, i32* %a, align 4
  %a2 = load i32, i32* %a, align 4
  %callputchar3 = call i32 @putchar(i32 %a2)
  store i32 83, i32* %a, align 4
  %a4 = load i32, i32* %a, align 4
  %callputchar5 = call i32 @putchar(i32 %a4)
  store i32 84, i32* %a, align 4
  %a6 = load i32, i32* %a, align 4
  %callputchar7 = call i32 @putchar(i32 %a6)
  store i32 67, i32* %a, align 4
  %a8 = load i32, i32* %a, align 4
  %callputchar9 = call i32 @putchar(i32 %a8)
  store i32 83, i32* %a, align 4
  %a10 = load i32, i32* %a, align 4
  %callputchar11 = call i32 @putchar(i32 %a10)
  store i32 69, i32* %a, align 4
  %a12 = load i32, i32* %a, align 4
  %callputchar13 = call i32 @putchar(i32 %a12)
  ret i32 72
}
