package main

// #include <stdint.h>
import "C"
import (
	"sync"
	"unsafe"

	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/kubernetes/scheme"
	"k8s.io/client-go/rest"
	"k8s.io/client-go/tools/clientcmd"
)

var clientsetMutex sync.Mutex
var clientsetMap = map[C.uintptr_t]*kubernetes.Clientset{}

//export create_clientset_in_cluster
func create_clientset_in_cluster(clientsetID *C.uintptr_t) *C.char {
	config, err := rest.InClusterConfig()
	if err != nil {
		return C.CString(err.Error())
	}
	return newClientset(clientsetID, config)
}

//export create_clientset_with_config_file
func create_clientset_with_config_file(clientsetID *C.uintptr_t, configPath *C.char) *C.char {
	cfgPath := C.GoString(configPath)
	config, err := clientcmd.BuildConfigFromFlags("", cfgPath)
	if err != nil {
		return C.CString(err.Error())
	}
	return newClientset(clientsetID, config)
}

func newClientset(clientsetID *C.uintptr_t, cfg *rest.Config) *C.char {
	clientset, err := kubernetes.NewForConfig(cfg)
	if err != nil {
		return C.CString(err.Error())
	}
	*clientsetID = C.uintptr_t(uintptr(unsafe.Pointer(clientset)))
	// TODO: mutex or sync.Map?
	clientsetMutex.Lock()
	defer clientsetMutex.Unlock()
	clientsetMap[*clientsetID] = clientset
	return nil
}

//export delete_clientset
func delete_clientset(clientsetID C.uintptr_t) {
	clientsetMutex.Lock()
	defer clientsetMutex.Unlock()
	delete(clientsetMap, clientsetID)
}

//export get_resource
func get_resource(clientsetID C.uintptr_t, namespace, resourceType, resourceName *C.char) (result *C.char, err *C.char) {
	clientset, ok := clientsetMap[clientsetID]
	if ok {
		// FIXME: add a parameter for different version API, so we can get a workable REST Client
		raw, err := clientset.CoreV1().RESTClient().
			Get().
			Namespace(C.GoString(namespace)).
			Resource(C.GoString(resourceType)).
			Name(C.GoString(resourceName)).
			VersionedParams(&metav1.GetOptions{}, scheme.ParameterCodec).
			DoRaw()
		if err != nil {
			return nil, C.CString(err.Error())
		}
		return C.CString(string(raw)), nil
	}
	return nil, C.CString("no such clientset")
}

//export list_resource
func list_resource(clientsetID C.uintptr_t, namespace, resourceType *C.char) (result *C.char, err *C.char) {
	clientset, ok := clientsetMap[clientsetID]
	if ok {
		// FIXME: add a parameter for different version API, so we can get a workable REST Client
		raw, err := clientset.CoreV1().RESTClient().
			Get().
			Namespace(C.GoString(namespace)).
			Resource(C.GoString(resourceType)).
			VersionedParams(&metav1.ListOptions{}, scheme.ParameterCodec).
			DoRaw()
		if err != nil {
			return nil, C.CString(err.Error())
		}
		return C.CString(string(raw)), nil
	}
	return nil, C.CString("no such clientset")
}

func main() {}
