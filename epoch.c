#include <windows.h>
#include <time.h>
#include <tchar.h>

#define EPOCH1600_1970 11644473600ULL
#define _1E7ULL 10000000ULL

#define int64OfFT(ft) (((__int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime)
#define int64ToFT(i64, ft) { ft.dwLowDateTime = (i64 & 0xffffffff); ft.dwHighDateTime = i64 >> 32; }

__int64 TZDIFF = 0;
__int64 settzlapse(void) {
	FILETIME ftLocal, ftGlobal;
	SYSTEMTIME tmLocal, tmGlobal;
	TIME_ZONE_INFORMATION tz;
	__int64 iLoc, iUTC;

	GetSystemTime(&tmLocal);
	SystemTimeToFileTime(&tmLocal, &ftLocal);

	// copy data //
	tmGlobal.wYear = tmLocal.wYear;
	tmGlobal.wMonth = tmLocal.wMonth;
	tmGlobal.wDayOfWeek = tmLocal.wDayOfWeek;
	tmGlobal.wDay = tmLocal.wDay;
	tmGlobal.wHour = tmLocal.wHour;
	tmGlobal.wMinute = tmLocal.wMinute;
	tmGlobal.wSecond = tmLocal.wSecond;
	tmGlobal.wMilliseconds = tmLocal.wMilliseconds;

	GetTimeZoneInformation(&tz);
	TzSpecificLocalTimeToSystemTime(&tz, &tmLocal, &tmGlobal);

	SystemTimeToFileTime(&tmLocal, &ftLocal);
	SystemTimeToFileTime(&tmGlobal, &ftGlobal);

	iLoc = int64OfFT(ftLocal);
	iUTC = int64OfFT(ftGlobal);
	//printf("ftLocal:%lld, ftGlobal:%lld\n", i1, i2);

	TZDIFF = (iLoc - iUTC);
	return (iUTC - iLoc);
}

__int64 ATOI64(char *s) {
    __int64 ret = 0;
	int n, c = 0;

	while ((c = *s++) != '\0') {
		if (c >= '0' && c <= '9') n = (c - '0');
		else break;
		ret *= 10;
		ret += (__int64)n;
    }
    return ret;
}

#define OPT_UNIX    0x01
#define OPT_LOCAL   0x02
#define OPT_TSTAMP  0x04
#define OPT_TDEBUG  0x08
#define OPT_SPACE   0x10

TCHAR * _getBasename(TCHAR * arg0);
void Help(TCHAR *arg0) {
	printf("\n");
	printf("  Copyright(C) 2003-2017, PT SOFTINDO Jakarta\n");
	printf("  email: gm.dadang _at_ gmail.com\n");
	printf("  All rights reserved.\n");
	printf("\n");

  printf("  Timestamp to date converter\n");
  printf("  Version: 1.0.1.0, Build: 103, Date: 2020.03.08\n");
  printf("\n");
  printf("  Synopsys:\n");
  printf("    Convert the given number to datetime\n");
  printf("    datetime format: dd/mm/yyyy hh:mm:ss\n");
  printf("\n");
  printf("  Usage:\n");
  printf("    %s [ options ] timestamp...\n", _getBasename(arg0));
  printf("\n");
  printf("  Arguments:\n");
  printf("    timestamp (mandatory): positive numbers of seconds since epoch\n");
  printf("\t- numbers can be prefixed by 0x for hex and 0 for octal\n");
  printf("\t- negative numbers will be interpreted as unsigned value\n");
  printf("\t  (use the -d or -e switch to trace the number/value)\n");
  printf("\n");
  printf("    options/switches (optional), the last one given, took precedent:\n");
  printf("\tswitch can be prefixed by \"-\" (DASH) or \"/\" (SLASH)\n");
  printf("\tswitch also case insensitive, ie. \"-n\" is equal with \"/N\"\n");
  printf("\t-x\tUse UNIX epoch: seconds since 1970 (DEFAULT)\n");
  printf("\t-m\tUse MS/Excel epoch: seconds since 1600\n");
  printf("\t-l\tinterpret as local time (DEFAULT)\n");
  printf("\t-u\tinterpret as UTC/GMT time\n");
  printf("\t-s\"str\"\tuse this str as separator for multiple timestamps\n");
  printf("\t\t(default is using new-line as separator)\n");
  printf("\n");
  printf("\t-d\tshow the interpreted given numbers in the result (DEBUG)\n");
  printf("\t-e\talso show the original given numbers in the result (DEBUG)\n");
  printf("\t-n\tdon't print the original/interpreted values (DEFAULT)\n");
  printf("\n");
  printf("\t-?|?\tthis help.\n");
  printf("\t-!\tShow limits.\n");
  printf("\t--\tstop parsing the next remaining arguments as options\n");
  printf("\n");
  printf("  Examples:\n");
}

