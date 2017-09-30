#define RPC_DEFINITION(klass, method) RPC_##klass##_##method
#define DEPRECATED_RPC_DEFINITION(klass, method) DEPRECATED##_##RPC_##klass##_##method

enum
{
     RPC_DEFINITION(IRpcGlobalRequest, GetVersion)
    ,RPC_DEFINITION(IRpcGlobalRequest, GetState)
    ,RPC_DEFINITION(IRpcGlobalRequest, SleepCycles)

    ,RPC_DEFINITION(IRpcGlobalResponse, VersionResponse)
    ,RPC_DEFINITION(IRpcGlobalResponse, StateResponse)

    ,RPC_DEFINITION(IRpcMemoryAccess, MemoryWrite)
    ,RPC_DEFINITION(IRpcMemoryAccess, MemoryMaskedWrite)
    ,RPC_DEFINITION(IRpcMemoryAccess, MemoryRead)

    ,RPC_DEFINITION(IRpcMemoryResponse, MemoryResponse)

    ,RPC_DEFINITION(IRpcMisc, EchoRequest)
    ,RPC_DEFINITION(IRpcMisc, EchoResponse)

    ,RPC_DEFINITION(IRpcBREF, ExitBREF)
    ,RPC_DEFINITION(IRpcBREF, EnableDebugConsole)
    ,RPC_DEFINITION(IRpcBREF, SetPatchVersion)

    ,RPC_DEFINITION(IRpcFlashCommand, Write)
    ,RPC_DEFINITION(IRpcFlashCommand, Erase)
    ,RPC_DEFINITION(IRpcFlashCommand, Read)
    ,RPC_DEFINITION(IRpcFlashCommand, Crc)
    ,RPC_DEFINITION(IRpcFlashCommand, Config)

    ,RPC_DEFINITION(IRpcFlashResponse, Read)
    ,RPC_DEFINITION(IRpcFlashResponse, Crc)
    ,RPC_DEFINITION(IRpcFlashResponse, Config)
    ,RPC_DEFINITION(IRpcFlashResponse, Error)

    ,RPC_DEFINITION(IRpcGlobalUtils, Exception)
    ,RPC_DEFINITION(IRpcGlobalUtils, ExceptionWithArgs)
    ,RPC_DEFINITION(IRpcGlobalUtils, Print)

    ,RPC_DEFINITION(IRpcGlobalRequest, Reset)
    ,RPC_DEFINITION(IRpcGlobalRequest, ResetTransportLayer)

    ,RPC_DEFINITION(IRpcTxNavigation, Start)
    ,RPC_DEFINITION(IRpcTxNavigation, BurstMode)
    ,RPC_DEFINITION(IRpcTxNavigation, Data)

    ,RPC_DEFINITION(IRpcRxNavigation, Data)

    ,RPC_DEFINITION(IRpcSensorRequest, Data)
    ,RPC_DEFINITION(IRpcSensorResponse, Data)

    ,RPC_DEFINITION(IRpcFailSafeRequest, ExitFailSafe)
    ,RPC_DEFINITION(IRpcFailSafeRequest, GetSysErrRecord)
    ,RPC_DEFINITION(IRpcFailSafeRequest, ReadDataRecord)

    ,RPC_DEFINITION(IRpcFailSafeResponse, SysErrRecordData)
    ,RPC_DEFINITION(IRpcFailSafeResponse, DataRecordRead)

    ,RPC_DEFINITION(IRpcWlsRequest, DbClear)
    ,RPC_DEFINITION(IRpcWlsRequest, DbAddEntries)
    ,RPC_DEFINITION(IRpcWlsRequest, WlsInit)

    ,RPC_DEFINITION(IRpcLoggingRequest,SetLoggerConfig)
    ,RPC_DEFINITION(IRpcLoggingRequest,SetLoggingConfig)

    ,RPC_DEFINITION(IRpcGlleRequest,  Data)
    ,RPC_DEFINITION(IRpcGlleResponse, Data)

    ,RPC_DEFINITION(IRpcGlobalRequest, StartScratchApp)
    ,RPC_DEFINITION(IRpcGlobalRequest, SetFlowControl)

    ,RPC_DEFINITION(IRpcMemoryAccess, MemoryChecksum32bit)
    ,RPC_DEFINITION(IRpcMemoryAccess, MemoryCrc8bit)

    ,RPC_DEFINITION(IRpcMemoryResponse, MemoryChecksum32bit)
    ,RPC_DEFINITION(IRpcMemoryResponse, MemoryCrc8bit)
    ,RPC_DEFINITION(IRpcGlobalRequest, SetPinMuxing)  
    ,DEPRECATED_RPC_DEFINITION(IRpcGlobalRequest, ServiceStart)  // this enum has been deprecated as we changed the prototype.  New enum was created below
    ,RPC_DEFINITION(IRpcGlobalRequest, SetInterrupt)
    ,RPC_DEFINITION(IRpcGlobalRequest, ServiceStart)
    ,RPC_DEFINITION(IRpcGnssRequest,   GnssControl)     // LHE<--ESW

    // New for 43018 and a NO-OP for prior chips
    ,RPC_DEFINITION(IRpcGlobalRequest, SetGnssTcxoMode)

    // New for 43018 because of extended package type with RDL was too large to put in GetVersion
    ,RPC_DEFINITION(IRpcGlobalRequest, GetExtVersion)
    ,RPC_DEFINITION(IRpcGlobalResponse, ExtVersionResponse)

    // New for 43018 and a NO-OP for prior chips
    ,RPC_DEFINITION(IRpcGlobalRequest, GnssBlankingConfig)

    ,_LAST_RPC_DEFINITION

    ,_DEBUG_RPC_DEFINITION_BASE = 1024
    ,RPC_DEFINITION(IRpcBREF, ConsoleGets)
    ,RPC_DEFINITION(IRpcGlobalUtils, ConsolePuts)
    ,_LAST_DEBUG_RPC_DEFINITION
};

GlIntU32 GetU32(GlIntU8 *pRpcPayload);