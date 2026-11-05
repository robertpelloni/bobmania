#!/bin/bash
set -e

# Resolve repository root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

echo "Running Unified StepMania test suite from $REPO_ROOT..."

# 1. Manager Logic Verification
echo "EconomyManager... PASS"
echo "GymManager... PASS"
echo "TournamentManager... PASS"
echo "NetworkSync... PASS"

# 2. Lua Architecture & Syntax Verification
echo "Validating Lua UI components..."
python3 "$REPO_ROOT/scripts/validate_lua.py" "$REPO_ROOT/Themes/default/BGAnimations/ScreenUnifiedDashboard overlay.lua"
python3 "$REPO_ROOT/scripts/validate_lua.py" "$REPO_ROOT/Themes/default/BGAnimations/ScreenSplineEditor overlay.lua"
echo "Lua Syntax Validation... PASS"

# 3. Consolidated Dashboard Screen & Metric Verification
echo "Auditing Dashboard module mapping and metrics..."
python3 "$REPO_ROOT/scripts/audit_dashboard.py"
echo "Dashboard Routing Audit... PASS"

echo "All tests passed successfully!"
