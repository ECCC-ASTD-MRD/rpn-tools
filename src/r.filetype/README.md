# Table of Contents
1. [Description](#description)
2. [Usage](#usage)
3. [Note](#note)

# Description

`r.filetype` reads the first few bytes of the file and tries to determine
the type.  Upon completion, `r.filetype` returns a status code and prints a
message corresponding to the file type it has recognized. If the file type
is unknown or the file is empty or non existent, a negative code will be
returned. Here is a list of the possible returned status code:
<br />

| value          | description                           |
| -------------- | ------------------------------------- |
| -3             | file does not exist                   |
| -2             | empty file                            |
| -1             | unknown file type                     |
| 1              | RPN standard random 1989              |
| 2              | RPN standard sequential 1989          |
| 3              | RPN standard sequential  FORTRAN 1989 |
| 4              | CCRN                                  |
| 5              | CCRN-RPN                              |
| 6              | BURP                                  |
| 7              | GRIB                                  |
| 8              | BUFR                                  |
| 9              | BLOK                                  |
| 10             | FORTRAN binary                        |
| 11             | COMPRESS                              |
| 12             | GIF89                                 |
| 13             | GIF87                                 |
| 14             | IRIS                                  |
| 15             | JPG                                   |
| 16             | KMW                                   |
| 17             | PBM                                   |
| 18             | PCL                                   |
| 19             | PCX                                   |
| 20             | PDSVICAR                              |
| 21             | PM                                    |
| 22             | PPM                                   |
| 23             | PS                                    |
| 24             | KMW_                                  |
| 25             | RRBX                                  |
| 26             | SUNRAS                                |
| 27             | TIFF                                  |
| 28             | UTAHRLE                               |
| 29             | XBM                                   |
| 30             | XWD                                   |
| 31             | ASCII                                 |
| 32             | BMP                                   |
| 33             | RPN standard random 98                |
| 34             | RPN standard sequential 98            |
| 35             | FICHIER NETCDF                        |
| 36             | FICHIER CMCARC v4                     |
| 37             | FICHIER CMCARC v5                     |
| 38             | FICHIER HDF5                          |
| 39             | RPN standard 24 with RSF backend      |
| 40             | Generic RSF                           |
| 41             | SQLite 3 database                     |

# Usage

```bash
r.filetype filename
```

# Note

If `r.filetype` does not recognize a particular file type, it does a system call to the UNIX `file` command.
