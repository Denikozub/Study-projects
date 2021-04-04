    .686
    .model flat
    .code 
        extern SYSTEM_$$_RANDOM$LONGINT$$LONGINT:near
        public sort
        public crossing_one
        public crossing_two
        public crossing_universal
        public crossing_odnor
        public mutation_change
        public mutation_swap
        public mutation_reverse

    .code
;list = record  osob: word +0
                ;quality: real +8, +12
                ;chance: integer +16
                ;next: ref_list +20
                ;prev: ref_list +24
        ;end    loop 48
        
;procedure sort( var head: ref_list )
sort proc
        push    ebp
        mov     ebp, esp
        sub     esp, 16                 ; temp1, temp2
        push    eax
        push    ebx
        push    ecx
        push    edx
        push    esi
        push    edi

        temp1   equ [ebp - 4]
        temp2   equ [ebp - 12]
        mov     ecx, [ebp + 8]          ; head
        mov     ecx, [ecx]
        mov     ebx, [ebx + 20]         ; i := head^.next
Loop_sort:
        cmp     ebx, 0                  ; i <> nil
        je      End_sort
        mov     edx, ebx                ; j := i
Loop_sort_2:
        cmp     edx, edi                ; j <> head
        je      Insert
        
        mov     edi, dword ptr [ebx + 8]                ; i^.quality part 1
        mov     esi, dword ptr [ebx + 12]               ; i^.quality part 2
        mov     dword ptr temp1, edi
        mov     dword ptr temp1 + 4, esi
        fld     qword ptr temp1
        mov     edi, dword ptr [[edx + 24] + 8]        ; j^.prev^.quality part 1
        mov     esi, dword ptr [[edx + 24] + 12]       ; j^.prev^.quality part 2
        mov     dword ptr temp2, edi
        mov     dword ptr temp2 + 4, esi
        fcomp   qword ptr temp2         ; compare
        fstsw   ax                      ; flags to ax
        sahf                            ; from al to EFLAGS
        ja      Insert
        ffree   st                      ; clear stack
        mov     edx, [edx + 24]         ; j := j^.prev
        jmp     Loop_sort_2
Insert:
        ffree   st                      ; clear stack
        ;//insert ebx(b) after eax(a)
        mov     eax, [edx + 24]         ; a := j^.prev
        cmp     eax, 0                  ; a = nil
        je      Next_check2
        mov     esi, [ebx + 20]         ; bn := b^.next
        mov     edi, [ebx + 24]         ; bp := b^.prev
        push    edi
        mov     edi, 0
        mov     [ebx + 24], edi         ; b^.prev := nil
        pop     edi
        mov     [ecx + 24], ebx         ; head^.prev := b
        mov     [ebx + 20], ecx         ; b^.next := head
        cmp     esi, 0                  ; bn <> nil
        je      Next_check1
        mov     [esi + 24], edi         ; bn^.prev := bp
Next_check1:
        mov     [edi + 20], esi         ; bp^.next := bn
        mov     ecx, [ecx + 24]         ; head := head^.prev
        jmp     Next_loop_sort
Next_check2:
        cmp     [eax + 20], ebx         ; a^.next = b
        je      Next_loop_sort
        push    [eax + 20]              ; an := a^.next
        mov     esi, [ebx + 20]         ; bn := b^.next
        mov     edi, [ebx + 24]         ; bp := b^.prev
        mov     [eax + 20], ebx         ; a^.next := b
        mov     [ebx + 24], eax         ; b^.prev := a
        mov     [ecx + 24], ebx         ; an^.prev := b
        pop     [ebx + 20]              ; b^.next := an
        cmp     esi, 0                  ; bn <> nil
        je      Next_check3
        mov     [esi + 24], edi         ; bn^.prev := bp
Next_check3:
        mov     [edi + 20], esi         ; bp^.next := bn
        
Next_loop_sort:
        mov     ebx, [ebx + 20]         ; i := i^.next
        jmp     Loop_sort
End_sort:
        pop     edi
        pop     esi
        pop     edx
        pop     ecx
        pop     ebx
        pop     eax
        mov     esp, ebp
        pop     ebp
        ret     4
sort endp

;eax := 2^eax
stepen_2 proc
        push    ebx
        push    ecx
        push    edx
        
        mov     ecx, eax
        sub     edx, edx
        mov     bx, 2
        mov     eax, 1
        jecxz   End_stepen_2
Loop_stepen_2:
        mul     bx
        Loop    Loop_stepen_2
        
End_stepen_2:
        pop     edx
        pop     ecx
        pop     ebx
        ret
stepen_2 endp

