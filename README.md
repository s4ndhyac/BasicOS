# BasicOS

- Extended the open source Unix-like Operating System [xv-6](https://github.com/mit-pdos/xv6-public)
- Implemented unix system calls to:
  - get the memory dump [mtop]
  - support multi-threading [thread]
  - mutex locking [thread]
  - conditional variables [extracredit_cond_var]
  - semaphores [extracredit_semaphore]
  - fixed sbrk dynamic memory allocation function to support multi-threading
  - TODO: Fix malloc memory allocator to support multi-threading
  - linked list file-addressing [big]
