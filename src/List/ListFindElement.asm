section .text

global ListFindElement

;----------------------------------------------------------------------------------------------
; Поиск элемента с заданной строкой в списке и возврат его позиции.
; Вход: rdi – указатель на структуру List (список) 
;       rsi – указатель на искомую строку (value) 
;       rdx – указатель на переменную int (position), куда будет записана позиция
; Выход: rax – код ошибки (0 – LIST_ERROR_NO, 5 – LIST_NULL_POINTER)
;        *position = -1 если элемент не найден, иначе индекс найденного узла
;
; Уничтожаемые регистры: rax, rcx, r8, r9, r10, r11 (каждый из них caller-saved)
;                        ymm0, ymm1, ymm2 
;----------------------------------------------------------------------------------------------
ListFindElement:
    test    rdi, rdi
    je      .L6
    test    rdx, rdx
    je      .L6

    mov     dword [rdx], -1
    cmp     qword [rdi+24], 0
    je      .L3

    mov     r9, qword [rdi]          ; r9 = array
    mov     rcx, qword [r9+8]        ; head
    test    rcx, rcx
    je      .L3

    vmovdqa ymm2, [rsi]                  ; !!!вынос pattern (одна загрузка)

    ; начальный переход (как у компилятора)
    jmp     .L5

.L4:
    ; переход к следующему элементу
    mov     rcx, qword [r8+8]        ; r8 уже указывает на элемент
    test    rcx, rcx
    je      .L3

.L5:
    ; вычисление адреса элемента
    lea     rax, [rcx+rcx*2]
    lea     r8, [r9+rax*8]           ; r8 = &element
    mov     rdi, qword [r8]          ; rdi = element->data

    ; --- именно такой порядок, как у компилятора ---
    vmovdqa ymm0, [rdi]              ; загрузка данных элемента ;!!!!!!!!!!!!разбил одну операцию на две
    vpcmpeqb ymm1, ymm0, ymm2        ; сравнение с pattern
    vpmovmskb eax, ymm1
    inc eax
    ;;;xor     eax, 0xffffffff ;все байты равны <=> маска 0xFFFFFFFF. При прибавлении 1 к 0xFFFFFFFF получается 0x00000000 (переполнение) и флаг нуля ZF=1
    ;;;test    eax, eax
    ;;;jne     .L4
    jnz     .L4

    ; найдено
    mov     dword [rdx], ecx
.L3:
    xor     eax, eax
    ret

.L6:
    mov     eax, 5
    ret