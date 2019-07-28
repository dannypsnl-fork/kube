extern crate dirs;
extern crate kube;

use dirs::home_dir;
use kube::resource::Pod;
use kube::Config;
use kube::Namespace;

fn main() {
    let mut kube_config_path = home_dir().expect("failed at get home directory");
    kube_config_path.push(".kube");
    kube_config_path.push("config");

    let cluster = kube::attach_cluster(Config::Path(kube_config_path.to_str().unwrap())).unwrap();
    let pod_list = cluster.list::<Pod>(Namespace::All).unwrap();
    println!("{:?}", pod_list);
}
