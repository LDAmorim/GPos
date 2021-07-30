# GPos docs

The `GPos` code documentation is written in the *docs/src* folder and can be generated with `shpinx`.

The following packages are required to build it:
- sphinx
- sphinx-rtd-theme-common
- python-sphinxcontrib-programoutput
- graphviz
- git
- mercurial
- subversion

To compile and update the documentation, in the *docs* directory and run:

```bash
make clean
make html
```

Then point your preferred browser to *build/html/index.html*.

The developer documentation is available via `doxygen` and can be built in the *docs* directory, with `graphviz` pre-installed, by running:

```bash
doxygen Doxyfile
```

The main page generated can be found in *doxyhtml/html/index.html*.
