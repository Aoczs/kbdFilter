#include "wdm.h"

//Declare the function used

//Filter-device's extension 
typedef struct _KBD_FILTER_EXT
{
	//This device's DeviceObject
	PDEVICE_OBJECT DeviceObject;
	//The next lower device's DeviceObject
	PDEVICE_OBJECT LowerDeviceObject;
	//The PDO of this Device_Stack
	PDEVICE_OBJECT Pdo;
	//Symbolic name
	UNICODE_STRING SymbolicName;

}KBD_FILTER_EXT, *PKBD_FILTER_EXT;

//Define functions are used
NTSTATUS DefaultDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS PowerDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS PnpDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS PnpMniorDispatch()

NTSTATUS AddFilter(PDRIVER_OBJECT DriverObject, PDEIVE_OBJECT pdo);
NTSTATUS FilterUnload(PDRIVER_OBJECT DriverObject);