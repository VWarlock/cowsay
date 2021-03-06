#include "opendir.h"
#include "string.h"

VOID	_ffirst(pattern, fib, regs)
char	*pattern;
FIB	*fib;
XREG	*regs;
{
	regs->bc = SEARCH_MASK | (unsigned)_FFIRST;
	regs->de = (unsigned)pattern;
	regs->ix = (unsigned)fib;
	callxx(BDOS, regs);
}

VOID	_fnext(fib, regs)
FIB	*fib;
XREG	*regs;
{
	regs->bc = SEARCH_MASK | (unsigned)_FNEXT;
	regs->ix = (unsigned)fib;
	callxx(BDOS, regs);
}

VOID	opendir(pattern)
char	*pattern;
{
	FIB		fib;
	BOOL		done;
	XREG		regs;
	int		i;

	i = 0;	/* Just in case */

	/* Make sure the whole FIB is set to 0			*/
	memset(&fib, (char)0, sizeof(FIB));

	/* Search for first entry */
	_ffirst(pattern, &fib, &regs);


	if (!(regs.af & 0xff00)) {
		/* Found at least one matching file 	*/
		printf("%s ", substr(fib.name,0, strpos(fib.name, ".")));
		i++;

		/* Look for more matches		*/
		done = FALSE;

		while(!done) {
			_fnext(&fib, &regs);
			if (regs.af & 0xff00) {
				done = TRUE;
			} else {
				printf("%s ", substr(fib.name,0, strpos(fib.name, ".")));
				i++;
			}
		}
	} else {
		puts("No .cow files found!");
	}
	puts("\n");
}
                                                                                                              
