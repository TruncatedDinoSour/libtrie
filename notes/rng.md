Libtrie uses a pseudorandom function to traverse the model and generate text.
It has the following functionality:

- `rng_iter`: A macro to modify state on new iteration.
- `rng_seed`: Seed a pseudorandom state.
- `rng_next`: Generate a new pseudorandom number (as a `uint32_t`).

If you're iterating over things (such as generating multiple sentences)
use `rng_iter` if possible. It updates the state based off the parameters
and current iteration:

```c
rng_iter(rng.state, iteration_count, current_index);
```

This will help to ensure unique output.

If you don't know the iteration count, pass in the maximum value of the iteration variable type (such as `UINT32_MAX`.
