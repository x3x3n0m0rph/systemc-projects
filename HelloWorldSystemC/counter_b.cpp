
#include "systemc.h"
#include "counter_b.h"

void counter_b::process_clock()
{
    if (reset.read())
    {
        count = 0;
        out.write(count);
    }
    else if (input.read())
    {
        count += 1;
        out.write(count);
    }
}