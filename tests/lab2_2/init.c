void page_strong_check(void) {
	struct Page *pp, *pp0, *pp1, *pp2, *pp3, *pp4;
	struct Page_list fl;

	// should be able to allocate a page for directory
	assert(page_alloc(&pp) == 0);
	Pde *boot_pgdir = (Pde *)page2kva(pp);

	// should be able to allocate three pages
	pp0 = pp1 = pp2 = pp3 = pp4 = 0;
	assert(page_alloc(&pp0) == 0);
	assert(page_alloc(&pp1) == 0);
	assert(page_alloc(&pp2) == 0);
	assert(page_alloc(&pp3) == 0);
	assert(page_alloc(&pp4) == 0);

	assert(pp0);
	assert(pp1 && pp1 != pp0);
	assert(pp2 && pp2 != pp1 && pp2 != pp0);
	assert(pp3 && pp3 != pp2 && pp3 != pp1 && pp3 != pp0);
	assert(pp4 && pp4 != pp3 && pp4 != pp2 && pp4 != pp1 && pp4 != pp0);

	// temporarily steal the rest of the free pages
	fl = page_free_list;
	// now this page_free list must be empty!!!!
	LIST_INIT(&page_free_list);

	// there is no free memory, so we can't allocate a page table
	assert(page_insert(boot_pgdir, 0, pp1, 0x0, 0) < 0);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	// free pp0 and try again: pp0 should be used for page table
	page_free(pp0);
	// check if PTE != PP
	assert(page_insert(boot_pgdir, 0, pp1, 0x0, 0) == 0);
	// should be able to map pp2 at PAGE_SIZE because pp0 is already allocated for page table
	assert(page_insert(boot_pgdir, 0, pp2, PAGE_SIZE, 0) == 0);
	assert(page_insert(boot_pgdir, 0, pp3, 2 * PAGE_SIZE, 0) == 0);
	assert(PTE_ADDR(boot_pgdir[0]) == page2pa(pp0));

	printk("va2pa(boot_pgdir, 0x0) is %x\n", va2pa(boot_pgdir, 0x0));
	printk("page2pa(pp1) is %x\n", page2pa(pp1));

	assert(va2pa(boot_pgdir, 0x0) == page2pa(pp1));
	assert(pp1->pp_ref == 1);
	assert(va2pa(boot_pgdir, PAGE_SIZE) == page2pa(pp2));
	assert(pp2->pp_ref == 1);
	assert(va2pa(boot_pgdir, 2 * PAGE_SIZE) == page2pa(pp3));
	assert(pp3->pp_ref == 1);

	printk("start page_insert\n");
	// should be able to map pp2 at PAGE_SIZE because it's already there
	assert(page_insert(boot_pgdir, 0, pp2, PAGE_SIZE, 0) == 0);
	assert(va2pa(boot_pgdir, PAGE_SIZE) == page2pa(pp2));
	assert(pp2->pp_ref == 1);

	// should not be able to map at PDMAP because need free page for page table
	assert(page_insert(boot_pgdir, 0, pp0, PDMAP, 0) < 0);
	// remove pp1 try again
	page_remove(boot_pgdir, 0, 0x0);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(page_insert(boot_pgdir, 0, pp0, PDMAP, 0) == 0);

	// insert pp2 at 2*PAGE_SIZE (replacing pp2)
	assert(page_insert(boot_pgdir, 0, pp2, 2 * PAGE_SIZE, 0) == 0);

	// should have pp2 at both 0 and PAGE_SIZE, pp2 nowhere, ...
	assert(va2pa(boot_pgdir, PAGE_SIZE) == page2pa(pp2));
	assert(va2pa(boot_pgdir, 2 * PAGE_SIZE) == page2pa(pp2));
	// ... and ref counts should reflect this
	assert(pp2->pp_ref == 2);
	assert(pp3->pp_ref == 0);
	// try to insert PDMAP+PAGE_SIZE
	assert(page_insert(boot_pgdir, 0, pp2, PDMAP + PAGE_SIZE, 0) == 0);
	assert(pp2->pp_ref == 3);
	printk("end page_insert\n");

	// pp2 should be returned by page_alloc
	assert(page_alloc(&pp) == 0 && pp == pp3);

	// unmapping pp2 at PAGE_SIZE should keep pp1 at 2*PAGE_SIZE
	page_remove(boot_pgdir, 0, PAGE_SIZE);
	assert(va2pa(boot_pgdir, 2 * PAGE_SIZE) == page2pa(pp2));
	assert(pp2->pp_ref == 2);
	assert(pp3->pp_ref == 0);

	// unmapping pp2 at 2*PAGE_SIZE should keep pp2 at PDMAP+PAGE_SIZE
	page_remove(boot_pgdir, 0, 2 * PAGE_SIZE);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(va2pa(boot_pgdir, PAGE_SIZE) == ~0);
	assert(va2pa(boot_pgdir, 2 * PAGE_SIZE) == ~0);
	assert(pp2->pp_ref == 1);
	assert(pp3->pp_ref == 0);

	// unmapping pp2 at PDMAP+PAGE_SIZE should free it
	page_remove(boot_pgdir, 0, PDMAP + PAGE_SIZE);
	assert(va2pa(boot_pgdir, 0x0) == ~0);
	assert(va2pa(boot_pgdir, PAGE_SIZE) == ~0);
	assert(va2pa(boot_pgdir, 2 * PAGE_SIZE) == ~0);
	assert(va2pa(boot_pgdir, PDMAP + PAGE_SIZE) == ~0);
	assert(pp2->pp_ref == 0);

	// so it should be returned by page_alloc
	assert(page_alloc(&pp) == 0 && pp == pp2);

	// should be no free memory
	assert(page_alloc(&pp) == -E_NO_MEM);

	// forcibly take pp0 and pp1  back
	assert(PTE_ADDR(boot_pgdir[0]) == page2pa(pp0));
	assert(PTE_ADDR(boot_pgdir[1]) == page2pa(pp1));
	boot_pgdir[0] = 0;
	boot_pgdir[1] = 0;
	assert(pp0->pp_ref == 2);
	assert(pp1->pp_ref == 1);
	pp0->pp_ref = 0;
	pp1->pp_ref = 0;

	// give free list back
	page_free_list = fl;

	// free the pages we took
	page_free(pp0);
	page_free(pp1);
	page_free(pp2);
	page_free(pp3);
	page_free(pp4);
	page_free(pa2page(PADDR(boot_pgdir)));

	printk("page_check_strong() succeeded!\n");
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	printk("init.c:\tmips_init() is called\n");

	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();

	page_check();
	page_strong_check();
	halt();
}
