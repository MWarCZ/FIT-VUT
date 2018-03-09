
#ifdef DEBUG
#define debug(str,...) fprintf(stderr, str, ##__VA_ARGS__);
#else
#define debug(...) 
#endif

