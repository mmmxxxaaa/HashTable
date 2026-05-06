#!/bin/bash

PROGRAMS=("./hash_table_program_after_2")

CPU_CORE=2
MAX_FREQ="2.00GHz"
MIN_FREQ="2.00GHz"          
WARMUP=2
RUNS=7

run_benchmark() {
    local program_path="$1"
    local program_name=$(basename "$program_path")
    local result_md="assets/bench_${program_name}.md"
    local freq_log="logs/freq_${program_name}.csv"

    sudo turbostat --quiet --interval 1 --cpu $CPU_CORE --show "Bzy_MHz","CoreTmp" --out "$freq_log" &
    TURBO_PID=$!
    sleep 2

    hyperfine --warmup "$WARMUP" --runs "$RUNS" --export-markdown "$result_md" "taskset -c $CPU_CORE $program_path"

    sudo kill $TURBO_PID
    wait $TURBO_PID 2>/dev/null
    echo "Результаты сохранены в: $result_md, логи частоты: $freq_log"
}

sudo cpupower frequency-set --max "$MAX_FREQ" --min "$MIN_FREQ" > /dev/null
for gov in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor; do
    echo "performance" | sudo tee "$gov" > /dev/null
done
echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo > /dev/null

mkdir -p assets logs

for prog in "${PROGRAMS[@]}"; do
    if [[ -f "$prog" && -x "$prog" ]]; then
        run_benchmark "$prog"
    else
        echo "Файл '$prog' не найден или не является исполняемым. Пропускаем."
    fi
done

sudo cpupower frequency-set --min 400000 --max 4400000 > /dev/null
sudo cpupower frequency-set -g powersave > /dev/null
echo 0 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo > /dev/null
echo "Все бенчмарки завершены. CSV-логи лежат в папке logs/."