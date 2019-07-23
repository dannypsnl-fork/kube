workflow "test on push" {
  on = "push"
  resolves = ["cargo test"]
}
action "cargo test" {
  uses = "docker://rust:1.36"
  runs = "cargo"
  args = "test --verbose"
}
