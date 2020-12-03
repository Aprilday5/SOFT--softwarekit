//内核中
#define DEBUG1
#ifdef DEBUG1
#define dbg_wxt(fmt, args...) printk(KERN_ALERT " DBG(%s, %s(), %d): " fmt, __FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define dbg_wxt(fmt, args...)
#endif

//应用
#define DEBUG1
#ifdef DEBUG1
#define dbg_wxt(fmt, args...) printf(" DBG(%s, %s(), %d): " fmt, __FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define dbg_wxt(fmt, args...)
#endif
