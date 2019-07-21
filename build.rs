use std::process::Command;

fn main() {
    // build Go c-shared library
    Command::new("go")
        .args(&[
            "build",
            "-o",
            "libkube_wrap.so",
            "-buildmode=c-shared",
            "main.go",
        ])
        .status()
        .unwrap();

    println!("cargo:rustc-link-lib=dylib=kube_wrap");
    println!("cargo:rustc-link-search=native=.");
}
