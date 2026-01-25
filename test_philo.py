#!/usr/bin/env python3
"""
Comprehensive test suite for Philo and Philo_Bonus
Tests mandatory and bonus implementations with various parameter sets.
"""

import subprocess
import sys
import os
from pathlib import Path
from dataclasses import dataclass
from typing import List, Tuple, Optional


@dataclass
class TestCase:
	"""Represents a single test case"""
	name: str
	args: List[str]
	timeout: int
	expected_death: bool
	expected_death_time: Optional[Tuple[int, int]] = None  # (min, max) in ms
	description: str = ""


class PhiloTester:
	"""Main test runner for Philo binaries"""

	def __init__(self, binary_path: str):
		self.binary_path = binary_path
		self.binary_name = Path(binary_path).name
		self.test_results: List[Tuple[str, bool, str]] = []

	def run_test(self, test: TestCase) -> Tuple[bool, str]:
		"""
		Run a single test case.
		Returns: (passed, output)
		"""
		cmd = [self.binary_path] + test.args
		try:
			result = subprocess.run(
				cmd,
				capture_output=True,
				timeout=test.timeout,
				text=True
			)
			output = result.stdout
		except subprocess.TimeoutExpired:
			output = "TIMEOUT"
		except Exception as e:
			return False, f"ERROR: {str(e)}"

		# Analyze output
		passed, reason = self._analyze_output(output, test)
		return passed, reason

	def _analyze_output(self, output: str, test: TestCase) -> Tuple[bool, str]:
		"""Analyze test output against expectations"""
		
		lines = output.strip().split('\n') if output and output != "TIMEOUT" else []
		has_death = any(" died" in line for line in lines)

		if output == "TIMEOUT":
			if test.expected_death:
				return False, f"TIMEOUT but death was expected"
			return True, "OK (timeout as expected, no death)"

		if test.expected_death:
			if not has_death:
				return False, f"Expected death but none occurred"
			# Check death timing if specified
			if test.expected_death_time:
				death_line = next((l for l in lines if " died" in l), None)
				if death_line:
					try:
						death_time = int(death_line.split()[0])
						min_time, max_time = test.expected_death_time
						if not (min_time <= death_time <= max_time):
							return False, f"Death at {death_time}ms, expected {min_time}-{max_time}ms"
						return True, f"OK (death at {death_time}ms)"
					except (ValueError, IndexError):
						return False, "Could not parse death time"
			return True, "OK (death occurred as expected)"
		else:
			if has_death:
				death_line = next((l for l in lines if " died" in l), None)
				return False, f"Unexpected death: {death_line}"
			return True, "OK (no death as expected)"

	def run_tests(self, tests: List[TestCase]) -> bool:
		"""Run all tests and return True if all passed"""
		print(f"\n{'='*70}")
		print(f"Testing: {self.binary_name}")
		print(f"Binary: {self.binary_path}")
		print(f"{'='*70}\n")

		all_passed = True
		for i, test in enumerate(tests, 1):
			passed, reason = self.run_test(test)
			status = "✓ PASS" if passed else "✗ FAIL"
			print(f"[{i}/{len(tests)}] {status}: {test.name}")
			print(f"      Args: {' '.join(test.args)}")
			print(f"      {reason}")
			if test.description:
				print(f"      Description: {test.description}")
			print()
			
			self.test_results.append((test.name, passed, reason))
			all_passed = all_passed and passed

		return all_passed

	def print_summary(self):
		"""Print test summary"""
		total = len(self.test_results)
		passed = sum(1 for _, p, _ in self.test_results if p)
		print(f"\n{'='*70}")
		print(f"Summary for {self.binary_name}: {passed}/{total} tests passed")
		print(f"{'='*70}\n")


