import subprocess
import os
import datetime

def get_submodule_info():
    submodules = []
    # Get list of submodules
    try:
        output = subprocess.check_output(['git', 'submodule', 'status', '--recursive'], text=True)
        lines = output.strip().split('\n')
        for line in lines:
            parts = line.split()
            if len(parts) >= 2:
                commit_hash = parts[0].replace('+', '').replace('-', '')
                
                # Try to find the path by joining parts until it exists
                path = ""
                version_info_start_index = 1
                for i in range(1, len(parts)):
                    potential_path = " ".join(parts[1:i+1])
                    if os.path.exists(potential_path):
                        path = potential_path
                        version_info_start_index = i + 1
                        break
                
                if not path:
                    # Fallback if not found (maybe deleted?)
                    path = parts[1]
                    version_info_start_index = 2

                version_info = " ".join(parts[version_info_start_index:]) if len(parts) > version_info_start_index else ""
                
                # Get more info from inside the submodule
                try:
                    cwd = os.getcwd()
                    sub_path = os.path.join(cwd, path)
                    if os.path.exists(sub_path):
                        os.chdir(sub_path)
                        # Get commit date
                        date_str = subprocess.check_output(['git', 'show', '-s', '--format=%cd', '--date=short', 'HEAD'], text=True).strip()
                        # Get commit subject
                        subject = subprocess.check_output(['git', 'show', '-s', '--format=%s', 'HEAD'], text=True).strip()
                        
                        submodules.append({
                            'path': path,
                            'commit': commit_hash,
                            'version_info': version_info,
                            'date': date_str,
                            'subject': subject
                        })
                        os.chdir(cwd)
                except Exception as e:
                    print(f"Error getting info for {path}: {e}")
                    if os.getcwd() != cwd:
                        os.chdir(cwd)
    except Exception as e:
        print(f"Error listing submodules: {e}")
    
    return submodules

def generate_dashboard(submodules):
    content = "# Project Dashboard\n\n"
    content += f"Last Updated: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n"
    
    content += "## Project Structure\n\n"
    content += "The project is organized as follows:\n\n"
    content += "- **src/**: Source code for the application.\n"
    content += "- **extern/**: External dependencies and libraries (submodules).\n"
    content += "- **Themes/**: Themes for the game (including submodules).\n"
    content += "- **Docs/**: Documentation.\n"
    content += "- **Build/**: Build scripts and instructions.\n\n"
    
    content += "## Submodules\n\n"
    content += "| Path | Version (Commit) | Date | Description/Tag |\n"
    content += "|------|------------------|------|-----------------|\n"
    
    for sub in submodules:
        content += f"| `{sub['path']}` | `{sub['commit'][:8]}` | {sub['date']} | {sub['version_info']} |\n"
    
    return content

if __name__ == "__main__":
    subs = get_submodule_info()
    dashboard = generate_dashboard(subs)
    print(dashboard)
