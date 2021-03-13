// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include <mutex>
#include <PowerManager.g.h>
#include <PowerNotificationsPal.h>
#include <wil/resource.h>
#include <wil/result_macros.h>

using PowerEventHandle = winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>;
using eventType = wil::object_without_destructor_on_shutdown<winrt::event<PowerEventHandle>>;

namespace winrt::Microsoft::ProjectReunion::implementation
{
    using EventToken = event_token ;

    

    eventType EnergySaverEvent();
    void EnergySaver_Register();
    void EnergySaver_Unregister();
    void EnergySaver_Update();
    


    struct PowerManager
    {
        PowerManager() = default;

        

        struct PowerFunctionDetails
        {
            //eventType(*event)(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&);
            eventType(*event)();
            void(*registerListener)();
            void(*unregisterListener)();
            void(*getStatus)();
        };

        static winrt::Microsoft::ProjectReunion::EnergySaverStatus EnergySaverStatus();
        static EventToken EnergySaverStatusChanged(PowerEventHandle const&);
        static void EnergySaverStatusChanged(EventToken const&);
        static void EnergySaverStatusChanged_Callback(::EnergySaverStatus);

        static winrt::Microsoft::ProjectReunion::BatteryStatus BatteryStatus();
        static EventToken BatteryStatusChanged(PowerEventHandle const&);
        static void BatteryStatusChanged(EventToken const&);
        static void CompositeBatteryStatusChanged_Callback(CompositeBatteryStatus*);

        static winrt::Microsoft::ProjectReunion::PowerSupplyStatus PowerSupplyStatus();
        static EventToken PowerSupplyStatusChanged(PowerEventHandle const&);
        static void PowerSupplyStatusChanged(EventToken const&);

        static int RemainingChargePercent();
        static EventToken RemainingChargePercentChanged(PowerEventHandle const&);
        static void RemainingChargePercentChanged(EventToken const&);
        
        static Windows::Foundation::TimeSpan RemainingDischargeTime();
        static EventToken RemainingDischargeTimeChanged(PowerEventHandle const&);
        static void RemainingDischargeTimeChanged(EventToken const&);
        static void RemainingDischargeTimeChanged_Callback(ULONGLONG);

        static winrt::Microsoft::ProjectReunion::PowerSourceStatus PowerSourceStatus();
        static EventToken PowerSourceStatusChanged(PowerEventHandle const&);
        static void PowerSourceStatusChanged(EventToken const&);
        static void PowerSourceStatusChanged_Callback(DWORD);

        static winrt::Microsoft::ProjectReunion::DisplayStatus DisplayStatus();
        static EventToken DisplayStatusChanged(PowerEventHandle const&);
        static void DisplayStatusChanged(EventToken const&);
        static void DisplayStatusChanged_Callback(DWORD);

        static winrt::Microsoft::ProjectReunion::SystemIdleStatus SystemIdleStatus();
        static EventToken SystemIdleStatusChanged(PowerEventHandle const&);
        static void SystemIdleStatusChanged(EventToken const&);
        static void SystemIdleStatusChanged_Callback();

        static winrt::Microsoft::ProjectReunion::PowerSchemePersonality PowerSchemePersonality();
        static EventToken PowerSchemePersonalityChanged(PowerEventHandle const&);
        static void PowerSchemePersonalityChanged(EventToken const&);
        static void PowerSchemePersonalityChanged_Callback(GUID);

        static winrt::Microsoft::ProjectReunion::UserPresenceStatus UserPresenceStatus();
        static EventToken UserPresenceStatusChanged(PowerEventHandle const&);
        static void UserPresenceStatusChanged(EventToken const&);
        static void UserPresenceStatusChanged_Callback(DWORD);

        static winrt::Microsoft::ProjectReunion::SystemAwayModeStatus SystemAwayModeStatus();
        static EventToken SystemAwayModeStatusChanged(PowerEventHandle const&);
        static void SystemAwayModeStatusChanged(EventToken const&);
        static void SystemAwayModeStatusChanged_Callback(DWORD);

