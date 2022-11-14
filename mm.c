#include <stdio.h>
#include <memory.h>
#include <unistd.h>  /*For getpage size*/
#include <sys/mman.h>  /*For using mmap()*/

static size_t SYSTEM_PAGE_SIZE = 0;

// Getting 1 virtual memory page from system
void mm_init() 
{
    SYSTEM_PAGE_SIZE = getpagesize();
}

// Function to request VM page from kernal
/*
** Parameters for mmap() 
** 1st is null
** 2nd the amount of memory we want to request from the kernal
** 3rd is specifying permissions on virtual memory pages
*/
static void * mm_get_new_vm_page_from_kernal(int units) 
{
    char *vm_page = mmap(
        0, 
        units * SYSTEM_PAGE_SIZE, 
        PROT_READ|PROT_WRITE|PROT_EXEC, 
        MAP_ANON|MAP_PRIVATE,
        0, 0);
    
    if(vm_page == MAP_FAILED) 
    {
        printf("ERROR : VM PAGE ALLOCATION FAILED\n");
        return NULL;
    }
    memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
    return (void *)vm_page;
}

// Function to return a page to kernal
static void mm_return_vm_page_to_kernal (void *vm_page, int units) 
{
    if(munmap(vm_page, units * SYSTEM_PAGE_SIZE))
        printf("ERROR : COULD NOT MUNMAP VM PAGE TO KERNAL \n");

}

int main(int argc, char **argv) 
{
    mm_init();
    printf("VM Page size = %lu\n", SYSTEM_PAGE_SIZE);
    void *addr1 = mm_get_new_vm_page_from_kernal(1);
    void *addr2 = mm_get_new_vm_page_from_kernal(1);
    printf("Page 1 = %p, Page 2 = %p\n", addr1, addr2);
    return 0;
}