#!/usr/bin/env bash
set -euo pipefail

if [ "${BASH_VERSINFO:-0}" -lt 5 ]; then
  echo "[ERROR] This script must be used with bash 5+"
  exit 1
fi

readonly PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if ! command -v ansible-vault &> /dev/null; then
  echo "[ERROR] ansible-vault command is missing"
  exit
fi

if ! grep "[env]" "${PROJECT_DIR}/.env.ini" &> /dev/null; then
  echo "****************************"
  echo "[WARN] env file is encrypted"
  echo "****************************"
fi

function clean {
  pio run -t clean
}

function test {
  pio test
}

function dev {
	pio run -e dev
}

function deploy {
  pio run -e default
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
