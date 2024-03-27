.text
.globl add_function
add_function:
    add $t1,$a1,$a2
    mv $v1 $t1
    jr $ra