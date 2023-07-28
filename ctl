#!/usr/bin/env bash
set -euo pipefail

if [ "${BASH_VERSINFO:-0}" -lt 5 ]; then
  echo "[ERROR] This script must be used with bash 5+"
  exit 1
fi

readonly PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

function install {
  pio pkg install
}

function dev {
	pio run -e dev
}

function deploy {
  pio run -e default
}

function clean {
  pio run -t clean
}

function vault {
	bash "${PROJECT_DIR}/.vault/vault_ctl" "$@"
}

function help {
  echo "Usage: "
  echo ""
  echo "  ./ctl <task> <args>"
  echo ""
  echo "Tasks:"
  echo ""
  compgen -A function | sort | grep -v -e "^_"
  echo ""
}

TIMEFORMAT="Task completed in %3lR"
time "${@:-help}"
