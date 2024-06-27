{ stdenv, pkgs, lib, fetchFromGitHub, reactor-cpp, ... }:
let 
	xarm7 = stdenv.mkDerivation {
		name = "xarm7";

		src = fetchFromGitHub {
			owner = "xArm-Developer";
			repo = "xArm-CPLUS-SDK";
			rev = "v1.13.21";
			hash = "sha256-QuR7TltD/mjXASa3smJN76h53iRnhHEz+3H4u+841m4=";
		};
		installPhase = ''
			ls -alh
			ls -alh ./build
			find ./build 
			mkdir -p $out/
			cp -r ./build/lib $out/
			cp -r ./include/ $out/
		'';

	};
in
stdenv.mkDerivation {
  name = "alarm-clock";
  version = "0.0.1";

  src = ./..;
  
  #depsBuildBuild = with pkgs; [
  #  pkg-config
  #];

  nativeBuildInputs = with pkgs; [
    pkg-config
  ];

  buildInputs = with pkgs; [ 
    which 
    gcc 
    cmake
    git 
    mujoco 
    jdk17_headless 
    #freeglut
    #xorg.libX11
    #xorg.libXext
    #libGL
    libGLU
    #libxkbcommon
    #libdecor
    glfw
    eigen
    opencv
    #glfw-wayland
		xarm7
  ];

  configurePhase = ''
    export LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib";
  '';

  buildPhase = ''
    #echo "Starting compiling"
    #mkdir -p include/reactor-cpp/
    #cp -r ${reactor-cpp}/include/reactor-cpp/* include/reactor-cpp/
    ${pkgs.lingua-franca}/bin/lfc --external-runtime-path ${reactor-cpp}/ ./src/MujocoSimulator.lf
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp -r ./bin/* $out/bin
  '';
}

