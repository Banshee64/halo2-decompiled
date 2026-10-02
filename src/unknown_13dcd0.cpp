#include "cseries.h"

// @flags /O2 /arch:SSE /Gr

typedef bool (__stdcall *t_compare_function)(const void *, const void *, const void *);
typedef long (__stdcall *t_bsearch_compare_function)(const void *, const void *, const void *);
typedef long (__stdcall *t_bsearch_4byte_compare_function)(long, long, const void *);

// @retail 0x13dcd0
void shortsort_elements(char *lo, char *hi, unsigned int element_size, bool swap_dwords, t_compare_function compare, const void *context)
{
    while (hi > lo)
    {
        char *max = lo;
        char *p = lo + element_size;
        while (p <= hi)
        {
            max = compare(p, max, context) ? p : max;
            p += element_size;
        }

        char *a = max;
        char *b = hi;
        if (swap_dwords)
        {
            int n = element_size >> 2;
            do
            {
                long t = *(long *)b;
                long u = *(long *)a;
                *(long *)a = t;
                a += 4;
                *(long *)b = u;
                b += 4;
                n--;
            } while (n > 0);
        }
        else
        {
            int n = element_size;
            do
            {
                char t = *b;
                char u = *a;
                *a = t;
                a++;
                *b = u;
                b++;
                n--;
            } while (n > 0);
        }

        hi -= element_size;
    }
}

// @retail 0x13dd70
long bsearch_4byte(long key, const long *base, long count, t_bsearch_4byte_compare_function compare, const long *context)
{
    const long *start = base;
    while (count != 0)
    {
        const long *middle = start + (count >> 1);
        long result = compare(key, *middle, context);
        if (result == 0)
        {
            return middle - base;
        }
        if (result > 0)
        {
            start = middle + 1;
            count--;
        }
        count >>= 1;
    }
    return -1;
}

// @retail 0x13ddd0
long bsearch_elements(const void *key, const void *base, long count, long element_size, t_bsearch_compare_function compare, const void *context)
{
    const char *start = (const char *)base;
    while (count != 0)
    {
        const char *middle = start + (count >> 1) * element_size;
        long result = compare(key, middle, context);
        if (result == 0)
        {
            return (middle - (const char *)base) / element_size;
        }
        if (result > 0)
        {
            start = middle + element_size;
            count--;
        }
        count >>= 1;
    }
    return -1;
}
