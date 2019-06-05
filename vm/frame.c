#include "vm/frame.h"
#include <stdio.h>
#include "threads/synch.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "userprog/pagedir.h"
#include "userprog/syscall.h"

/*
modified files:
    1. threads/init.c
    2. userprog/process.c
    3. userprog/pagedir.c

1. init.c - added 'include "vm/frame.h"' and 'frame_init()' in main function
2. process.c - load_segment(), setup_stack(), start_process(), process_exit()
3. pagedir.c - pagedir_destroy()
*/

static struct hash frametable;  // The Frame Table
static struct lock frame_lock;  // Lock

bool frame_less (const struct hash_elem *, const struct hash_elem *, void *);
unsigned frame_hash (const struct hash_elem *, void *);


/* Initialize frametable (this will be called in init.c) */
void frame_init()
{
    hash_init(&frametable, frame_hash, frame_less, NULL);
    lock_init(&frame_lock);
}

void frame_lock_frametable()
{
    lock_acquire(&frame_lock);
}
void frame_unlock_frametable()
{
    lock_release(&frame_lock);
}

/* return the frametable */
struct hash frame_get_frame(void)
{
    return frametable;
}

/* add page in the frametable */
void frame_addframe(void * upage, void *address)
{
    frame_lock_frametable();

    struct frame * new_frame = (struct frame*) malloc(sizeof(struct frame));
    new_frame->userpage = upage;
    new_frame->thread = thread_current();
    new_frame->address = address;
    printf("\naddframe - userpage: %p\n", upage);

    hash_insert(&frametable, &new_frame->hash_elem);    // add a frame to frametable
    frame_unlock_frametable();
}

void frame_printf(void)
{
    printf("\n============this is frame_printf===========\n");
    printf("hash size: %d\n", hash_size(&frametable));
}

/* Free an entry of the frametable */
void frame_free(void * address)
{
	struct frame * frame;
	struct hash_elem * found_frame;
	struct frame frame_elem;
	frame_elem.address = address;
    printf("\nframe free! address: %p\n", address);

	found_frame = hash_find(&frametable, &frame_elem.hash_elem);
	if(found_frame != NULL)
    {
		frame = hash_entry(found_frame, struct frame, hash_elem);
		palloc_free_page(frame->address);   // Free physical memory
		hash_delete(&frametable, &frame->hash_elem);    // Free the entry in the frametable
		free(frame);
        printf("\nframe free!\n");
	}
    else {printf("found_frame is null");}
}

// hash functions
bool frame_less(const struct hash_elem * frame1, const struct hash_elem * frame2, void * aux UNUSED)
{
	const struct frame * f1 = hash_entry(frame1, struct frame, hash_elem);
	const struct frame * f2 = hash_entry(frame2, struct frame, hash_elem);
	return f1->address < f2->address;
}
unsigned frame_hash(const struct hash_elem * frame1, void * aux UNUSED)
{
	const struct frame * frame = hash_entry(frame1, struct frame, hash_elem);
	return hash_int((unsigned)frame->address);
}