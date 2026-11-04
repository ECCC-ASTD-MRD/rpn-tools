# Table of Contents
1. [Description](#description)
2. [Usage](#usage)
3. [Examples](#note)

# Description

`r.date` prints on standard output the difference between 2 (CMC) timestamps
(in hours by default) or, if it is given one date and a negative/positive
offset (in hours by default), it will return the resulting date. If `r.date`
receives no arguments or incorrect arguments, it will give comments on how
to use it. You have an option of adding a newline (end of line) at the end
of the result.

# Usage

```bash
r.date [-nVSLMM] date1 [+ntime][SMDW]
 Result: YYYYMMDDHHMMSSHH or CMC date time stamp
 Examples: 
         r.date 2013041700 
         r.date 2013041700 +24
         r.date =-1274137515 +24S
         r.date 385856000 +1.5D
 Usage   : r.date [-nVSLpP] date1 date2
 Resultat: nhours (date1-date2)
 Example: r.date 2013041700  2013041600
 Formats Allowed:
        date1/date2: YYYYMMDD[HH][MM][SS][HH] or 9 digit CMC timestep
        ntime units: none:hour,S:sec,M:min,D:day,W:week
 options : 
        -n newline char
        -I ignore leap year
        -L output format YYYY MM DD HH MM SS
        -S output format CMC timestamp
        -V output format YYYYMMDDHHMMSS00
        -M  multiply result by 60
        -MM multiply result by 3600
        -p print hours mins secs with C format_string 
        -P print hours mins secs with format format HHHMMSS
        -f print dates with date format string (see date manual)
        -F print dates with date format string "%Y%m%d%H"
```

# Examples

* Difference between two given dates

```bash
r.date -n 19961225 199612161548

200.2
```

* Offset added to a given date with no newline character

```bash
r.date 19961225 -200

1996121616000000
```

* Difference of 20 seconds (answer in hours by default)

```bash
r.date 1998031316164000 1998031316162000

0.00555556
```
