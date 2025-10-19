# OnChange

## Overview

Small program that triggers a command on a modification of any watched file.

## Usage

### Command Line Syntax

```sh
./onchange [options] [files...] -- [command]
```

### Example
```sh
./onchange file1.txt file2.txt -- echo "Files changed"
```
