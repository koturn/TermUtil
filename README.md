TermUtil
========

Terminal utilities for C-language.

This library enables to change foreground or background color, to move cursor
and others on terminal.

This library supports both MSVC and gcc.


## Build

#### gcc

```sh
$ make
```

#### MSVC

###### Release version

```sh
> nmake /f msvc.mk
```

###### Debug version

```sh
> nmake /f msvc.mk DEBUG=true
```

If you want to build with Dynalic-Link CRT Library, build as following.

```sh
> nmake /f msvc.mk CRTDLL=true
```


## LICENSE

This software is released under the MIT License, see [LICENSE](LICENSE).
