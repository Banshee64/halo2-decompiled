# decomp.dev progress report

[decomp.dev](https://decomp.dev) charts a decompilation from an
[objdiff](https://github.com/encounter/objdiff) progress report. Kirk's note
on [issue #1](https://github.com/kirklandsig/halo2-decompiled/issues/1#issuecomment-5960390856)
is that a listing would be welcome, and that we would have to export the
checker's results in that report format ourselves: the toolchain is MSVC with
link-time code generation building an Xbox XBE, so `objdiff-cli report
generate` does not apply.

The format is
[`objdiff-core/protos/report.proto`](https://github.com/encounter/objdiff/blob/main/objdiff-core/protos/report.proto)
(report version 2; see the
[decomp.dev guide](https://decomp.wiki/tools/decomp-dev)). decomp.dev reads a
JSON report uploaded as a GitHub Actions artifact named `<version>_report`
(for example `retail_report`). Byte counts (`total_code`, `matched_code`, …)
are JSON strings; function counts are JSON numbers; percents are JSON numbers.
Projects that cannot run objdiff synthesise the same `measures` object. Fire
Emblem 8 (JP) does that in
[`scripts/gen-report.py`](https://github.com/laqieer/fireemblem8j/blob/main/scripts/gen-report.py).

## What this repo exports

`tools/check.py` writes `build/report.json`, but that file is a map of retail
addresses (`status`, `size`, `first_difference`), not an objdiff report, and
producing it needs `orig/default.xbe`. The committed inventory already records
every function's address, size and status.

```
python tools/decomp_dev_export.py -o build/decomp.dev.json
```

That reads `config/functions.csv` only. Default `--scope game` is the README
headline (owner `game`). `--scope in-scope` is the checker's second line:
everything except `eh` and `third:`. `--scope all` is the whole CSV.

| CSV status | In the report |
| --- | --- |
| `matched` | Full size counts as matched code. Fuzzy match 100. |
| `near` | Counted in the total only. Fuzzy match 0. |
| `todo` | Counted in the total only. Fuzzy match 0. |

`near` means the checker saw at most two differing instructions. It does not
record how many bytes matched, so this export cannot fill objdiff's
`fuzzy_match_percent` honestly for a near function. Those functions stay at 0
until a live check can supply per-function match bytes.

Data is not compared. `total_data` and `matched_data` are `"0"`. objdiff treats
an empty data total as 100%. That is not a data match.

Units are source files (`src/*.cpp` when the CSV has one), otherwise the
function's 64 KB address range (`asm/0x1e0000`). Functions are named
`function_<va>`; the export does not read the CSV's `name` or `object`
columns. Categories are `with-source` and `no-source`. Scopes other than `game` also split by owner bucket (`game`,
`xdk`, `third`, `eh`, `other`).

If a `build/report.json` from `tools/check.py` is already on disk, overlay its
statuses (the CSV size is kept; the XBE is not opened):

```
python tools/decomp_dev_export.py -o build/decomp.dev.json \
    --check-report build/report.json
```

The tool always prints a stderr note that a live `python tools/check.py` pass
is still required before uploading, so the statuses are the ones just checked.

`build/` is gitignored. Do not commit the report.

## Uploading (not wired up)

decomp.dev only sees a report that the default branch publishes as an artifact.
A repo admin has to register the project at <https://decomp.dev/manage/new> and
install the [decomp.dev GitHub app](https://github.com/apps/decomp-dev). No
secret is required for the artifact itself. Suggested version slug: `retail`
(this repo's disc build; `config/files.json` calls it `default`), artifact
name `retail_report`.

This draft does not add a workflow. When a listing is wanted, a push to `main`
can run the exporter (it does not need the XBE or the XDK) and upload
`build/decomp.dev.json`:

```yaml
- run: python tools/decomp_dev_export.py -o build/decomp.dev.json
- uses: actions/upload-artifact@v4
  with:
    name: retail_report
    path: build/decomp.dev.json
```

Run `python tools/check.py` in that workflow first once the retail XBE is
available to CI, then pass `--check-report build/report.json`, so the listing
is not stuck on whatever statuses were last committed to the CSV. Until
`check.py` records a matched-byte count for `near` functions, the fuzzy
percent will keep equalling the full-match percent.