def get_mandatory_tests() -> List[TestCase]:
	"""Get all mandatory part (philo) tests"""
	return [
		TestCase(
			name="Single Philosopher (should die)",
			args=["1", "800", "200", "200"],
			timeout=5,
			expected_death=True,
			expected_death_time=(790, 810),
			description="Single philosopher takes one fork and dies at t_die"
		),
		TestCase(
			name="Two Philosophers (no death)",
			args=["2", "400", "100", "100"],
			timeout=5,
			expected_death=False,
			description="Two philosophers alternate eating, no starvation"
		),
		TestCase(
			name="Two Philosophers (tight timing death)",
			args=["2", "200", "100", "100"],
			timeout=5,
			expected_death=False,
			description="Two philosophers alternate eating without starvation at these timings"
		),
		TestCase(
			name="Four Philosophers (one dies at 310ms)",
			args=["4", "310", "200", "100"],
			timeout=5,
			expected_death=True,
			expected_death_time=(300, 320),
			description="One philosopher dies at ~310ms"
		),
		TestCase(
			name="Four Philosophers (no death at 410ms)",
			args=["4", "410", "200", "200"],
			timeout=5,
			expected_death=False,
			description="Adequate time prevents starvation"
		),
		TestCase(
			name="Five Philosophers (no death)",
			args=["5", "800", "200", "200"],
			timeout=5,
			expected_death=False,
			description="Odd number philosophers with sufficient time"
		),
		TestCase(
			name="Five Philosophers with must_eat=7",
			args=["5", "800", "200", "200", "7"],
			timeout=10,
			expected_death=False,
			description="Simulation stops after each philosopher eats 7 times"
		),
		TestCase(
			name="Large Scale (200 philosophers)",
			args=["200", "800", "200", "200"],
			timeout=30,
			expected_death=False,
			description="Stress test with 200 philosophers"
		),
	]


def get_bonus_tests() -> List[TestCase]:
	"""Get all bonus part (philo_bonus) tests"""
	return [
		TestCase(
			name="Single Philosopher (should die)",
			args=["1", "800", "200", "200"],
			timeout=5,
			expected_death=True,
			expected_death_time=(790, 810),
			description="Single philosopher in separate process takes one fork and dies"
		),
		TestCase(
			name="Two Philosophers (no death)",
			args=["2", "400", "100", "100"],
			timeout=5,
			expected_death=False,
			description="Two philosopher processes alternate eating"
		),
		TestCase(
			name="Two Philosophers (tight timing death)",
			args=["2", "200", "100", "100"],
			timeout=5,
			expected_death=False,
			description="Two philosopher processes alternate eating without starvation at these timings"
		),
		TestCase(
			name="Four Philosophers (one dies at 310ms)",
			args=["4", "310", "200", "100"],
			timeout=5,
			expected_death=True,
			expected_death_time=(300, 320),
			description="One philosopher dies at ~310ms"
		),
		TestCase(
			name="Four Philosophers (no death at 410ms)",
			args=["4", "410", "200", "200"],
			timeout=5,
			expected_death=False,
			description="Adequate time prevents starvation with semaphores"
		),
		TestCase(
			name="Five Philosophers (no death)",
			args=["5", "800", "200", "200"],
			timeout=5,
			expected_death=False,
			description="Odd number philosophers in processes"
		),
		TestCase(
			name="Five Philosophers with must_eat=7",
			args=["5", "800", "200", "200", "7"],
			timeout=10,
			expected_death=False,
			description="All child processes terminate after each eats 7 times"
		),
		TestCase(
			name="Large Scale (200 philosophers)",
			args=["200", "800", "200", "200"],
			timeout=30,
			expected_death=False,
			description="Stress test with 200 processes"
		),
	]


def menu_choice() -> str:
	"""Display menu and get user choice"""
	print("\n" + "="*70)
	print("PHILO TEST SUITE")
	print("="*70)
	print("\nChoose which tests to run:")
	print("  1) Mandatory part (philo) - threads-based implementation")
	print("  2) Bonus part (philo_bonus) - process-based with semaphores")
	print("  3) Both (mandatory and bonus)")
	print("  4) Custom test (run with your own arguments)")
	print("  5) Exit")
	print()
	
	choice = input("Enter choice (1-5): ").strip()
	return choice


