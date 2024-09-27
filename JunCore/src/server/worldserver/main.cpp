#include <iostream>

// common.lib build test
#include <Common.h>
#include <Define.h>

// shared.lib build test
#include <Networking/AsyncAcceptor.h>
#include <Networking/NetworkThread.h>
#include <Networking/Socket.h>
#include <Networking/SocketMgr.h>

// game.lib build test
#include <Server/WorldPacket.h>
#include <Server/WorldSession.h>
#include <Server/WorldSocket.h>
#include <Server/WorldSocketMgr.h>
#include <World/World.h>

using namespace std;

/*
WorldSocketMgr::OnSocketOpen() 구현 필요
*/

void ClearOnlineAccounts();

int main(int argc, char** argv)
{
//    Trinity::Impl::CurrentServerProcessHolder::_type = SERVER_PROCESS_WORLDSERVER;
//    signal(SIGABRT, &Trinity::AbortHandler);
//
//    Trinity::VerifyOsVersion();
//
//    Trinity::Locale::Init();
//
//    auto configFile = fs::absolute(_TRINITY_CORE_CONFIG);
//    auto configDir = fs::absolute(_TRINITY_CORE_CONFIG_DIR);
//    std::string winServiceAction;
//
//    auto vm = GetConsoleArguments(argc, argv, configFile, configDir, winServiceAction);
//    // exit if help or version is enabled
//    if (vm.count("help") || vm.count("version"))
//        return 0;
//
//#ifdef _WIN32
//    if (winServiceAction == "install")
//        return WinServiceInstall() ? 0 : 1;
//    if (winServiceAction == "uninstall")
//        return WinServiceUninstall() ? 0 : 1;
//    if (winServiceAction == "run")
//        return WinServiceRun() ? 0 : 1;
//
//    Optional<UINT> newTimerResolution;
//    boost::system::error_code dllError;
//    std::shared_ptr<boost::dll::shared_library> winmm(new boost::dll::shared_library("winmm.dll", dllError, boost::dll::load_mode::search_system_folders), [&](boost::dll::shared_library* lib)
//        {
//            try
//            {
//                if (newTimerResolution)
//                    lib->get<decltype(timeEndPeriod)>("timeEndPeriod")(*newTimerResolution);
//            }
//            catch (std::exception const&)
//            {
//                // ignore
//            }
//
//            delete lib;
//        });
//
//    if (winmm->is_loaded())
//    {
//        try
//        {
//            auto timeGetDevCapsPtr = winmm->get<decltype(timeGetDevCaps)>("timeGetDevCaps");
//            // setup timer resolution
//            TIMECAPS timeResolutionLimits;
//            if (timeGetDevCapsPtr(&timeResolutionLimits, sizeof(TIMECAPS)) == TIMERR_NOERROR)
//            {
//                auto timeBeginPeriodPtr = winmm->get<decltype(timeBeginPeriod)>("timeBeginPeriod");
//                newTimerResolution = std::min(std::max(timeResolutionLimits.wPeriodMin, 1u), timeResolutionLimits.wPeriodMax);
//                timeBeginPeriodPtr(*newTimerResolution);
//            }
//        }
//        catch (std::exception const& e)
//        {
//            printf("Failed to initialize timer resolution: %s\n", e.what());
//        }
//    }
//
//#endif

//    std::string configError;
//    if (!sConfigMgr->LoadInitial(configFile.generic_string(),
//        std::vector<std::string>(argv, argv + argc),
//        configError))
//    {
//        printf("Error in config file: %s\n", configError.c_str());
//        return 1;
//    }
//
//    std::vector<std::string> loadedConfigFiles;
//    std::vector<std::string> configDirErrors;
//    bool additionalConfigFileLoadSuccess = sConfigMgr->LoadAdditionalDir(configDir.generic_string(), true, loadedConfigFiles, configDirErrors);
//    for (std::string const& loadedConfigFile : loadedConfigFiles)
//        printf("Loaded additional config file %s\n", loadedConfigFile.c_str());
//
//    if (!additionalConfigFileLoadSuccess)
//    {
//        for (std::string const& configDirError : configDirErrors)
//            printf("Error in additional config files: %s\n", configDirError.c_str());
//
//        return 1;
//    }
//
//    std::vector<std::string> overriddenKeys = sConfigMgr->OverrideWithEnvVariablesIfAny();
//
    std::shared_ptr<boost::asio::io_context> ioContext = std::make_shared<boost::asio::io_context>();
//
//    sLog->RegisterAppender<AppenderDB>();
//    // If logs are supposed to be handled async then we need to pass the IoContext into the Log singleton
//    sLog->Initialize(sConfigMgr->GetBoolDefault("Log.Async.Enable", false) ? ioContext.get() : nullptr);
//
//    Trinity::Banner::Show("worldserver-daemon",
//        [](char const* text)
//        {
//            // TC_LOG_INFO("server.worldserver", "{}", text);
//        },
//        []()
//        {
//            // TC_LOG_INFO("server.worldserver", "Using configuration file {}.", sConfigMgr->GetFilename());
//            // TC_LOG_INFO("server.worldserver", "Using SSL version: {} (library: {})", OPENSSL_VERSION_TEXT, OpenSSL_version(OPENSSL_VERSION));
//            // TC_LOG_INFO("server.worldserver", "Using Boost version: {}.{}.{}", BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
//        }
//    );
//
//    for (std::string const& key : overriddenKeys)
//        // TC_LOG_INFO("server.worldserver", "Configuration field '{}' was overridden with environment variable.", key);
//
//    OpenSSLCrypto::threadsSetup(boost::dll::program_location().remove_filename());
//
//    std::shared_ptr<void> opensslHandle(nullptr, [](void*) { OpenSSLCrypto::threadsCleanup(); });
//
//    // Seed the OpenSSL's PRNG here.
//    // That way it won't auto-seed when calling BigNumber::SetRand and slow down the first world login
//    BigNumber seed;
//    seed.SetRand(16 * 8);
//
//    /// worldserver PID file creation
//    std::string pidFile = sConfigMgr->GetStringDefault("PidFile", "");
//    if (!pidFile.empty())
//    {
//        if (uint32 pid = CreatePIDFile(pidFile))
//            // TC_LOG_INFO("server.worldserver", "Daemon PID: {}\n", pid);
//        else
//        {
//            // TC_LOG_ERROR("server.worldserver", "Cannot create PID file {}.\n", pidFile);
//            return 1;
//        }
//    }
//
//    // Set signal handlers (this must be done before starting IoContext threads, because otherwise they would unblock and exit)
//    boost::asio::signal_set signals(*ioContext, SIGINT, SIGTERM);
//#if TRINITY_PLATFORM == TRINITY_PLATFORM_WINDOWS
//    signals.add(SIGBREAK);
//#endif
//    signals.async_wait(SignalHandler);
//
//    // Start the Boost based thread pool
//    int numThreads = sConfigMgr->GetIntDefault("ThreadPool", 1);
//    if (numThreads < 1)
//        numThreads = 1;
//
//    std::shared_ptr<Trinity::ThreadPool> threadPool = std::make_shared<Trinity::ThreadPool>(numThreads);
//
//    for (int i = 0; i < numThreads; ++i)
//        threadPool->PostWork([ioContext]() { ioContext->run(); });
//
//    std::shared_ptr<void> ioContextStopHandle(nullptr, [ioContext](void*) { ioContext->stop(); });
//
//    // Set process priority according to configuration settings
//    SetProcessPriority("server.worldserver", sConfigMgr->GetIntDefault(CONFIG_PROCESSOR_AFFINITY, 0), sConfigMgr->GetBoolDefault(CONFIG_HIGH_PRIORITY, false));
//
//    // Start the databases
//    if (!StartDB())
//        return 1;
//
//    std::shared_ptr<void> dbHandle(nullptr, [](void*) { StopDB(); });
//
//    if (vm.count("update-databases-only"))
//        return 0;
//
//    // Set server offline (not connectable)
//    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag | {} WHERE id = '{}'", REALM_FLAG_OFFLINE, realm.Id.Realm);
//
//    LoadRealmInfo(*ioContext);
//
//    sMetric->Initialize(realm.Name, *ioContext, []()
//        {
//            TC_METRIC_VALUE("online_players", sWorld->GetPlayerCount());
//            TC_METRIC_VALUE("db_queue_login", uint64(LoginDatabase.QueueSize()));
//            TC_METRIC_VALUE("db_queue_character", uint64(CharacterDatabase.QueueSize()));
//            TC_METRIC_VALUE("db_queue_world", uint64(WorldDatabase.QueueSize()));
//        });
//
//    TC_METRIC_EVENT("events", "Worldserver started", "");
//
//    std::shared_ptr<void> sMetricHandle(nullptr, [](void*)
//        {
//            TC_METRIC_EVENT("events", "Worldserver shutdown", "");
//            sMetric->Unload();
//        });
//
//    sScriptMgr->SetScriptLoader(AddScripts);
//    std::shared_ptr<void> sScriptMgrHandle(nullptr, [](void*)
//        {
//            sScriptMgr->Unload();
//            sScriptReloadMgr->Unload();
//        });
//
//    // Initialize the World
//    sSecretMgr->Initialize();
//    sWorld->SetInitialWorldSettings();
//
//    std::shared_ptr<void> mapManagementHandle(nullptr, [](void*)
//        {
//            // unload battleground templates before different singletons destroyed
//            sBattlegroundMgr->DeleteAllBattlegrounds();
//
//            sInstanceSaveMgr->Unload();
//            sOutdoorPvPMgr->Die();                     // unload it before MapManager
//            sMapMgr->UnloadAll();                      // unload all grids (including locked in memory)
//        });
//
//    // Start the Remote Access port (acceptor) if enabled
//    std::unique_ptr<AsyncAcceptor> raAcceptor;
//    if (sConfigMgr->GetBoolDefault("Ra.Enable", false))
//        raAcceptor.reset(StartRaSocketAcceptor(*ioContext));
//
//    // Start soap serving thread if enabled
//    std::shared_ptr<std::thread> soapThread;
//    if (sConfigMgr->GetBoolDefault("SOAP.Enabled", false))
//    {
//        soapThread.reset(new std::thread(TCSoapThread, sConfigMgr->GetStringDefault("SOAP.IP", "127.0.0.1"), uint16(sConfigMgr->GetIntDefault("SOAP.Port", 7878))),
//            [](std::thread* thr)
//            {
//                thr->join();
//                delete thr;
//            });
//    }
//

    // Launch the worldserver listener socket
    uint16 worldPort = 8085;                //  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));
    std::string worldListener = "0.0.0.0";  // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");

    int32 networkThreads = 1;               // sConfigMgr->GetIntDefault("Network.Threads", 1);

    //if (networkThreads <= 0)
    //{
    //    // TC_LOG_ERROR("server.worldserver", "Network.Threads must be greater than 0");
    //    World::StopNow(ERROR_EXIT_CODE);
    //    return 1;
    //}

    if (!sWorldSocketMgr.StartWorldNetwork(*ioContext, worldListener, worldPort, networkThreads))
    {
        // todo error log
         World::StopNow(ERROR_EXIT_CODE);
        return 1;
    }

    std::shared_ptr<void> sWorldSocketMgrHandle(nullptr
        , [](void*) {
            sWorld->KickAll();              // save and kick all players
            sWorld->UpdateSessions(1);      // real players unload required UpdateSessions call

            sWorldSocketMgr.StopNetwork();

            //- Clean database before leaving
            ClearOnlineAccounts();
        }
    );

//    // Set server online (allow connecting now)
//    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag & ~{}, population = 0 WHERE id = '{}'", REALM_FLAG_OFFLINE, realm.Id.Realm);
//    realm.PopulationLevel = 0.0f;
//    realm.Flags = RealmFlags(realm.Flags & ~uint32(REALM_FLAG_OFFLINE));
//
//    // Start the freeze check callback cycle in 5 seconds (cycle itself is 1 sec)
//    std::shared_ptr<FreezeDetector> freezeDetector;
//    if (int coreStuckTime = sConfigMgr->GetIntDefault("MaxCoreStuckTime", 60))
//    {
//        freezeDetector = std::make_shared<FreezeDetector>(*ioContext, coreStuckTime * 1000);
//        FreezeDetector::Start(freezeDetector);
//        // TC_LOG_INFO("server.worldserver", "Starting up anti-freeze thread ({} seconds max stuck time)...", coreStuckTime);
//    }
//
//    // TC_LOG_INFO("server.worldserver", "{} (worldserver-daemon) ready...", GitRevision::GetFullVersion());
//
//    sScriptMgr->OnStartup();
//
//    // Launch CliRunnable thread
//    std::shared_ptr<std::thread> cliThread;
//#ifdef _WIN32
//    if (sConfigMgr->GetBoolDefault("Console.Enable", true) && (m_ServiceStatus == -1)/* need disable console in service mode*/)
//#else
//    if (sConfigMgr->GetBoolDefault("Console.Enable", true))
//#endif
//    {
//        cliThread.reset(new std::thread(CliThread), &ShutdownCLIThread);
//    }
//
//    WorldUpdateLoop();
//
//    // Shutdown starts here
//    ioContextStopHandle.reset();
//
//    threadPool.reset();
//
//    sLog->SetSynchronous();
//
//    sScriptMgr->OnShutdown();
//
//    // set server offline
//    LoginDatabase.DirectPExecute("UPDATE realmlist SET flag = flag | {} WHERE id = '{}'", REALM_FLAG_OFFLINE, realm.Id.Realm);
//
//    // TC_LOG_INFO("server.worldserver", "Halting process...");

    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart Trinityd

    // return World::GetExitCode();
    return 0;
}

void ClearOnlineAccounts()
{
    //// Reset online status for all accounts with characters on the current realm
    //LoginDatabase.DirectPExecute("UPDATE account SET online = 0 WHERE online > 0 AND id IN (SELECT acctid FROM realmcharacters WHERE realmid = {})", realm.Id.Realm);

    //// Reset online status for all characters
    //CharacterDatabase.DirectExecute("UPDATE characters SET online = 0 WHERE online <> 0");

    //// Battleground instance ids reset at server restart
    //CharacterDatabase.DirectExecute("UPDATE character_battleground_data SET instanceId = 0");
}