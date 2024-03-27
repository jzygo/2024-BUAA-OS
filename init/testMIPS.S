#include<mmu.h>
#inlcude<asm/asm.h>

.text
EXPORT(testMIPS)
.set at
.set recorder
lw t1,0(sp)
lw t2,4(sp)
add v0,t1,t2
jr ra
