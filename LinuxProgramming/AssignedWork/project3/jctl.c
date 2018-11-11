#include "jctl.h"

void write_jnl(int prior, char * name, char * msg)
{
	syslog(prior, "This is a msg from program %s : %s", name, msg);
}
