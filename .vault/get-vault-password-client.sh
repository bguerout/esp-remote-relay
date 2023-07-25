#!/usr/bin/env bash
set -euo pipefail

readonly VAULT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

gpg --batch --use-agent --decrypt "${VAULT_DIR}/vault_passphrase.gpg"
