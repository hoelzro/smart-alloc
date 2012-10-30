  * We could coalesce adjacent free blocks
  * Create a standalone implementation of malloc() and friends using smart\_alloc
  * smart\_alloc\_info - Returns allocation information
  * smart\_alloc\_options - Where to get memory from, whether or not to zero/randomize memory post-free
  * What license should this be under?
  * Be able to give memory back if we can
  * Optionally run tests in a child process
   * Or in the middle of read-only memory so we can catch access violations
  * free\_list.size isn't that accurate; the available size is free\_list.size + sizeof(struct free\_list) - sizeof(struct alloc)
  * Add assertions that can be compiled out
   * The free list must never have a cycle
   * The last node of the free list must not extend outside of managed memory
   * Every free list node must be within managed memory
   * Every free list node's size must be greater than 0
