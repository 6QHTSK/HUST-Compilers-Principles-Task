 ; ModuleID = 'task1.c'
source_filename = "task1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 1, i32* %2, align 4
  ; print H
  store i32 72, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = call i32 @putchar(i32 %3)
  ; print U
  store i32 85, i32* %2, align 4
  %5 = load i32, i32* %2, align 4
  %6 = call i32 @putchar(i32 %5)
  ; print S
  store i32 83, i32* %2, align 4
  %7 = load i32, i32* %2, align 4
  %8 = call i32 @putchar(i32 %7)
  ; print T
  store i32 84, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  %10 = call i32 @putchar(i32 %9)
  ; print C
  store i32 67, i32* %2, align 4
  %11 = load i32, i32* %2, align 4
  %12 = call i32 @putchar(i32 %11)
  ; print S
  store i32 83, i32* %2, align 4
  %13 = load i32, i32* %2, align 4
  %14 = call i32 @putchar(i32 %13)
  ; print E
  store i32 69, i32* %2, align 4
  %15 = load i32, i32* %2, align 4
  %16 = call i32 @putchar(i32 %15)
  ;print \n
  store i32 10, i32* %2, align 4
  %17 = load i32, i32* %2, align 4
  %18 = call i32 @putchar(i32 %17)
  ret i32 0
}

declare dso_local i32 @putchar(i32) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1 "}
