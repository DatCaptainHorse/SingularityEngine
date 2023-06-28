{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.05";
    unstable.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    nix-filter.url = "github:numtide/nix-filter";
  };

  outputs = { self, nixpkgs, unstable, flake-utils, nix-filter }:
    let pkgname = "se";
    in flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        unstablePkgs = unstable.legacyPackages.${system};

        stdenv = pkgs.llvmPackages_16.libcxxStdenv;

        clang-tools_16_cxx = pkgs.clang-tools_16.overrideAttrs (final: prev: {
          clang = pkgs.llvmPackages_16.libcxxClang;
        });

        se = stdenv.mkDerivation {
          pname = "se";
          version = "0.0.0";
          src = nix-filter.lib {
            root = ./.;
            include = [
              "CMakeLists.txt"
              (nix-filter.lib.inDirectory "External")
              (nix-filter.lib.inDirectory "CMake")
              (nix-filter.lib.inDirectory "Source")
              (nix-filter.lib.inDirectory "Tests")
              (nix-filter.lib.inDirectory "Benchmarks")
            ];
          };
          nativeBuildInputs = with pkgs; [
            clang-tools_16_cxx

            unstablePkgs.cmake
            ninja
          ];
          buildInputs = with pkgs; [
            catch2_3
			gbenchmark
          ];
        };
      in
      {
        packages.default = se;

        devShells.default = (pkgs.mkShell.override { stdenv = stdenv; }) {
          inputsFrom = [
            se
          ];

          packages = with pkgs; [
            nixpkgs-fmt
          ];

          shellHook = ''
            set -e
            if [ ! -d build ]; then
              # CMake makes a dumb assumption and will occasionally pull clang-scan-deps from somewhere outside of the Nix store.
              # This is an equally dumb workaround for now.
              cmake -G Ninja --preset=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_COMPILER_CLANG_SCAN_DEPS=$(which clang-scan-deps)
              echo ""
              echo "* I have created a release build directory for you under 'build'"
              echo "* Compile commands are exported for clangd, use 'ninja' to build"
              echo ""
            fi
          '';
        };
      });
}
