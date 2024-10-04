#ifndef __DL_H_
#define __DL_H_

#include <stdlib.h>
#include <string.h>

#define dl_define(name, type) typedef struct {	\
		size_t capacity;		\
		size_t count;			\
		type* items;			\
	} name

#define dl_create(cap, item_size, dl) do {				\
		(dl).count = 0;						\
		(dl).capacity = (cap);					\
		(dl).items = malloc((cap) * (item_size));		\
		memset((dl).items, 0, (dl).capacity * (item_size));	\
	} while(0) 

#define dl_check_cap(dl) do {						\
	if ((dl).count >= (dl).capacity) {				\
		(dl).capacity *= 2;					\
		(dl).items = realloc((dl).items, (dl).capacity * sizeof((dl).items[0])); \
	}								\
	} while(0)

#define dl_append(dl, item) do {			\
		dl_check_cap(dl);			\
		(dl).items[(dl).count] = (item);	\
		(dl).count += 1;			\
	} while(0)

#define dl_delete(dl) do {		\
		free((dl).items);	\
	} while(0)

#endif // __DL_H_
