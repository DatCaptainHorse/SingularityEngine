## Contributing

Contributions are highly welcome!

Experiment with the code, use the project for learning modern C++ and play around.

For a proper PR (Pull Request):

* Run your code through ClangFormat
* Add and/or modify tests if your changes require it.
* Compile and run without errors, tests should pass without failures.
* Split your changes into smaller commits if possible, don't make one massive commit.

### Code Style

Code style is subject to changes initially, as long as you use the ClangFormat you'll be fine!

Generally use tabs for indentation, avoid trailing whitespaces and keep to column limit.

### Commit Style

Commit title should be short, solid and in present tense, while prefixing the engine module in the title.

For example: `Audio: Clean up sounds on exit` or `Maths: Fix matrix SSE multiplication`

For more detailed changes, put them in the commit description!

### Dependencies

Try to keep your contributions light without extra dependencies unless absolutely necessary.

If your required dependency isn't available through xmake, add it as a Git submodule in the `External/` directory.