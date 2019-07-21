# kube

A kubernetes client

### Usage

```rust
use kube::Config;
use kube::Namespace;

fn main() {
    let cluster = kube::attach_cluster(Config::InCluster);
    let pod = cluster.get::<Pod>(Namespace::Namespace("default"), "nginx-se3jn1-34jbk");
    let pods = cluster.list::<Pod>(Namespace::All);
    // ...
}
```
