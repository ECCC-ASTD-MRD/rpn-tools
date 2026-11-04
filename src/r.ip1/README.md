# Table of Contents
1. [Description](#description)
2. [Usage](#usage)
3. [Examples](#note)

# Description

`r.ip1` prints on standard output the ip1code (old or new style) or the
level and type of level encoded in a given ip1code. You have an option of
adding a newline (end of line) at the end of the result.

# Usage

```bash
 Usage : r.ip1 [-nk] ipcode
 Result: value [level_type or kind]
 Usage : r.ip1 [-no] [--] value kind
 Result: ipcode(newstyle or oldstyle)
 Formats :
 options : -n to add end of line char
         : -k to get code for kind 
         : -o to get ipcode in oldstyle 
         : -- to prevent negative values from being parsed as options
 kind    : level_type
 0       : m  [metres] (height with respect to sea level)
 1       : sg [sigma] (0.0->1.0)
 2       : mb [mbars] (pressure in millibars)
 3       :    [others] (arbitrary code)
 4       : M  [metres] (height with respect to ground level)
 5       : hy [hybrid pressure] (0.0->1.0)
 6       : th [theta]
 7       : m- [metres] (height below sea level)
10       : H  [hours]
15       :    [reserved, integer]
17       :    [index X of conversion matrix]
21       : hh [hybrid height] (0.0->1000000.0)
```

# Examples

```bash
r.ip1 -n 1000
1000 mb

r.ip1 -nk 1000
1000 2

r.ip1 -o 1000.0 2
1000

r.ip1 -n -- -1.0 4
76507472
```
