import subprocess
import os
import json

CONFIG_FILE = "chif_config.json"
first_dev_commands = True
is_in_cd = False
# Helper function to run shell commands
def run_command(command):
    try:
        result = subprocess.run(command, check=True, shell=True, capture_output=True, text=True)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        print(e.stderr)

# Check if Git is installed
def check_git_installed():
    try:
        subprocess.run(["git", "--version"], check=True, capture_output=True, text=True)
        print("Git is already installed.")
    except subprocess.CalledProcessError:
        print("Git is not installed. Installing Git now...")
        install_git()
def is_git_installed():
    try:
        subprocess.run(["git", "--version"], check=True, capture_output=True, text=True)
        run_command("cd CHIF-Engine")
    except subprocess.CalledProcessError:
        print("Git is not installed. Installing Git now...")
        install_git()

# Install Git based on the OS
def install_git():
    if os.name == 'nt':  # Windows
        print("Please install Git manually from https://git-scm.com/download/win.")
    elif os.name == 'posix':  # macOS/Linux
        if "darwin" in os.uname().sysname.lower():
            run_command("brew install git")  # macOS with Homebrew
        else:
            run_command("sudo apt-get update && sudo apt-get install git")  # Linux
    else:
        print("OS not recognized. Install Git manually.")
def setup_streams():
    run_command('git remote rename origin upstream')
    run_command('git remote -v')
    run_command('git remote add origin <fork>')
    run_command('git remote -v')

# Clone a repository based on the user input
def clone_repo():
    fork_url = input("Enter your fork repository URL: ")
    if not fork_url:
        print("Invalid URL. Please try again.")
        return
    
    repo_name = fork_url.split('/')[-1].replace('.git', '')
    if os.path.exists(repo_name):
        print(f"The repository '{repo_name}' already exists.")
    else:
        run_command(f"git clone {fork_url}")
        save_config({"fork_url": fork_url, "repo_name": repo_name})
        print(f"Repository '{repo_name}' cloned successfully!")
        is_in_cd = True
        run_command('cd CHIF-Engine')
        setup_streams()

# Save user config to a JSON file
def save_config(config):
    with open(CONFIG_FILE, 'w') as file:
        json.dump(config, file)
    print("Configuration saved.")

# Load user config from the JSON file
def load_config():
    if os.path.exists(CONFIG_FILE):
        with open(CONFIG_FILE, 'r') as file:
            return json.load(file)
    return {}

# Run a few basic Git commands (like pull, status, etc.)
def run_dev_commands():
    config = load_config()
    if "repo_name" not in config:
        print("No repository found. Please clone your repository first.")
        return
    if is_in_cd == False:
        run_command("cd CHIF-Engine")
        is_in_cd = True
    repo_name = config['repo_name']
    os.chdir(repo_name)

    while True:
        print("\nAvailable commands:")
        print("1. Show status")
        print("2. Update Code")
        print("3. List all files")
        print("4. Back to main menu")
        print("5. Upload current code")
        print("6. Back to main menu")
        
        choice = input("Enter your command choice: ")

        if choice == '1':
            run_command("git status")
        elif choice == '2':
            run_command("git pull")
        elif choice == '3':
            if os.name == 'nt':
                run_command("dir")
            else:
                run_command("ls")
        elif choice == '4':
            break
        elif choice == '5':
            message = input("Enter your Commit Message: ")
            run_command('git add .')
            run_command(f'git commit -m "{message}"')
            run_command('git push origin main')
            print("Upload to CHIFEngine Succeded")
        elif choice == '6':
            break
        else:
            print("Invalid choice. Try again.")

# Main menu
def main_menu():
    first_command = True
    while True:
        if first_command == True:
            is_git_installed()
            print("\n=== Welcome to CHIF Dev Console ===")
            print("1. Check Git installation")
            print("2. Clone a forked repository")
            print("3. Run developer commands")
            print("4. Exit")

        choice = input("CHIFEngine >>> ")
        first_command = False
        if choice == '1':
            check_git_installed()
        elif choice == '2':
            clone_repo()
        elif choice == '3':
            run_dev_commands()
        elif choice == '4':
            print("Exiting CHIF Dev Console. Goodbye!")
            break
        else:
            print("Invalid choice. Try again.")

if __name__ == "__main__":
    main_menu()
