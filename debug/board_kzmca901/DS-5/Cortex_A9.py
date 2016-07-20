from com.arm.debug.dtsl.configurations import DTSLv1
from com.arm.debug.dtsl.components import PTMTraceSource
from com.arm.debug.dtsl.components import ETBTraceCapture
from com.arm.debug.dtsl.components import CSTPIU
from com.arm.debug.dtsl.components import Device
from com.arm.debug.dtsl.components import CSFunnel
from com.arm.debug.dtsl.components import FormatterMode
from com.arm.debug.dtsl import DTSLException


class CortexA9Board(DTSLv1):
    def __init__(self, root, cores):
        """
        Constructor which creates the self.core_associations_map[] 
        """
        DTSLv1.__init__(self, root)
        self.core_list = cores
        self.trace_sources = []

    def getFunnelPort(self, core):
        #
        # select port for desired core
        # *** User must customize for specific board oddities ***
        #
        port = core
        return port

    def a9SingleCoreETB(self):
        coreNo = self.core_list[0]

        managedDevices = []

        # disable the TPIU (if present) to allow ETB to work at full rate
        try:
            tpiuDev = self.findDevice("CSTPIU")
            tpiu = CSTPIU(self, tpiuDev, "TPIU")
            tpiu.setEnabled(False)
            managedDevices.append(tpiu)
        except DTSLException:
            # Ignore if not present
            pass

        # enable port for self. core on the funnel
        funnelDev = self.findDevice("CSTFunnel")
        funnel = CSFunnel(self, funnelDev, "Funnel")
        # To enable widest range of ETB targets
        # enable all ports on the funnel
        # NOTE: If a more specific funnel port strategy is required
        # enable the specific port here using the code below or similar:
        #funnel.setAllPortsDisabled()
        #funnel.setPortEnabled(self.getFunnelPort(coreNo))
        for i in range(0, 8):
            funnel.setPortEnabled(i)

        # find first core/PTM
        coreDev = self.findDevice("Cortex-A9")
        ptmDev = self.findDevice("CSPTM")

        if coreNo == -1:
            # single core system - don't suffix device name
            devName = "Cortex-A9"
        else:
            # skip through list to desired core/PTM
            for i in range(0, coreNo):
                coreDev = self.findDevice("Cortex-A9", coreDev+1)
                ptmDev = self.findDevice("CSPTM", ptmDev+1)
            devName = "Cortex-A9_%d" % coreNo

        self.PTM = PTMTraceSource(self, ptmDev, 1, "PTM")
        self.addDeviceInterface(Device(self, coreDev, devName))

        etbDev = self.findDevice("CSETB")
        self.ETB = ETBTraceCapture(self, etbDev, "ETB")
        self.ETB.setFormatterMode(FormatterMode.BYPASS)

        self.ETB.addTraceSource(self.PTM, coreDev)
        self.ETB.setTraceComponentOrder([ funnel ])
        self.addTraceCaptureInterface(self.ETB)

        self.setManagedDevices(managedDevices + [ self.PTM, funnel, self.ETB ])


class Cortex_A9_0_ETB(CortexA9Board):
    def __init__(self, root):
        core = 0 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()

class Cortex_A9_1_ETB(CortexA9Board):
    def __init__(self, root):
        core = 1 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()

class Cortex_A9_2_ETB(CortexA9Board):
    def __init__(self, root):
        core = 2 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()

class Cortex_A9_3_ETB(CortexA9Board):
    def __init__(self, root):
        core = 3 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()

# Configuration for a single core system
class Cortex_A9_ETB(Cortex_A9_0_ETB):
    def __init__(self, root):
        core = -1
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()

class Cortex_A9_0_ETB_KernelOnly(CortexA9Board):
    def __init__(self, root):
        core = 0
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()
        #
        # The trace range may need adjusting for some kernels
        self.PTM.addTraceRange(0xBF000000,0xFFFFFFFF) 

class Cortex_A9_1_ETB_KernelOnly(CortexA9Board):
    def __init__(self, root):
        core = 1 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()
        #
        # The trace range may need adjusting for some kernels
        self.PTM.addTraceRange(0xBF000000,0xFFFFFFFF) 

class Cortex_A9_2_ETB_KernelOnly(CortexA9Board):
    def __init__(self, root):
        core = 2 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()
        #
        # The trace range may need adjusting for some kernels
        self.PTM.addTraceRange(0xBF000000,0xFFFFFFFF) 

class Cortex_A9_3_ETB_KernelOnly(CortexA9Board):
    def __init__(self, root):
        core = 3 
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()
        #
        # The trace range may need adjusting for some kernels
        self.PTM.addTraceRange(0xBF000000,0xFFFFFFFF)

# Configuration for a single core system
class Cortex_A9_ETB_KernelOnly(Cortex_A9_0_ETB_KernelOnly):
    def __init__(self, root):
        core = -1
        CortexA9Board.__init__(self, root, [core])
        self.a9SingleCoreETB()
        #
        # The trace range may need adjusting for some kernels
        self.PTM.addTraceRange(0xBF000000,0xFFFFFFFF)

