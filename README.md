```
|¯|     ___   ___       
| |__  / _ \ / _ \ _  __
| '_ \| | | | | | | |/ /
| | | | |_| | |_| |   < 
|_| |_|\___/ \___/|_|\_\

> What did you learn in school today?
Revenge.
```

## Description
**h00k** is a simple Windows c keylogger made using ```window.h```. It writes all keystrokes by application, on a log file in plain text.

[![forthebadge](https://forthebadge.com/images/badges/oooo-kill-em.svg)](https://forthebadge.com)

This is an example of a log:
![Char to LPCWSTR](images/example.png?raw=true)

## Compile errors

* Cannot convert from CHAR to LPCWSTR

![Char to LPCWSTR](images/char_2_lpcwstr.png?raw=true)

The solutions is to change some parameters in project properties.

```
Project > Properties > Configuration Properties > General > Character Set > Use Multi-Byte Character Set
```

* Cannot find or open PDB file

![Char to LPCWSTR](images/cannot_find_pdb.png?raw=true)

Solution: Add debugging symbols from Microsoft Server automatically ([Stackoverflow](https://stackoverflow.com/a/12954908))

```
Tools > Options > Debugging > Symbols > Microsoft Symbol Servers
```

* Linking errors

If you get error of unresolved link symbols, you might want to coompile it statically.

For Debug:
```
Project > Properties > Configuration Properties > C/C++ > Code Generation > Runtime Library > Multi-threaded Debug (/MTd)
```

For Release:
```
Project > Properties > Configuration Properties > C/C++ > Code Generation > Runtime Library > Multi-threaded (/MT)
```