;function crossing_one( osob1, osob2: word ) : longword;
crossing_one proc
        push    ebp
        mov     ebp, esp
        sub     esp, 4                  ; t
        push    ebx
        push    ecx
        push    edx
        push    edi
        push    esi

        t       equ word ptr [ebp - 4]
        mov     bx, [ebp + 8]           ; osob2
        mov     cx, [ebp + 12]          ; osob1

        mov     ax, 15
        push    edx
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        pop     edx
        inc     ax                      ; n
        call    stepen_2
        dec     ax                      ; mask_y
        mov     dx, 65535
        sub     dx, ax                  ; mask_x
        mov     si, cx                  ; osob1
        and     si, dx
        mov     di, bx                  ; osob2
        and     di, ax
        add     di, si
        mov     t, di                   ; new_osob1
        mov     si, cx                  ; osob1
        and     si, ax
        mov     di, bx                  ; osob2
        and     di, dx
        add     di, si
        mov     t + 2, di               ; new_osob2
        mov     eax, dword ptr [ebp - 4]

        pop     esi
        pop     edi
        pop     edx
        pop     ecx
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     8
crossing_one endp

;function crossing_two( osob1, osob2: word ) : longword;
crossing_two proc
        push    ebp
        mov     ebp, esp
        sub     esp, 4                  ; t
        push    ebx
        push    ecx
        push    edx
        push    edi
        push    esi

        t       equ word ptr [ebp - 4]
        mov     bx, [ebp + 8]           ; osob2
        mov     cx, [ebp + 12]          ; osob1

        mov     ax, 14
        push    edx
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        pop     edx
        inc     ax
        mov     t, ax                   ; n2
        call    stepen_2
        mov     dx, ax                  ; stepen_2( n2 )
        mov     ax, 16
        sub     ax, t
        push    edx
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        pop     edx
        inc     ax
        add     ax, t                   ; n1
        call    stepen_2                ; stepen_2( n1 )
        sub     ax, dx                  ; mask_y
        mov     dx, 65535
        sub     dx, ax                  ; mask_x
        mov     si, cx                  ; osob1
        and     si, dx
        mov     di, bx                  ; osob2
        and     di, ax
        add     di, si
        mov     t, di                   ; new_osob1
        mov     si, cx                  ; osob1
        and     si, ax
        mov     di, bx                  ; osob2
        and     di, dx
        add     di, si
        mov     t + 2, di               ; new_osob2
        mov     eax, dword ptr [ebp - 4]

        pop     esi
        pop     edi
        pop     edx
        pop     ecx
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     8
crossing_two endp

;function crossing_universal( osob1, osob2: word;
;universal_chance: word ) : longword;
crossing_universal proc
        push    ebp
        mov     ebp, esp
        sub     esp, 4                  ; t
        push    ebx
        push    ecx
        push    edx
        push    edi
        push    esi

        t       equ word ptr [ebp - 4]
        mov     dx, [ebp + 8]
        mov     t, dx                   ; univ_c
        mov     bx, [ebp + 12]          ; osob2
        mov     cx, [ebp + 16]          ; osob1

        sub     edi, edi                ; new1
        sub     esi, esi                ; new2
        sub     edx, edx                ; i
Loop_universal:
        inc     edx
        mov     ax, 100
        push    edx
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        pop     edx
        inc     ax                      ; shot
        cmp     t, ax
        ja      Popadaniye_universal    ; est' popadaniye
        mov     ax, dx
        dec     ax
        call    stepen_2
        and     ax, bx                  ; osob2 and stepen_2( i - 1 )
        add     si, ax                  ; new2
        mov     ax, dx
        dec     ax
        call    stepen_2
        and     ax, cx                  ; osob1 and stepen_2( i - 1 )
        add     di, ax                  ; new1
        jmp     Next_loop_universal
Popadaniye_universal:
        mov     ax, dx
        dec     ax
        call    stepen_2
        and     ax, cx                  ; osob1 and stepen_2( i - 1 )
        add     si, ax                  ; new2
        mov     ax, dx
        dec     ax
        call    stepen_2
        and     ax, bx                  ; osob2 and stepen_2( i - 1 )
        add     di, ax                  ; new1
Next_loop_universal:
        cmp     edx, 16
        jb      Loop_universal
        
        mov     t, di                   ; new1
        mov     t + 2, si               ; new2
        mov     eax, dword ptr [ebp - 4]

        pop     esi
        pop     edi
        pop     edx
        pop     ecx
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     12
crossing_universal endp

;function crossing_odnor( osob1, osob2: word ): word;
crossing_odnor proc
        push    ebp
        mov     ebp, esp
        push    ebx
        push    ecx
        push    edx

        mov     bx, [ebp + 8]           ; osob2
        mov     cx, [ebp + 12]          ; osob1

        mov     eax, 65536
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        and     cx, ax                  ; osob1 and mask
        not     ax
        and     ax, bx                  ; osob2 and (not mask)
        add     ax, cx                  ; result

        pop     edx
        pop     ecx
        pop     ebx
        pop     ebp
        ret     8