        enum PowerFunction
        {
            EnergySaverStatusFn = 0,
            BatteryStatusFn,
            PowerSupplyStatusFn,
            RemainingChargePercentFn,
            RemainingDischargeTimeFn,
            PowerSourceStatusFn,
            DisplayStatusFn,
            SystemIdleStatusFn,
            PowerSchemePersonalityFn,
            UserPresenceStatusFn,
            SystemAwayModeStatusFn
        };

        private:
        // REVIEW: leaks during ExitProcess, to avoid making cross-binary calls. Probably insufficient if we need to tolerate final CoUninitialize scenarios.
        // Ultimately depends on whether Reunion binaries are persistently kept loaded by something other than COM.   

        static bool       NotAlreadyRegisteredForEvents(eventType);
        static eventType  GetEventObj(PowerFunction const&);        
        static EventToken AddCallback(PowerFunction const&, PowerEventHandle const&);
        static void RegisterListener (PowerFunction const&);
        static void RemoveCallback   (PowerFunction const&, EventToken const&);
        static void UnregisterListener(PowerFunction const&);
        static void CheckRegistrationAndOrUpdateValue(PowerFunction const&);
        static void FireEvent(PowerFunction const&);
        static void FireCorrespondingBatteryEvent();
        static void ProcessCompositeBatteryStatus(CompositeBatteryStatus const&);

        static EventToken AddCallback(PowerFunctionDetails fn, PowerEventHandle const&);

        PowerFunctionDetails EnergySaverStatusFunc{ &EnergySaverEvent, &EnergySaver_Register, &EnergySaver_Unregister, &EnergySaver_Update };

    };
}

namespace winrt::Microsoft::ProjectReunion::factory_implementation
{
    struct PowerManager : PowerManagerT<PowerManager, implementation::PowerManager, static_lifetime>
    {
        eventType m_energySaverStatusChangedEvent;
        eventType m_batteryStatusChangedEvent;
        eventType m_powerSupplyStatusChangedEvent;
        eventType m_remainingChargePercentChangedEvent;
        eventType m_remainingDischargeTimeChangedEvent;

        eventType m_powerSourceStatusChangedEvent;
        eventType m_displayStatusChangedEvent;
        eventType m_systemIdleStatusChangedEvent;
        eventType m_powerSchemePersonalityChangedEvent;
        eventType m_userPresenceStatusChangedEvent;
        eventType m_systemAwayModeStatusChangedEvent;

        EnergySaverStatusRegistration      m_energySaverStatusHandle;
        CompositeBatteryStatusRegistration m_batteryStatusHandle;
        PowerConditionRegistration         m_powerConditionHandle;
        DischargeTimeRegistration          m_dischargeTimeHandle;
        DisplayStatusRegistration          m_displayStatusHandle;
        SystemIdleStatusRegistration       m_systemIdleStatusHandle;
        PowerSchemePersonalityRegistration m_powerSchemePersonalityHandle;
        UserPresenceStatusRegistration     m_userPresenceStatusHandle;
        SystemAwayModeStatusRegistration   m_systemAwayModeStatusHandle;

        std::mutex m_mutex;

        int   m_batteryChargePercent;
        int   m_oldBatteryChargePercent;
        DWORD m_cachedDisplayStatus;
        DWORD m_cachedUserPresenceStatus;
        DWORD m_cachedSystemAwayModeStatus;
        DWORD m_cachedPowerCondition;
        GUID  m_cachedPowerSchemePersonality;
        ULONGLONG              m_cachedDischargeTime;
        ::EnergySaverStatus    m_cachedEnergySaverStatus;
        CompositeBatteryStatus* m_cachedCompositeBatteryStatus;
        winrt::Microsoft::ProjectReunion::BatteryStatus m_batteryStatus;
        winrt::Microsoft::ProjectReunion::BatteryStatus m_oldBatteryStatus;
        winrt::Microsoft::ProjectReunion::PowerSupplyStatus m_powerSupplyStatus;
        winrt::Microsoft::ProjectReunion::PowerSupplyStatus m_oldPowerSupplyStatus;
    };
}
