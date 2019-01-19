# The Circa Library Set

## Table of Contents

[Introduction](#introduction)
[Source And Usage](#source_and_usage)
[Usage](#usage)

------

## Introduction

Circa is a library set for the C programming language that aims to fill in the
gaps the standard library left-- better data structures, portable versions of
intrinsic functions, etc.

------

## Source And Usage

Circa is hosted on GitHub [here](https://github.com/davidgarland/circa).

If you want to clone the source code locally and have Git installed on your
machine, you can do so using the following command:

```Bash
git clone https://github.com/davidgarland/circa
```

As for actually using Circa in a project, the recommended way of using Circa is
to bundle it directly into your project as a Git subtree. Assuming you want to
install Sol to a Git project, storing it in the path `./lib/circa`, you would do
the following:

```Bash
git remote add -f circa https://github.com/davidgarland/circa
git subtree add --prefix lib/circa circa master --squash
```

Then, updating in the future is pretty simple:

```Bash
git fetch circa master
git subtree pull --prefix lib/circa circa master --squash
```

Note that this requires the working directory to be fully up-to-date.