crossing_odnor endp

;function mutation_change ( osob: word; change_chance: word ) : word;
mutation_change proc
        push    ebp
        mov     ebp, esp
        push    ebx
        push    ecx
        push    edx
        push    esi

        mov     bx, [ebp + 8]           ; change_c
        mov     cx, [ebp + 12]          ; osob

        sub     esi, esi                ; new_osob = 0
        sub     edx, edx                ; i = 0
Loop_change:
        inc     edx
        mov     ax, 100
        push    edx
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        pop     edx
        inc     ax                      ; shot
        cmp     bx, ax
        ja      Popadaniye_change       ; est' popadaniye
        mov     ax, dx
        dec     ax
        call    stepen_2
        and     ax, cx                  ; osob and stepen_2( i - 1 )
        add     si, ax                  ; new_osob
        jmp     Next_loop_change
Popadaniye_change:
        mov     ax, dx
        dec     ax
        call    stepen_2
        xor     ax, cx                  ; osob xor stepen_2( i - 1 )
        add     si, ax                  ; new_osob
Next_loop_change:        
        cmp     edx, 16
        jb      Loop_change
        
        mov     ax, si                  ; result

        pop     esi
        pop     edx
        pop     ecx
        pop     ebx
        pop     ebp
        ret     8
mutation_change endp

;function swap( ax, bh, bl ) : ax
;bh = a, bl = b
swap proc
        push    ecx
        push    edx
        push    esi
        
        sub     edx, edx                ; new_osob = 0
        sub     ecx, ecx                ; i = 0
        mov     si, ax                  ; osob
Loop_swap:
        inc     cl
        cmp     cl, bh                  ; if ( i = a )
        jne     Check_b
        sub     eax, eax
        mov     al, bl
        dec     al
        call    stepen_2
        and     ax, si                  ; osob and stepen_2( b - 1 )
        cmp     ax, 0
        je      Next_loop_swap
        sub     eax, eax
        mov     al, cl
        dec     al
        call    stepen_2
        add     dx, ax                ; new_osob := new_osob + stepen_2( i - 1 )
        jmp     Next_loop_swap
Check_b:
        cmp     cl, bl                  ; if ( i = b )
        jne     Not_equal
        sub     eax, eax
        mov     al, bh
        dec     al
        call    stepen_2
        and     ax, si                  ; osob and stepen_2( a - 1 )
        cmp     ax, 0
        je      Next_loop_swap
        sub     eax, eax
        mov     al, cl
        dec     al
        call    stepen_2
        add     dx, ax                ; new_osob := new_osob + stepen_2( i - 1 )
        jmp     Next_loop_swap
Not_equal:                              ; if ( i <> a ) and ( i <> b )
        sub     eax, eax
        mov     al, cl
        dec     al
        call    stepen_2
        and     ax, si                  ; osob and stepen_2( i - 1 )
        add     dx, ax
Next_loop_swap:
        cmp     ecx, 16
        jb      Loop_swap
        
        sub     eax, eax
        mov     ax, dx                  ; result
        
        pop     esi
        pop     edx
        pop     ecx
        ret
swap endp

;function mutation_swap( osob: word ) : word;
mutation_swap proc
        push    ebp
        mov     ebp, esp
        push    ebx
        push    ecx

        mov     cx, [ebp + 8]           ; osob
        
        mov     eax, 16
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        inc     eax
        mov     bh, al                  ; a
        mov     eax, 16
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        inc     eax
        mov     bl, al                  ; b
        mov     ax, cx
        call    swap                    ; swap a & b

        pop     ecx
        pop     ebx
        pop     ebp
        ret     4
mutation_swap endp

;function mutation_reverse( osob: word ) : word;
mutation_reverse proc
        push    ebp
        mov     ebp, esp
        push    ebx
        push    ecx

        mov     cx, [ebp + 8]           ; osob
        
        mov     eax, 15
        push    ecx
        call    SYSTEM_$$_RANDOM$LONGINT$$LONGINT
        pop     ecx
        add     al, 2
        mov     bh, al                  ; a = reverse point
        mov     bl, 100                 ; b = reversable point
        mov     ax, cx
Loop_reverse:
        cmp     bh, bl
        jbe     End_reverse
        call    swap
        dec     bh                      ; a := a - 1
        inc     bl                      ; b := b + 1
        jmp     Loop_reverse
End_reverse:
        pop     ecx
        pop     ebx
        pop     ebp
        ret     4
mutation_reverse endp

end