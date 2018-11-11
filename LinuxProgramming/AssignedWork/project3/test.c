#include "jctl.h"

int main(int argc, char *argv[])
{
	write_jnl(LOG_USER, "PROGRAM_NAME", "hELLO WorlD!");
	return 0;
}
