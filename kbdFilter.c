#include "kbdFilter.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	//Common variable represent the 'status'
	NTSTATUS status;

	//Set the MajorFunction for IRP
	for(ULONG i = 0; i < IRP_MJ_MAXNUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultDispatch;
	}

	//Set the specified IRP dispatch functions
	DriverObject->MajorFunction[IRP_MJ_PNP] = PnpDispatch;             
	DriverObject->MajorFunction[IRP_MJ_POWER] = PowerDispatch;

	//Set the Filter attached function(I want to use the Pnp attach)
	DriverObject->DriverExtension->AddDevice = AddFilter;

	//Set the driver unload function 
	//(when we unload filter manually, we need to ensure that there is no IRP don't return)
	DriverObject->DriverUnload = FilterUnload;
}

//Create a filter device object, and attach it to the device stack
NTSTATUS AddFilter(PDRIVER_OBJECT DirverObject, PDEVICE_OBJCET pdo)
{
	NTSTATUS status;
	//Device object which will be created
	PDEVICE_OBJECT kbdFilter;
	//Device object which be attached
	PDEVICE_OBJECT AttachedDevice;
//	PUNICODE_STRING filterName;

	//Create the filter device object
	status = IoCreateDevice(
		DriverObject,
		sizeof(KDB_FILTER_EXTENSION),
		//System will create a default name, and I do not think 
		//whether I need to create a Symbolic name for it well
		NULL,
		pdo->DeviceType,
		pdo->Characteristics,
		FALSE,
		&kbdFilter);
	//Set extension of the filter device
	PKBD_FILTER_EXTENSION filterExt;
	filterExt = kbdFilter->DeviceExtension;
	//Attach the filter device to the device stack
	IoAttachDeviceToDeviceStackSafe(
		kbdFilter,
		pdo,
		AttachedDevice);
	//fill the filter's extension with these device objects
	filterExt->DeviceObject = kbdFilter;
	filterExt->LowerDeviceObject = AttachedDevice;
	filterExt->Pdo = pdo;
	//Set the Flags of filter object as the attached device
	kbdFilter->Flags |= LowerDeviceObject->Flags & (DO_BUFFERED_IO | DO_DIRECT_IO | DO_POWER_PAGABLE);
	kbdFilter->Flags &= ~DO_DEVICE_INITIALIZING;
	//Set the other aspects of device object 
	kbdFilter->DeviceType = LowerDeviceObject->DeviceType;
	kbdFilter->Characteristics = LowerDeviceObject->Characteristics;
	kbdFilter->StackSize = LowerDeviceObject->StackSize + 1; 

	//Return the status, but I do not consider the failure of 'create & attach'
	return status;
}

NTSTATUS PnpDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS status;
}

NTSTATUS PowerDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	NTSTATUS status;
}

NTSTATUS FilterUnload(PDRIVER_OBJECT DriverObject)
{
	NTSTATUS status;
	//There has no need to process the unload
	status = STATUS_SUCCESS;
	return status;
}