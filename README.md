# epoch
Timestamp to date converter

    Copyright(C) 2003-2017, PT SOFTINDO Jakarta
    email: gm.dadang _at_ gmail.com
    All rights reserved.

    Timestamp to date converter
    Version: 1.0.1.0, Build: 103, Date: 2020.03.08

    Synopsys:
      Convert the given number to datetime
      datetime format: dd/mm/yyyy hh:mm:ss

    Usage:
      epoch [ options ] timestamp...

    Arguments:
      timestamp (mandatory): positive numbers of seconds since epoch
        - numbers can be prefixed by 0x for hex and 0 for octal
        - negative numbers will be interpreted as unsigned value
          (use the -d or -e switch to trace the number/value)

      options/switches (optional), the last one given, took precedent:
        switch can be prefixed by "-" (DASH) or "/" (SLASH)
        switch also case insensitive, ie. "-n" is equal with "/N"
        -x      Use UNIX epoch: seconds since 1970 (DEFAULT)
        -m      Use MS/Excel epoch: seconds since 1600
        -l      interpret as local time (DEFAULT)
        -u      interpret as UTC/GMT time
        -s"str" use this str as separator for multiple timestamps
                (default is using new-line as separator)

        -d      show the interpreted given numbers in the result (DEBUG)
        -e      also show the original given numbers in the result (DEBUG)
        -n      don't print the original/interpreted values (DEFAULT)

        -?|?    this help.
        -!      Show limits.
        --      stop parsing the next remaining arguments as options

Examples:

    epoch -!

    Max UTC datetime:     14/09/30828 02:48:05.477
    Max Local datetime:   13/09/30828 19:48:05.477
    Timezone difference:  25200 seconds (07:00:00)

    switch: -l* (Local time)
      -x* max timestamp:  910692704885
      -m  max timestamp:  922337178485

    switch: -u (UTC time)
      -x* max timestamp:  910692730085
      -m  max timestamp:  922337203685

    NOTE:
    *) DEFAULT switch if not given

    CAUTION:
      There's rounding error every 2 seconds for values
      given near the MAX_TIMESTAMP (2 consecutive
      timestamp numbers yield an identical datetime).
      If the [second] accuracy is crucial, the numbers
      given should be under the half of MAX_TIMESTAMP.