void printmaxdate(void) {
	FILETIME ftU, ftL;
	SYSTEMTIME tmU, tmL;
	__int64 u, l;
	int hours, minutes, seconds;

	u = l = (-1ULL>>1);///_1E7ULL*_1E7ULL;

	if (TZDIFF > 0) l -= TZDIFF; else u -= TZDIFF;

	int64ToFT(u, ftU)
	int64ToFT(l, ftL)

	FileTimeToSystemTime(&ftU, &tmU);
	FileTimeToSystemTime(&ftL, &tmL);

	SystemTimeToFileTime(&tmU, &ftU);
	SystemTimeToFileTime(&tmL, &ftL);

	u = int64OfFT(ftU);
	l = int64OfFT(ftL);
                        
    u /= 1e7;
    l /= 1e7;

	printf("\n");
	printf("  Max UTC datetime:\t%.02d/%.02d/%d %.02d:%.02d:%.02d.%.03d\n",
		tmU.wDay,tmU.wMonth,tmU.wYear, //tm.wDayOfWeek
		tmU.wHour,tmU.wMinute,tmU.wSecond,tmU.wMilliseconds
	);
	printf("  Max Local datetime:\t%.02d/%.02d/%d %.02d:%.02d:%.02d.%.03d\n",
		tmL.wDay,tmL.wMonth,tmL.wYear, //tm.wDayOfWeek
		tmL.wHour,tmL.wMinute,tmL.wSecond,tmL.wMilliseconds
	);

	seconds = TZDIFF/_1E7ULL;
	minutes = (seconds / 60) % 60;
	hours = (seconds / 3600) % 24;
	printf("  Timezone difference:\t%d seconds (%.02d:%.02d:%.02d)\n",
		seconds, hours, minutes, seconds % 60);

	printf("\n");
	printf("  switch: -l* (Local time)\n");
	printf("    -x* max timestamp:\t%lld\n", l-EPOCH1600_1970);
	printf("    -m  max timestamp:\t%lld\n", l);

	printf("\n");
	printf("  switch: -u (UTC time)\n");
	printf("    -x* max timestamp:\t%lld\n", u-EPOCH1600_1970);
	printf("    -m  max timestamp:\t%lld\n", u);
	printf("\n");
	printf("  NOTE:\n");
	printf("    *) DEFAULT switch if not given\n");
	printf("\n");
	printf("  CAUTION:\n");
	printf("    There's rounding error every 2 seconds for values\n");
	printf("    given near the MAX_TIMESTAMP (2 consecutive\n");
	printf("    timestamp numbers yield an identical datetime).\n");
	printf("    If the [second] accuracy is crucial, the numbers\n");
	printf("    given should be under the half of MAX_TIMESTAMP.\n");
}

void cvtodate(char * const s, int options) {
	__int64 i, tmp; // = ATOI64(s);
	FILETIME ft;
	SYSTEMTIME tm;
	i = _strtoi64(s, NULL, 0);
	if (i < 0) i &= 0xffffffff;

	tmp = i;

	if(options & OPT_UNIX) i += EPOCH1600_1970;

	i *= 1e7;

	// int64ToFT(i, ft)
	// FileTimeToSystemTime(&ft, &tm);
	// printf("%.02d/%.02d/%d %.02d:%.02d:%.02d",
	// 	tm.wDay,tm.wMonth,tm.wYear, //tm.wDayOfWeek
	// 	tm.wHour,tm.wMinute,tm.wSecond
	// );
	// printf(" UTC\n");

	if (options & OPT_LOCAL) i += TZDIFF;
	int64ToFT(i, ft)
	FileTimeToSystemTime(&ft, &tm);
	printf("%.02d/%.02d/%d %.02d:%.02d:%.02d",
		tm.wDay,tm.wMonth,tm.wYear, //tm.wDayOfWeek
		tm.wHour,tm.wMinute,tm.wSecond
	);

	if(options & OPT_TDEBUG) printf(" (%s = %llu)", s, tmp);
	else if(options & OPT_TSTAMP) printf(" (%llu)", tmp);
}

