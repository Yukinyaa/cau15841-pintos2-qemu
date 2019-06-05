#ifndef VM_FRAME_H
#define VM_FRAME_H

#include "threads/palloc.h"
#include <hash.h>

struct frame {
    void *userpage;
    struct thread *thread;
    void *address;
    struct hash_elem hash_elem;
};

void frame_init(void);
void frame_lock_frametable(void);
void frame_unlock_frametable(void);
void frame_addframe(void * upage, void *address);
struct hash frame_get_frame(void);
void frame_free(void * address);
struct frame * frame_find(void * address);

void frame_printf(void);
#endif