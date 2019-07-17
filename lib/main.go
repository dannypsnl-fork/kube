package main

// #include <stdint.h>
import "C"
import (
	"unsafe"

	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
	"k8s.io/client-go/tools/clientcmd"
)

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
	clientsetMap[*clientsetID] = clientset
	return nil
}

//export delete_clientset
func delete_clientset(clientsetID C.uintptr_t) {
	delete(clientsetMap, clientsetID)
}

func main() {}
