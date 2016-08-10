
// Global defines
#define NULL 0

// Global Macros

/* Returns a 'return' in case value is zero */
#define checkPtr(address) if(!address) { return;}

/* Returns 'true' if value is not zero */
#define isValidAddress(address) ((address) != NULL)