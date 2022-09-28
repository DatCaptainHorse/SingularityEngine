# Singularity Engine

## Introduction

Singularity Engine is an open-source game/application engine written in C++

The engine aims to use latest C++ and features, currently targeting C++20 with modules.

This project can be used for learning and experimenting purposes,
while things are kept clean, don't expect a super-serious .

## Current Status

In progress of bringing in "good bits" from old, private version of engine and modulizing and rewriting the code.

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

## Code of Conduct

* Provide a friendly, safe and welcoming environment for others, no matter what.
* There is no need to be rude, mean or hurtful.
* Respect different opinions, try to see things from other's viewpoint.
* No spamming, trolling, flaming, baiting or other attention-stealing behavior.
* Failing to follow these guidelines repeatedly will result in you being excluded from interactions.
* Being a contributor does not give you special rights to avoid following these guidelines.

## History

Singularity Engine started many years back as a learning project for C++

The engine was initially based on [Sparky Game Engine](https://github.com/TheCherno/Sparky),
but once the project took it's own path any relation to Sparky was quickly gone.

The project grew larger and new features were constantly added, it was too much for one person.
Thus decision was made to rewrite Singularity Engine as a public open-source project,
preserving some of the "good bits" from original private version.
