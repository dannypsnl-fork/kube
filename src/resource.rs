use serde::{Deserialize, Serialize};
use std::collections::HashMap;

pub trait Resource {
    fn from_str(s: &str) -> Self;
    fn resource_type() -> String;
}

#[derive(Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct TypeMeta {
    kind: Option<String>,
    api_version: Option<String>,
}

#[derive(Serialize, Deserialize)]
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

#[derive(Serialize, Deserialize)]
pub struct Pod {
    #[serde(flatten)]
    type_meta: TypeMeta,
}

impl Resource for Pod {
    fn from_str(s: &str) -> Pod {
        serde_json::from_str(s).expect("serde fail")
    }
    fn resource_type() -> String {
        "pods".to_string()
    }
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
        let pod = Pod::from_str(json_str.to_string().as_str());
        assert_eq!(pod.type_meta.kind.unwrap(), "Pod");
    }
}
