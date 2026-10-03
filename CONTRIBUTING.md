# Contributing to the Halo 2 decompilation

Thanks for helping. This is a matching decompilation: a function is done
when the original compiler turns our source back into the retail XBE's exact
bytes, and `tools/check.py` decides that automatically.

## What you need

- **Your own copy of Halo 2** for the original Xbox (the retail disc build;
  the README lists its SHA-256). Extract `default.xbe` with
  `tools/xiso_extract.py`. Never commit it, or any other game file.
- **XDK 5849**, the Xbox SDK the game was built with. It belongs to
  Microsoft and is not in this repository; never commit any part of it.
- Python 3 with `pip install -r requirements-dev.txt`. The build runs on
  Windows, and on Linux under Wine.

The README's [Build and check](README.md#build-and-check) section has the
setup steps.

## Claim before you start

Several people and automated lanes work at once, so we split the game by
address range or by source file:

1. Check the pinned
   [Active claims](https://github.com/kirklandsig/halo2-decompiled/issues/9)
   issue, which lists every range being worked on (contributors' and our
   own), and the open pull requests, each of which names its range.
2. Pick something unclaimed. A whole original source file (for example
   everything from one `.obj` in the symbol names) is a good unit. Small
   leaf functions are the easiest start; `python tools/ready.py` lists
   functions whose callees are already done.
3. Open a **draft pull request** early with the range in its description,
   for example "Retail range claimed: `0xd5990`–`0xd9fff` (`damage.obj`)".
   We keep our own work out of claimed ranges. If something you need is
   already claimed, say so in the pull request and we'll sort it out.

## Working

- Follow [docs/DECOMPILING.md](docs/DECOMPILING.md): the `// @retail 0x...`
  markers, per-file `// @flags`, stubs for callees that aren't decompiled
  yet, and the known compiler idioms.
- Run `python tools/check.py <va> ...` for your functions and a full
  `python tools/check.py` before you push. A function that matched before
  must still match.
- Near-misses are fine to commit with their markers; the checker re-tests
  them on every build, and they often match once their callers or callees
  land.
- You don't need to commit `config/functions.csv`; we regenerate its
  statuses when we merge.

## Pull requests

- Commit with your GitHub no-reply email address
  (`<id>+<username>@users.noreply.github.com`). Our publishing checks reject
  other addresses.
- Say in the description what matched and what is near, and list any change
  outside your own files (shared headers, stubs, other files' flags).
- Mark the pull request ready when you want it merged. We check it against
  current `main` and merge it with a merge commit, so your authorship stays
  in the history.

## Licence

By contributing you agree that your contribution is released under CC0 1.0,
like the rest of the repository (see [LICENSE](LICENSE)).