def find_binary(name: str, project_root: str) -> str:
	"""Find binary in project structure (relative to project root)"""
	paths = [
		os.path.join(project_root, "philo", name),
		os.path.join(project_root, "philo_bonus", name),
	]
	for path in paths:
		if os.path.isfile(path) and os.access(path, os.X_OK):
			return path
	raise FileNotFoundError(f"Binary '{name}' not found in {project_root}")


def build_binaries(project_root: str) -> bool:
	"""Build both philo and philo_bonus if not already built"""
	print("\n" + "="*70)
	print("Building binaries...")
	print("="*70 + "\n")
	
	try:
		# Build philo
		print("Building philo...")
		result = subprocess.run(
			["make", "-C", f"{project_root}/philo"],
			capture_output=True,
			timeout=60
		)
		if result.returncode != 0:
			print(f"✗ Failed to build philo")
			print(result.stderr.decode())
			return False
		print("✓ philo built successfully")
		
		# Build philo_bonus
		print("Building philo_bonus...")
		result = subprocess.run(
			["make", "-C", f"{project_root}/philo_bonus"],
			capture_output=True,
			timeout=60
		)
		if result.returncode != 0:
			print(f"✗ Failed to build philo_bonus")
			print(result.stderr.decode())
			return False
		print("✓ philo_bonus built successfully\n")
		
		return True
	except Exception as e:
		print(f"✗ Build error: {e}\n")
		return False


def cleanup_binaries(project_root: str) -> bool:
	"""Clean up binaries and object files after tests"""
	print("\n" + "="*70)
	print("Cleaning up (fclean)...")
	print("="*70 + "\n")
	
	try:
		# Clean philo
		print("Cleaning philo...")
		result = subprocess.run(
			["make", "-C", f"{project_root}/philo", "fclean"],
			capture_output=True,
			timeout=30
		)
		if result.returncode != 0:
			print(f"⚠ Warning: fclean philo had issues")
		else:
			print("✓ philo cleaned")
		
		# Clean philo_bonus
		print("Cleaning philo_bonus...")
		result = subprocess.run(
			["make", "-C", f"{project_root}/philo_bonus", "fclean"],
			capture_output=True,
			timeout=30
		)
		if result.returncode != 0:
			print(f"⚠ Warning: fclean philo_bonus had issues")
		else:
			print("✓ philo_bonus cleaned\n")
		
		return True
	except Exception as e:
		print(f"⚠ Cleanup error: {e}\n")
		return False


def run_custom_test(philo_bin: str, philo_bonus_bin: str):
	"""Run a custom test with user-provided arguments"""
	print("\n" + "="*70)
	print("CUSTOM TEST MODE")
	print("="*70)
	
	# Choose binary
	print("\nWhich binary to test?")
	print("  1) philo (mandatory - threads)")
	print("  2) philo_bonus (bonus - processes)")
	bin_choice = input("Enter choice (1-2): ").strip()
	
	if bin_choice == "1":
		binary = philo_bin
		bin_name = "philo"
	elif bin_choice == "2":
		binary = philo_bonus_bin
		bin_name = "philo_bonus"
	else:
		print("✗ Invalid choice")
		return
	
	# Get arguments
	print(f"\nEnter arguments for {bin_name}:")
	print("Example: 4 800 200 200 5")
	print("(N time_to_die time_to_eat time_to_sleep [optional: must_eat])")
	args_input = input("\nArguments: ").strip()
	
	if not args_input:
		print("✗ No arguments provided")
		return
	
	# Get timeout
	timeout_input = input("Timeout in seconds (default: 10): ").strip()
	try:
		timeout = int(timeout_input) if timeout_input else 10
	except ValueError:
		timeout = 10
	
	# Run the test
	print(f"\n{'='*70}")
	print(f"Running: {binary} {args_input}")
	print(f"Timeout: {timeout}s")
	print(f"{'='*70}\n")
	
	cmd = [binary] + args_input.split()
	
	try:
		result = subprocess.run(
			cmd,
			timeout=timeout,
			text=True
		)
		print(f"\n{'='*70}")
		print(f"Exit code: {result.returncode}")
		print(f"{'='*70}")
	except subprocess.TimeoutExpired:
		print(f"\n{'='*70}")
		print(f"TIMEOUT: Program ran longer than {timeout}s")
		print(f"{'='*70}")
	except Exception as e:
		print(f"\n✗ Error: {e}")


