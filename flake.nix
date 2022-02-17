{
  inputs.nixpkgs.url = "github:nixos/nixpkgs";

  outputs = { self, nixpkgs }: let
    pkgs = import nixpkgs { system = "x86_64-linux"; };
  in {
    defaultPackage.x86_64-linux = pkgs.stdenv.mkDerivation {
      name = "y86-asm";
      version = "v0.0.1";
      src = ./.;

      buildPhase = ''
        make -C c/
      '';

      installPhase = ''
        mkdir -p $out/bin

        cp c/y86-asm $out/bin/y86-asm
      '';

      installCheckPhase = ''
        cd tests
        ls
        PATH="$PATH:$out/bin" ./runner.sh
      '';

      doInstallCheck = true;
      dontConfigure = true;
    };

    # Put the binary on path so tests can be run unmodified.
    devShell.x86_64-linux = pkgs.mkShell {
      shellHook = ''
        PATH="$PATH:$(pwd)/c"
      '';
    };
  };
}
