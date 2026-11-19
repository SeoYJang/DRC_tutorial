#!/bin/bash
# Resubmit Condor jobs whose ROOT output is far smaller than a normal file.
#
# Usage:
#   ./resubmit_small.sh <results_dir> [--dry-run] [--fraction 0.5]
#
# <results_dir> is the campaign folder (the one that contains root/ and
# 1run_*.sh) or the root/ folder itself.
# A file is resubmitted when its size is below fraction * median size.
# The seed is the trailing integer in the file name, and that value is
# passed as the job argument so the same output path is overwritten.

set -euo pipefail

fraction=0.1
dry_run=0
results_dir=""

usage() {
    echo "Usage: $0 <results_dir> [--dry-run] [--fraction 0.5]" >&2
    exit 1
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --dry-run) dry_run=1; shift ;;
        --fraction)
            [[ $# -ge 2 ]] || usage
            fraction=$2
            shift 2
            ;;
        -h|--help) usage ;;
        *)
            [[ -z "$results_dir" ]] || usage
            results_dir=$1
            shift
            ;;
    esac
done

[[ -n "$results_dir" ]] || usage
[[ -d "$results_dir" ]] || { echo "No such directory: $results_dir" >&2; exit 1; }
results_dir=$(cd "$results_dir" && pwd)

if [[ $(basename "$results_dir") == "root" ]]; then
    root_dir=$results_dir
    results_dir=$(dirname "$results_dir")
else
    root_dir=$results_dir/root
fi
[[ -d "$root_dir" ]] || { echo "No root directory: $root_dir" >&2; exit 1; }

install_dir=$(cd "$(dirname "$0")" && pwd)
for need in "$install_dir/bin" "$install_dir/lib" "$install_dir/init.mac"; do
    [[ -e "$need" ]] || { echo "Missing $need (run this script from install/)" >&2; exit 1; }
done

shopt -s nullglob
runners=("$results_dir"/1run_*.sh)
[[ ${#runners[@]} -ge 1 ]] || { echo "No 1run_*.sh in $results_dir" >&2; exit 1; }
runner=${runners[0]}
mac=${runner%.sh}.mac
[[ -f "$mac" ]] || { echo "Missing macro: $mac" >&2; exit 1; }

mapfile -t sizes < <(find "$root_dir" -maxdepth 1 -name '*.root' -printf '%s\n' | sort -n)
[[ ${#sizes[@]} -gt 0 ]] || { echo "No ROOT files in $root_dir" >&2; exit 1; }
median=${sizes[$(( ${#sizes[@]} / 2 ))]}
threshold=$(awk -v m="$median" -v f="$fraction" 'BEGIN { printf "%d", m * f }')
[[ "$threshold" -gt 0 ]] || { echo "Median size is ${median} bytes; nothing to compare." >&2; exit 1; }

seeds=()
small_files=()
while IFS=$'\t' read -r bytes path; do
    if [[ "$bytes" -lt "$threshold" ]]; then
        base=$(basename "$path" .root)
        seed=${base##*_}
        [[ "$seed" =~ ^[0-9]+$ ]] || { echo "Cannot read seed from $path" >&2; exit 1; }
        seeds+=("$seed")
        small_files+=("$path")
    fi
done < <(find "$root_dir" -maxdepth 1 -name '*.root' -printf '%s\t%p\n' | sort -n)

echo "ROOT files : ${#sizes[@]}"
echo "Median     : ${median} bytes"
echo "Threshold  : ${threshold} bytes  (fraction ${fraction})"
echo "Resubmit   : ${#seeds[@]}"

if [[ ${#seeds[@]} -eq 0 ]]; then
    echo "No small files."
    exit 0
fi

mapfile -t seeds < <(printf '%s\n' "${seeds[@]}" | sort -n)
echo "Seeds      : ${seeds[*]}"

log_dir=$results_dir/log
mkdir -p "$log_dir"
sub_file=$results_dir/resubmit_small.sub
user_name=$(whoami)

{
    echo "universe = vanilla"
    echo "executable = $runner"
    echo "arguments = \$(seed)"
    echo "core_size = 0"
    echo "output = $log_dir/resub_\$(seed).out"
    echo "error = $log_dir/resub_\$(seed).err"
    echo "log = $log_dir/resub_\$(seed).log"
    echo "request_cpus = 1"
    echo "request_memory = 1.2 GB"
    echo "should_transfer_files = YES"
    echo "when_to_transfer_output = ON_EXIT"
    echo "transfer_input_files = $install_dir/bin, $install_dir/lib, $install_dir/init.mac, $mac"
    echo "accounting_group = group_fcc"
    echo "accounting_group_user = $user_name"
    echo "queue seed in ("
    printf '%s\n' "${seeds[@]}"
    echo ")"
} > "$sub_file"

echo "Submit file: $sub_file"

if [[ "$dry_run" -eq 1 ]]; then
    echo "Dry run: files left in place, nothing submitted."
    exit 0
fi

bad_dir=$root_dir/bad
mkdir -p "$bad_dir"
for path in "${small_files[@]}"; do
    mv -f "$path" "$bad_dir/"
done
echo "Moved ${#small_files[@]} small files to $bad_dir"

condor_submit "$sub_file"
