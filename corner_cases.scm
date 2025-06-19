; corner_cases_builtins.lisp
; 针对常见内置过程的边界/易错输入测试

; ==== arithmetics ====
(+)
(+ 1)
(+ 1 "2")        ; 非法参数类型
(+ 1 #t)         ; 非法参数类型
(+ 1 2 3 4 "a")  ; 多参数混合非法类型
(-)              ; 无参数
(- 5)            ; 一元减法合法性
(- 5 2 1 "a")    ; 多参数混合非法类型
(*)
(* 2 "x")
(* 1 2 3 #f)
(/)              ; 无参数
(/ 1)            ; 一元除法
(/ 1 0)          ; 除零
(/ 1 2 0)        ; 多参数含零

; ==== list operations ====
(car)                    ; 缺参数
(car 1)                  ; 非pair
(car '())                ; 空list
(cdr '())
(cdr 1)
(cons)                   ; 缺参数
(cons 1)                 ; 缺参数
(cons 1 2 3)             ; 多参数
(cons 1 (lambda ()))     ; cdr类型特殊
(list)
(list 1 2 (lambda (x) x))
(length)
(length 1)
(length '(1 2 . 3))      ; 非纯表list

; ==== boolean/compare ====
(not)                    ; 无参数
(not 1 2)                ; 多参数
(not #t)
(and)
(and #t #f 1 "hi")       ; 含非bool
(or)
(or #f #f)
(or #f 1 "hi")
(=)
(= 1)
(= 1 1 1 "a")
(< 1)
(< 1 2 1)
(< 1 2 "x")
(>)

; ==== map/filter/reduce ====
(map)
(map +)
(map 1 '(1 2 3))
(map + 1)
(map + '(1 2) '(a b))
(filter)
(filter 1 '(1 2))
(filter + 1)
(reduce)
(reduce +)
(reduce + '())
(reduce + '(1 2 3 "a"))

; ==== string ====
(string? 123)
(string? "hi" "ho")
(string-append)
(string-append "a")
(string-append "a" 1)
(display)
(display 1 2)
(display (lambda ()))

; ==== io/parse ====
(read)                    ; 多次连续输入
(write)
(write 1 2)
(eval)
(eval 1)
(eval '(+ "a" 2))
(eval (lambda ()))
(load)                   ; 不存在的文件
(load "not_exist.lisp")

; ==== procedure ====
(apply)
(apply +)
(apply + '(1 2) 3)
(apply 1 '(1 2))
(call/cc)                ; call with/without参数

; ==== misc/special forms ====
(define)
(define x)
(define x 1 2)
(lambda)
(lambda (x))
(lambda x x)
(if)
(if #t)
(if #t 1)
(if 1 2 3 4)

; ==== quote/unquote ====
(quote)
(quote 1 2)
(quasiquote)
(unquote)
(quasiquote (unquote 1 2))

; ==== edge case ====
((+ 1 2))
((lambda (x) x))
((lambda (x) x) 1 2)
("string" 1 2)            ; 字符串作为过程
