## Compile errors

* Cannot convert from CHAR to LPCWSTR

![Char to LPCWSTR](images/char_to_lpcwstr.png)

The solutions is to change some parameters in project properties.

```
Project > Properties > Configuration Properties > General > Character Set > Use Multi-Byte Character Set
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