def main():
	"""Main entry point"""
	print("\n" + "="*70)
	print("PHILO COMPREHENSIVE TEST SUITE")
	print("="*70)

	# Determine project root relative to script location
	script_dir = os.path.dirname(os.path.abspath(__file__))
	project_root = script_dir  # test_philo.py is in the project root
	
	philo_bin = None
	philo_bonus_bin = None

	# Try to find binaries, build if needed
	try:
		philo_bin = find_binary("philo", project_root)
		philo_bonus_bin = find_binary("philo_bonus", project_root)
		print(f"\n✓ Found philo: {philo_bin}")
		print(f"✓ Found philo_bonus: {philo_bonus_bin}")
	except FileNotFoundError:
		print("\n⚠ Binaries not found. Attempting to build...")
		if not build_binaries(project_root):
			print("\n✗ Failed to build binaries. Exiting.")
			sys.exit(1)
		
		# Try finding again after build
		try:
			philo_bin = find_binary("philo", project_root)
			philo_bonus_bin = find_binary("philo_bonus", project_root)
			print(f"\n✓ Found philo: {philo_bin}")
			print(f"✓ Found philo_bonus: {philo_bonus_bin}")
		except FileNotFoundError as e:
			print(f"\n✗ Error: {e}")
			sys.exit(1)

	test_results_overall = []
	
	while True:
		choice = menu_choice()

		if choice == "1":
			tester = PhiloTester(philo_bin)
			tests = get_mandatory_tests()
			all_passed = tester.run_tests(tests)
			tester.print_summary()
			test_results_overall.append(("Mandatory", all_passed))
			if not all_passed:
				print("⚠ Some tests failed!")
		
		elif choice == "2":
			tester = PhiloTester(philo_bonus_bin)
			tests = get_bonus_tests()
			all_passed = tester.run_tests(tests)
			tester.print_summary()
			test_results_overall.append(("Bonus", all_passed))
			if not all_passed:
				print("⚠ Some tests failed!")
		
		elif choice == "3":
			tester1 = PhiloTester(philo_bin)
			tests1 = get_mandatory_tests()
			passed1 = tester1.run_tests(tests1)
			tester1.print_summary()

			tester2 = PhiloTester(philo_bonus_bin)
			tests2 = get_bonus_tests()
			passed2 = tester2.run_tests(tests2)
			tester2.print_summary()

			print(f"\n{'='*70}")
			print("OVERALL SUMMARY")
			print(f"{'='*70}")
			print(f"Mandatory (philo): {'PASSED ✓' if passed1 else 'FAILED ✗'}")
			print(f"Bonus (philo_bonus): {'PASSED ✓' if passed2 else 'FAILED ✗'}")
			print()
			test_results_overall.append(("Mandatory", passed1))
			test_results_overall.append(("Bonus", passed2))
		
		elif choice == "4":
			run_custom_test(philo_bin, philo_bonus_bin)
		
		elif choice == "5":
			print("\nCleaning up before exit...")
			cleanup_binaries(project_root)
			print("Goodbye!\n")
			sys.exit(0)
		
		else:
			print("\n✗ Invalid choice. Please enter 1-5.\n")


if __name__ == "__main__":
	main()
