#include <pmap.h>

extern void do_tlb_refill_call(u_long non_used, u_long va, u_int entryhi);
extern void _do_tlb_refill(u_long *pentrylo, u_int va, u_int asid);

void tlb_refill_check(void) {
	struct Page *pp, *pp0, *pp1, *pp2, *pp3, *pp4;

	// should be able to allocate a page for directory
	assert(page_alloc(&pp) == 0);
	Pde *boot_pgdir = (Pde *)page2kva(pp);
	cur_pgdir = boot_pgdir;

	// should be able to allocate three pages
	pp0 = pp1 = pp2 = pp3 = pp4 = 0;
	assert(page_alloc(&pp0) == 0);
	assert(page_alloc(&pp1) == 0);
	assert(page_alloc(&pp2) == 0);
	assert(page_alloc(&pp3) == 0);
	assert(page_alloc(&pp4) == 0);

	// temporarily steal the rest of the free pages
	// now this page_free list must be empty!!!!
	LIST_INIT(&page_free_list);

	// free pp0 and try again: pp0 should be used for page table
	page_free(pp0);
	// check if PTE != PP
	assert(page_insert(boot_pgdir, 0, pp1, 0x0, 0) == 0);
	// should be able to map pp2 at PAGE_SIZE because pp0 is already allocated for page table
	assert(page_insert(boot_pgdir, 0, pp2, PAGE_SIZE, 0) == 0);

	printk("tlb_refill_check() begin!\n");

	Pte *walk_pte;
	u_long entrys[2];
	_do_tlb_refill(entrys, PAGE_SIZE, 0);
	assert(page_lookup(boot_pgdir, PAGE_SIZE, &walk_pte) != NULL);
	assert((entrys[0] == (*walk_pte >> 6)) + (entrys[1] == (*walk_pte >> 6)) == 1);
	assert(page2pa(pp2) == va2pa(boot_pgdir, PAGE_SIZE));

	printk("test point 1 ok\n");

	page_free(pp4);
	page_free(pp3);

	assert(page_lookup(boot_pgdir, 0x00400000, &walk_pte) == NULL);
	_do_tlb_refill(entrys, 0x00400000, 0);
	assert((pp = page_lookup(boot_pgdir, 0x00400000, &walk_pte)) != NULL);
	assert(va2pa(boot_pgdir, 0x00400000) == page2pa(pp3));

	printk("test point 2 ok\n");

	u_long badva, entryhi, entrylo;
	long index;
	badva = 0x00400000;
	entryhi = badva & 0xffffe000;
	asm volatile("mtc0 %0, $10" : : "r"(entryhi));
	do_tlb_refill_call(0, badva, entryhi);

	entrylo = 0;
	index = -1;
	badva = 0x00400000;
	entryhi = badva & 0xffffe000;
	asm volatile("mtc0 %0, $10" : : "r"(entryhi));
	asm volatile("mtc0 %0, $0" : : "r"(index));
	asm volatile("tlbp" : :);
	asm volatile("nop" : :);

	asm volatile("mfc0 %0, $0" : "=r"(index) :);
	assert(index >= 0);
	asm volatile("tlbr" : :);
	asm volatile("mfc0 %0, $2" : "=r"(entrylo) :);
	assert((entrylo == (entrys[0])) + (entrylo == (entrys[1])) == 1);

	printk("tlb_refill_check() succeed!\n");
}
void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	printk("init.c:\tmips_init() is called\n");

	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();

	tlb_refill_check();
	halt();
}
