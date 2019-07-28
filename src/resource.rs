use super::Result;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

pub trait Resource: Sized {
    fn from_str(s: &str) -> Result<Self>;
    fn from_str_to_list(s: &str) -> Result<List<Self>>;
    fn resource_type() -> String;
}

#[derive(Serialize, Deserialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct TypeMeta {
    kind: Option<String>,
    api_version: Option<String>,
}

#[derive(Serialize, Deserialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct ObjectMeta {
    name: Option<String>,
    generate_name: Option<String>,
    namespace: Option<String>,
    self_link: Option<String>,
    resource_version: Option<String>,
    generation: Option<i64>,
    deletion_grace_period_seconds: Option<i64>,
    labels: Option<HashMap<String, String>>,
    annotations: Option<HashMap<String, String>>,
    finalizers: Option<Vec<String>>,
    cluster_name: Option<String>,
}

#[derive(Serialize, Deserialize, Default)]
pub struct Pod {
    #[serde(flatten)]
    pub type_meta: TypeMeta,
    pub metadata: Option<ObjectMeta>,
    // TODO: spec
    // TODO: status
}

impl Resource for Pod {
    fn from_str(s: &str) -> Result<Pod> {
        let pod = serde_json::from_str(s)?;
        Ok(pod)
    }
    fn from_str_to_list(s: &str) -> Result<List<Pod>> {
        let pod_list = serde_json::from_str(s)?;
        Ok(pod_list)
    }
    fn resource_type() -> String {
        "pods".to_string()
    }
}

#[derive(Serialize, Deserialize, Default)]
#[serde(rename_all = "camelCase")]
pub struct List<T> {
    #[serde(flatten)]
    pub type_meta: TypeMeta,
    // TODO: metadata
    pub items: Vec<T>,
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    #[test]
    fn pod_from_str() {
        let json_str = json!({
            "apiVersion": "v1",
            "kind": "Pod",
            "metadata": {
                "creationTimestamp": "2019-07-20T13:17:05Z",
                "generateName": "debug-849b8df67f-",
                "labels": {
                    "app": "debug",
                    "pod-template-hash": "849b8df67f"
                },
                "name": "debug-849b8df67f-bstn4",
                "namespace": "default",
                "ownerReferences": [
                    {
                        "apiVersion": "apps/v1",
                        "blockOwnerDeletion": true,
                        "controller": true,
                        "kind": "ReplicaSet",
                        "name": "debug-849b8df67f",
                        "uid": "ab153f4b-aaf0-11e9-ba86-025000000001"
                    }
                ],
                "resourceVersion": "218912",
                "selfLink": "/api/v1/namespaces/default/pods/debug-849b8df67f-bstn4",
                "uid": "ab1a815a-aaf0-11e9-ba86-025000000001"
            }
        });
        let pod = Pod::from_str(json_str.to_string().as_str()).unwrap();
        assert_eq!(pod.type_meta.kind.unwrap(), "Pod");
        assert_eq!(
            pod.metadata.unwrap().name.unwrap(),
            "debug-849b8df67f-bstn4"
        );
    }

    #[test]
    fn pod_list_from_str() {
        let pod_list_str = json!({
            "apiVersion": "v1",
            "kind": "List",
            "items": []
        });
        let pod_list = Pod::from_str_to_list(pod_list_str.to_string().as_str()).unwrap();
        assert_eq!(pod_list.items.len(), 0);
    }
}
