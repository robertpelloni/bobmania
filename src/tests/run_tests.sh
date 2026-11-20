#!/bin/bash
set -e

# Resolve repository root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

echo "Running Unified StepMania test suite from $REPO_ROOT..."

# 1. Manager Logic Verification
# HONESTY NOTE (2026-11-19 restore): there are no automated tests for the
# C++ managers in this repository. Previous versions of this script printed
# "PASS" for each manager without running anything. Those lines now say
# NOT TESTED, which is the truth until real tests exist.
echo "EconomyManager... NOT TESTED (no automated test exists in this repo)"
echo "GymManager... NOT TESTED (no automated test exists in this repo)"
echo "TournamentManager... NOT TESTED (no automated test exists in this repo)"
echo "NetworkSync... NOT TESTED (no automated test exists in this repo)"

# 2. Lua Architecture & Syntax Verification
echo "Validating Lua UI components..."
python3 "$REPO_ROOT/scripts/validate_lua.py" "$REPO_ROOT/Themes/default/BGAnimations/ScreenUnifiedDashboard overlay.lua"
python3 "$REPO_ROOT/scripts/validate_lua.py" "$REPO_ROOT/Themes/default/BGAnimations/ScreenSplineEditor overlay.lua"
echo "Lua Syntax Validation... PASS"

# 3. Consolidated Dashboard Screen & Metric Verification
echo "Auditing Dashboard module mapping and metrics..."
python3 "$REPO_ROOT/scripts/audit_dashboard.py"
echo "Dashboard Routing Audit... PASS"

echo "Real checks passed: Lua syntax validation, dashboard routing audit."
echo "NOT covered: all C++ manager logic, engine build. See HANDOFF.md."
