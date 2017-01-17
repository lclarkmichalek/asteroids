#ifndef _MAIN_H
#define _MAIN_H

	#ifdef _MSC_VER // compiling with MSVC
		// substitutes taken from:
		// https://social.msdn.microsoft.com/Forums/vstudio/en-US/9356f151-51d2-412d-8889-d53230cc6ec9/porting-srand48-and-drand48-of-unix-like-platform-based-source?forum=vclanguage
		#define SRAND48() srand((long)time(NULL))
		#define DRAND48() ((double)rand() / RAND_MAX)
	#else // OSX, Linux
		#define SRAND48() srand48(time(NULL))
		#define DRAND48() drand48()
	#endif // _MSC_VER

#endif // _MAIN_H