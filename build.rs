use std::process::Command;

fn main() {
    // build Go c-shared library
    Command::new("go")
        .args(&[
            "build",
            "-o",
            "libkube_wrap.a",
            "-buildmode=c-archive",
            "main.go",
        ])
        .status()
        .expect("failed at build Go static library");

    let target = std::env::var("TARGET").unwrap();

    if target.contains("-apple") {
        println!("cargo:rustc-link-lib=framework=CoreFoundation");
        println!("cargo:rustc-link-lib=framework=Security");
    }

    println!("cargo:rustc-link-lib=static=kube_wrap");
    println!("cargo:rustc-link-search=native=.");
}
