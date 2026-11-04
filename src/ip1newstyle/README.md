# Table of Contents
1. [Description](#description)
2. [Usage](#usage)

# Description

Utility used for converting the `ip1` field of each record to `newstyle`
from a given set of RPN standard files and copy them to one RPN standard
file (FSTD98).

# Usage

```bash
ip1newstyle -s srcfile [-d dstfile]
```

Examples of usage:

```bash
ip1newstyle -s input1
ip1newstyle -s input1 input2 input3 -d myoutput
```

**Definitions of keys in the command line**

| key                  | description                           |
| -------------------- | ------------------------------------- |
| -s infile            | one input file                        |
| -s filea fileb filec | multiple (up to 35) input files       |
| -d outfile           | name of the destination (output) file |

If the `-d` key is not defined or is missing, the name of the output
filename will be `ip1new_file_98`