int optidx = 0, opterr = 0;
TCHAR *optarg;

int getoptn(int argc, TCHAR *argv[], TCHAR *optstring);

int main(int argn, char *args[]) {
	int i, options = OPT_UNIX | OPT_LOCAL;
	boolean opt_space = 0;
	TCHAR c, ch;
	__int64 L;
	char * separators[2] = { "\n\0", " \0" };
	char * sep = "\n";

	if (argn < 2) return Help(args[0]);
	if (*args[1] == _T('?')) return Help(args[0]);

	L = settzlapse();
	//printf("lapse:%lld, TZDIFF:%lld\n", L, TZDIFF);


	while ((c = getoptn(argn, args, _T("iemxuls:xmlus:den!XMULS:DEN!"))) != -1) {
		ch = args[optidx-1][1];
		switch (tolower(c)) {
			case _T('!'): return printmaxdate(); break;
			case _T('x'): options |= OPT_UNIX; break;
			case _T('m'): options ^= OPT_UNIX; break;
			case _T('l'): options |= OPT_LOCAL; break;
			case _T('u'): options ^= OPT_LOCAL; break;
			case _T('s'): sep = optarg; break;
			case _T('d'): options |= OPT_TSTAMP; break;
			case _T('e'): options |= OPT_TDEBUG; break;
			case _T('n'): options ^= (OPT_TSTAMP|OPT_TDEBUG); break;
			case _T('?'): if (ch == _T('?')) return Help(args[0]);
							else printf("Unknown switch: %c\n", opterr|0x20); break;
			//default: printf("unknown switch: %c\n", c); 
		}
	}
	if (argn > optidx) {
		opt_space = (options & OPT_SPACE) > 0;
		//printf("options: %X\n\n", options);
		for (i = optidx; i < argn; i++) {
			if (i > optidx) printf("%s", sep);
			//printf("%s: ", args[i]);
			cvtodate(args[i], options);
		}
	}
	else return Help(args[0]);
	return 0;
}


TCHAR * _getBasename(TCHAR * arg0) {
	TCHAR *p, *s = arg0;
	p = s;
	while((*p) && ((*p) != _T('\\'))) p++;
	while((*p)) {
		s = ++p;
		while ((*p) && ((*p) != _T('\\'))) p++;
	}
	return s;
}

int getoptn(int argc, TCHAR *argv[], TCHAR *optstring) {
	static TCHAR *next = NULL;
	TCHAR c, *cp;

	if (!optidx) next = NULL;
	optarg = NULL;

	if (!next || !*next) {
		if (!optidx) optidx++;

		#define OC_NEQ(c) (argv[optidx][0] != _T(c))
		if ((optidx >= argc) || (OC_NEQ('-') && OC_NEQ('/')) || !argv[optidx][1]) {
			optarg = NULL;
			if (optidx < argc) optarg = argv[optidx];
			return -1;
		}
		#undef OC_NEQ

		if (strcmp(argv[optidx], _T("--")) == 0) {
			optidx++;
			optarg = NULL;
			if (optidx < argc) optarg = argv[optidx];
			return -1;
		}

		next = argv[optidx++];
		next++;
	}

	c = *next++;
	cp = strchr(optstring, c);

	if (!cp || c == _T(':')) { opterr = c; return _T('?'); };

	cp++;
	if (*cp == _T(':')) {
		if (*next != _T('\0'))
			{ optarg = next; next = NULL; }
		else if (optidx < argc)
			optarg = argv[optidx++];
		else
			return _T('?');
	}

	return c;
}
