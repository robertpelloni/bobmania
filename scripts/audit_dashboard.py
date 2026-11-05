#!/usr/bin/env python3
"""
Dashboard audit tool: inspects registered categories, routes, and menu items across
Themes/default/BGAnimations/ScreenUnifiedDashboard overlay.lua.
"""
import re
import sys

def audit_dashboard():
    dashboard_file = "Themes/default/BGAnimations/ScreenUnifiedDashboard overlay.lua"

    with open(dashboard_file, "r") as f:
        dash_content = f.read()

    # Pattern for flat choices extraction: { text = "...", screen = "...", tag = "...", help = "..." }
    pattern = re.compile(r'\{\s*text\s*=\s*"([^"]+)",\s*screen\s*=\s*"([^"]+)",\s*tag\s*=\s*"([^"]+)",\s*help\s*=\s*"([^"]+)"\s*\}')
    matches = pattern.findall(dash_content)

    print(f"Auditing ScreenUnifiedDashboard: Found {len(matches)} consolidated modules:")
    for text, screen, tag, help_txt in matches:
        print(f" {tag:<8} | {text:<24} -> {screen:<24} | {help_txt[:60]}...")

    expected_screens = {
        "ScreenSelectMusic",
        "ScreenGymWelcome",
        "ScreenTournamentLadder",
        "ScreenNetRoom",
        "ScreenMarketplace",
        "ScreenWalletHistory",
        "ScreenMissionSelect",
        "ScreenAssetSync",
        "ScreenSpectate",
        "ScreenReplayMenu",
        "ScreenContentNetwork",
        "ScreenUnifiedOptions",
        "ScreenExit"
    }

    found_screens = {screen for _, screen, _, _ in matches}
    missing = expected_screens - found_screens
    if missing:
        print(f"ERROR: Missing expected screens in dashboard: {missing}")
        sys.exit(1)
    else:
        print("SUCCESS: All 13 high-value modules and system endpoints are registered and mapped properly.")

if __name__ == "__main__":
    audit_dashboard()
