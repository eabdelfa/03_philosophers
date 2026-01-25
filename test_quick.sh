#!/bin/bash

echo "=== PHILO TESTS ==="
echo "Test 1: Single philosopher (should die at ~800ms)"
timeout 2 philo/philo 1 800 100 100 2>&1 | tail -1
echo ""

echo "Test 2: Two philosophers, no must_eat (should run indefinitely)"
timeout 1 philo/philo 2 400 100 100 2>&1 | wc -l
echo "Output lines above (should be many)"
echo ""

echo "Test 3: Four philosophers, must_eat=2"
timeout 3 philo/philo 4 500 100 100 2 2>&1 | tail -3
echo ""

echo "=== PHILO BONUS TESTS ==="
echo "Test 4: Single philosopher bonus (should die at ~800ms)"
timeout 2 philo_bonus/philo_bonus 1 800 100 100 2>&1 | tail -1
echo ""

echo "Test 5: Two philosophers bonus, must_eat=1"
timeout 3 philo_bonus/philo_bonus 2 10000 100 100 1 2>&1 | tail -3
echo ""

echo "Test 6: Three philosophers bonus, must_eat=2"
timeout 5 philo_bonus/philo_bonus 3 800 200 200 2 2>&1 | tail -3
