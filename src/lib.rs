use libc::uintptr_t;
use std::ffi::CString;
use std::os::raw::c_char;

#[derive(Debug)]
pub enum KubeError {
    GetWithoutNamespace,
    Wrap(String),
}

type Result<T> = std::result::Result<T, KubeError>;

pub struct Cluster {
    clientset: uintptr_t,
}

impl Cluster {
    pub fn get<'a, T: Resource>(&self, namespace: Namespace, resource_name: &'a str) -> Result<T> {
        let wrap_result = unsafe {
            let ns_str = match namespace {
                Namespace::All => Err(KubeError::GetWithoutNamespace),
                Namespace::Namespace(ns) => Ok(ns),
            }?;
            let ns = CString::new(ns_str).unwrap().into_raw();
            let r_type = CString::new(T::resource_type().as_str())
                .unwrap()
                .into_raw();
            let r_name = CString::new(resource_name).unwrap().into_raw();
            get_resource(self.clientset, ns, r_type, r_name)
        };
        let error_message = wrap_result.err;
        if !error_message.is_null() {
            let s = unsafe { CString::from_raw(error_message) };
            Err(KubeError::Wrap(s.into_string().unwrap()))
        } else {
            Ok(T::new())
        }
    }
    pub fn list<T: Resource>(&self, namespace: Namespace) -> Result<T> {
        let wrap_result = unsafe {
            let ns_str = match namespace {
                Namespace::All => "",
                Namespace::Namespace(ns) => ns,
            };
            let ns = CString::new(ns_str).unwrap().into_raw();
            let r_type = CString::new(T::resource_type().as_str())
                .unwrap()
                .into_raw();
            list_resource(self.clientset, ns, r_type)
        };
        let error_message = wrap_result.err;
        if !error_message.is_null() {
            let s = unsafe { CString::from_raw(error_message) };
            Err(KubeError::Wrap(s.into_string().unwrap()))
        } else {
            Ok(T::new())
        }
    }
}

pub fn attach_cluster(cfg: Config) -> Result<Cluster> {
    let mut clientset: uintptr_t = 0;
    let error_message = match cfg {
        Config::InCluster => unsafe { create_clientset_in_cluster(&mut clientset) },
        Config::Path(path) => unsafe {
            create_clientset_with_config_file(&mut clientset, CString::new(path).unwrap().as_ptr())
        },
    };
    if !error_message.is_null() {
        let s = unsafe { CString::from_raw(error_message) };
        Err(KubeError::Wrap(s.into_string().unwrap()))
    } else {
        Ok(Cluster { clientset })
    }
}

pub enum Namespace<'a> {
    All,
    Namespace(&'a str),
}

pub enum Config<'a> {
    InCluster,
    Path(&'a str),
}

pub trait Resource {
    fn new() -> Self;
    fn resource_type() -> String;
}

pub struct Pod {}

impl Resource for Pod {
    fn new() -> Pod {
        Pod {}
    }
    fn resource_type() -> String {
        "pods".to_string()
    }
}

#[repr(C)]
struct WrapResult {
    result: *mut c_char,
    err: *mut c_char,
}

#[link(name = "kube_wrap")]
extern "C" {
    fn create_clientset_in_cluster(clientset_ptr: *mut uintptr_t) -> *mut c_char;
    fn create_clientset_with_config_file(
        clientset_ptr: *mut uintptr_t,
        config_path: *const c_char,
    ) -> *mut c_char;

    fn get_resource(
        clientset_ptr: uintptr_t,
        namespace: *const c_char,
        resource_type: *const c_char,
        resource_name: *const c_char,
    ) -> WrapResult;
    fn list_resource(
        clientset_ptr: uintptr_t,
        namespace: *const c_char,
        resource_type: *const c_char,
    ) -> WrapResult;
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn get_pod() {
        let cluster = attach_cluster(Config::Path("/Users/dannypsnl/.kube/config")).unwrap();
        cluster
            .get::<Pod>(Namespace::Namespace("default"), "nginx")
            .unwrap();
    }
}
