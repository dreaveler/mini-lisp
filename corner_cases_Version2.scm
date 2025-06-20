; corner_cases.lisp
; 一些能暴露 mini-lisp 实现边界/bug/易错点的测试用例

; 1. 参数数量不符
(car)                    ; 应抛异常：参数数量不对
(cons 1)                 ; 应抛异常：参数数量不对
(+ 1)                    ; 若只实现多元加法会出错
(-)                      ; 零参数，可能崩溃
(=)                      ; 零参数，可能崩溃
(map +)                  ; 缺list

; 2. 参数类型错误
(car 1)                  ; 参数不是pair
(+ "hello" 2)            ; 字符串和数字混用
(eq? (lambda () 1) 1)    ; 过程和数字比较
(cons 1 "a")             ; cdr 不是pair或nil
(list 1 (lambda () 1))   ; 混合合法和非法类型
(map 1 '(1 2))           ; map的第一个参数不是过程

; 3. 特殊值和边界情况
(cdr '())                ; 空list求cdr
(car '())                ; 空list求car
(reduce + '())           ; reduce 空list
(length '())             ; 空list长度
(length 1)               ; 非list参数

; 4. 未定义变量与环境
xxx                      ; 未定义符号
(define a 1) (b)         ; 定义后调用未定义过程

; 5. 过程/内置函数混用或嵌套
((+ 1 2))                ; 尝试把结果当过程用
((lambda (x) x) 1 2)     ; 参数数量不符
(apply + 1 2 3)          ; apply参数数量

; 6. 字符串、布尔、nil相关
(display #t)             ; 显示布尔
(display nil)            ; 显示nil
(string? 1)              ; 类型检查
(eval 1)                 ; eval 非表达式

; 7. 数学运算特殊值
(/ 1 0)                  ; 除零
(modulo 1 0)             ; 模0
(remainder 1 0)
(expt 0 0)               ; 0 的 0 次幂

; 8. 特殊语法输入
(list)                   ; 空list
(map + '())              ; 空list map
(filter + '())           ; 空list filter

; 9. 嵌套与组合边界
(map (lambda (x) (car x)) '((1) (2) (3))) ; 每个元素是单元素list
(reduce + '(1))          ; reduce 单元素

; 10. 递归/极深嵌套
; (let loop ((n 10000)) (if (= n 0) 0 (loop (- n 1))))
; 注：如支持 let/递归时可测试极深递归的表现
