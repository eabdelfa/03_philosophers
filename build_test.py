import subprocess
import threading
import os
import argparse

# Define the directories
PHILO_DIR = os.path.join(os.getcwd(), "philo")
PHILO_BONUS_DIR = os.path.join(os.getcwd(), "philo_bonus")

def run_make_commands(directory, name, do_make, do_fclean):
    try:
        if do_make:
            print(f"[{name}] Starting 'make' in {directory}...")
            subprocess.run(["make"], cwd=directory, check=True)
            print(f"[{name}] 'make' completed successfully.")
        
        if do_fclean:
            print(f"[{name}] Starting 'make fclean' in {directory}...")
            subprocess.run(["make", "fclean"], cwd=directory, check=True)
            print(f"[{name}] 'make fclean' completed successfully.")
        
    except subprocess.CalledProcessError as e:
        print(f"[{name}] Error executing command: {e}")
    except Exception as e:
        print(f"[{name}] Unexpected error: {e}")

def main():
    parser = argparse.ArgumentParser(description="Run make and make fclean in philo and philo_bonus directories simultaneously.")
    parser.add_argument("--make", action="store_true", help="Run 'make'")
    parser.add_argument("--fclean", action="store_true", help="Run 'make fclean'")
    args = parser.parse_args()

    do_make = args.make
    do_fclean = args.fclean

    # Default behavior: if no flags provided, run both
    if not do_make and not do_fclean:
        do_make = True
        do_fclean = True

    # Create threads for each folder
    t1 = threading.Thread(target=run_make_commands, args=(PHILO_DIR, "Mandatory", do_make, do_fclean))
    t2 = threading.Thread(target=run_make_commands, args=(PHILO_BONUS_DIR, "Bonus", do_make, do_fclean))

    # Start threads
    t1.start()
    t2.start()

    # Wait for both to finish
    t1.join()
    t2.join()

    print("All tasks finished.")

if __name__ == "__main__":
    main()
