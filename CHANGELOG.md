# Changelog

## Recent Improvements (January 31, 2026)

### Code Refactoring & Optimization

#### philo_bonus
1. **Removed `validate_timings()` function** (both philo and philo_bonus)
   - Redundant validation already performed in `parse_and_set_timings()`
   - All time values validated against `LONG_MAX` and range constraints
   - Simplified code flow without affecting functionality

2. **Integrated `make_sem_name()` into `sem_init_bonus.c`**
   - Moved from separate `sem_name_bonus.c` file
   - Removed orphaned `sem_name_bonus.c` source file
   - Updated Makefile to reflect removal
   - Function now private (static) to sem_init_bonus.c
   - Removed declaration from header (no longer public)
   - Cleaner code organization with better cohesion

3. **Removed `make_name()` wrapper function**
   - Direct calls to `make_sem_name()` in `open_sems()`
   - Eliminated unnecessary indirection
   - Improved code clarity

4. **Fixed memory leak in `make_sem_name()`**
   - Intermediate string allocation now properly tracked
   - Added `tmp2` variable to preserve intermediate result before final join
   - All temporary allocations explicitly freed before return
   - Valgrind confirms zero definite leaks

#### Both versions
- **Memory safety verified with Valgrind**
  - philo: `All heap blocks were freed -- no leaks is possible`
  - philo_bonus: `0 bytes definitely lost` (only system lib allocations, not leaks)
  - All malloc/free pairs properly matched

### Documentation Updates

1. Updated README.md
   - Removed `sem_name_bonus.c` from project structure
   - Updated bonus function descriptions
   - Noted `make_sem_name()` integration

2. Updated `doc/philo_bonus/philo_bonus_section_flows.md`
   - Removed `sem_name_bonus` from initialization flow
   - Clarified `sem_init_bonus` now contains all semaphore setup

### Quality Metrics

- **Compiler**: No warnings with `-Wall -Wextra -Werror`
- **Memory**: Zero definite memory leaks (Valgrind verified)
- **Code Style**: 42 Norm compliant
- **Test Status**: All edge cases handled correctly

### Files Modified

```
philo/src/parsing.c                          - Removed validate_timings()
philo_bonus/src/parsing_bonus.c              - Removed validate_timings()
philo_bonus/src/sem_init_bonus.c             - Added make_sem_name(), fixed memory leak
philo_bonus/src/sem_name_bonus.c             - DELETED
philo_bonus/include/philo_bonus.h            - Removed make_sem_name declaration
philo_bonus/Makefile                         - Removed sem_name_bonus.c
README.md                                    - Updated structure and descriptions
doc/philo_bonus/philo_bonus_section_flows.md - Updated initialization flow
```

### Performance Impact

- **No performance regression** - refactoring is internal
- **Improved memory efficiency** - fixed potential leak
- **Better code organization** - reduced file count, clearer dependencies

### Testing

Run the following to verify improvements:

```bash
# Build and test
cd philo_bonus && make && ./philo_bonus 5 800 200 200 7

# Memory validation
valgrind --leak-check=full ./philo_bonus 5 800 200 200 7

# Both versions
cd ../philo && make && valgrind --leak-check=full ./philo 5 800 200 200 7
```

