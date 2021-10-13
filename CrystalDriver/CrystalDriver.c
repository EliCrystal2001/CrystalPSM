/*
*	 Written by Eli Crystal
*	 This file is part of CrystalDriver.c
*	 Which is part of the CrystalPsmInstaller program.
*/

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <vitasdkkern.h>
#include <psp2kern/kernel/proc_event.h>
#include <psp2kern/fios2.h>
#include <taihen.h>

#define SceSblQafMgr (0x756B7E89)
#define SceSblQafMgrIsAllowLimitedDebugMenuDisplay (0xC456212D)

#define SceIofilemgrForDriver (0x40FD29C7)
#define SceIoOpen (0x75192972)


static int PkgInstallerHook = -1;
static tai_hook_ref_t PkgInstallerPkgInstallerHookReference;

static SceFiosOverlay Overlay;
static int OverlayOut;

/*
*	Apply overlay (thanks princess!)
*	Callback for when a process starts.
*/
int ProcessCreated(SceUID Pid, SceProcEventInvokeParam2 *Param, int Unknown){
	memset(&Overlay, 0x00, sizeof(SceFiosOverlay));
	
	Overlay.type = SCE_FIOS_OVERLAY_TYPE_OPAQUE;
	Overlay.order = 0xFF;
	Overlay.dst_len = strlen("host0:/package")+1;
	Overlay.src_len = strlen("ux0:/data")+1;
	Overlay.pid = Pid;
	strncpy(Overlay.dst, "host0:/package", strlen("host0:/package")+1);
	strncpy(Overlay.src,"ux0:/data", strlen("ux0:/data")+1);
	OverlayOut = 0;
	
	ksceFiosKernelOverlayAddForProcess(Pid, &Overlay, &OverlayOut);
	return 0;
}

/*
*	Force Package Installer Enabled.
*/
int sceSblQafMgrIsAllowLimitedDebugMenuDisplayPatched(){
	if (PkgInstallerHook >= 0) taiHookReleaseForKernel(PkgInstallerHook, PkgInstallerPkgInstallerHookReference);  // unhook, this ensures it only runs once
	return 1;
}


/*
*	Required Entry Point function
*/
void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args){
	ksceIoRemove("ux0:/data/CrystalDriver.skprx");
	
	/*
	*	Register a ProcEventHandler
	*	Gives me a callback every time a process starts.
	*/
	
	SceProcEventHandler Handler;
	memset(&Handler, 0, sizeof(Handler));
	Handler.size = sizeof(Handler);
	Handler.create = ProcessCreated;

	ksceKernelRegisterProcEventHandler("CrystalIoRedirect", &Handler, 0);
	
	/*
	*	Package Installer checks this flag
	*	If it returns 0, then the Package Installer just exits.
	*/
	PkgInstallerHook = taiHookFunctionExportForKernel(KERNEL_PID, &PkgInstallerPkgInstallerHookReference, "SceSblSsMgr", SceSblQafMgr, SceSblQafMgrIsAllowLimitedDebugMenuDisplay, sceSblQafMgrIsAllowLimitedDebugMenuDisplayPatched);	
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	if (PkgInstallerHook >= 0) taiHookReleaseForKernel(PkgInstallerHook, PkgInstallerPkgInstallerHookReference); 
    return SCE_KERNEL_STOP_SUCCESS;
}


