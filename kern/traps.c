#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <trap.h>

extern void handle_int(void);
extern void handle_tlb(void);
extern void handle_sys(void);
extern void handle_mod(void);
extern void handle_reserved(void);
extern void handle_ri(void);

void (*exception_handlers[32])(void) = {
    [0 ... 31] = handle_reserved,
    [0] = handle_int,
    [2 ... 3] = handle_tlb,
    [10] = handle_ri,
#if !defined(LAB) || LAB >= 4
    [1] = handle_mod,
    [8] = handle_sys,
#endif
};

/* Overview:
 *   The fallback handler when an unknown exception code is encountered.
 *   'genex.S' wraps this function in 'handle_reserved'.
 */
void do_reserved(struct Trapframe *tf) {
	print_tf(tf);
	panic("Unknown ExcCode %2d", (tf->cp0_cause >> 2) & 0x1f);
}

void do_ri(struct Trapframe *tf) {
    u_int *epc;
    epc=tf->cp0_epc;
    u_int instr=*epc;
    int rs=(instr&0x3E00000);
    int rt=(instr&0x1F0000);
    int rd_index=(instr&0xF800);
    if ((instr&0xFC000000)==0) {
        if ((instr&63)==63) {//pmaxub
            u_int rs_value=tf->regs[rs];
            u_int rt_value=tf->regs[rt];
            u_int rd = 0;
            int i;
            for (i = 0; i < 32; i += 8) {
                u_int rs_i = rs_value & (0xff << i);
                u_int rt_i = rt_value & (0xff << i);
                if (rs_i < rt_i) {
                    rd = rd | rt_i;
                } else {
                    rd = rd | rs_i;
                }
            }
            tf->regs[rd_index]=rd;
        }
        else
        if ((instr&62)==62) {//cas
            u_int rs_value=tf->regs[rs];
            u_int rt_value=tf->regs[rt];
            u_int tmp = *(int *)rs_value;
            u_int rd = 0;
            if (*(int *)rs_value == rt_value) {
                *(int *)rs_value = rd;
            }
            rd = tmp;
            tf->regs[rd_index]=rd;
        }
    }
    tf->cp0_epc+=4;
}