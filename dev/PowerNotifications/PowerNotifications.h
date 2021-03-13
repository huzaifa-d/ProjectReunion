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

// Forward-declarations
namespace winrt::Microsoft::ProjectReunion::implementation
{
    struct PowerManager;
    struct PowerFunctionDetails
    {
        eventType(*event)();
        void(*registerListener)();
        void(*unregisterListener)();
        void(*getStatus)();
    };
}

namespace winrt::Microsoft::ProjectReunion::factory_implementation
{
    using PowerFunctionDetails = winrt::Microsoft::ProjectReunion::implementation::PowerFunctionDetails;

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

        bool NotAlreadyRegisteredForEvents(eventType eventObj)
        {
            return !eventObj.get();
        }

        event_token AddCallback(PowerFunctionDetails fn, PowerEventHandle const& handler)
        {
            auto eventObj = fn.event();
            std::scoped_lock<std::mutex> lock(m_mutex);

            //Check prior registrations
            if (NotAlreadyRegisteredForEvents(eventObj))
            {
                fn.registerListener();
            }
            eventObj.get().add(handler);
        }

        void RemoveCallback(PowerFunctionDetails fn, event_token const& token)
        {
            auto eventObj = fn.event();
            eventObj.get().remove(token);
            std::scoped_lock<std::mutex> lock(m_mutex);
            //Check prior registrations
            //Review: Fix this
            if (!NotAlreadyRegisteredForEvents(eventObj))
            {
                fn.unregisterListener();
            }
        }

        void RaiseEvent(PowerFunctionDetails fn)
        {
            //fn.event()(nullptr, nullptr);
        }

        winrt::Microsoft::ProjectReunion::EnergySaverStatus EnergySaverStatus()
        {
            //CheckRegistrationAndOrUpdateValue(EnergySaverStatusFn);
            return static_cast<winrt::Microsoft::ProjectReunion::EnergySaverStatus>(m_cachedEnergySaverStatus);
        }

        event_token EnergySaverStatusChanged(PowerEventHandle const& handler)
        {
            //return AddCallback(EnergySaverStatusFn, handler);
        }

        void EnergySaverStatusChanged(event_token const& token)
        {
            //RemoveCallback(EnergySaverStatusFn, token);
        }

        void EnergySaverStatusChanged_Callback(::EnergySaverStatus energySaverStatus)
        {
            //stats->m_cachedEnergySaverStatus = energySaverStatus;
            //FireEvent(EnergySaverStatusFn);
        }

    };
}

namespace winrt::Microsoft::ProjectReunion::implementation
{
    auto stats = make_self<factory_implementation::PowerManager>();    

    eventType EnergySaverEvent();
    void EnergySaver_Register();
    void EnergySaver_Unregister();
    void EnergySaver_Update();
    
     struct PowerManager
    {
        PowerManager() = default;        




        static winrt::Microsoft::ProjectReunion::EnergySaverStatus EnergySaverStatus()
        {
            return stats->EnergySaverStatus();
        }

        static winrt::Microsoft::ProjectReunion::BatteryStatus BatteryStatus();
        static event_token BatteryStatusChanged(PowerEventHandle const&);
        static void BatteryStatusChanged(event_token const&);
        static void CompositeBatteryStatusChanged_Callback(CompositeBatteryStatus*);

        static winrt::Microsoft::ProjectReunion::PowerSupplyStatus PowerSupplyStatus();
        static event_token PowerSupplyStatusChanged(PowerEventHandle const&);
        static void PowerSupplyStatusChanged(event_token const&);

        static int RemainingChargePercent();
        static event_token RemainingChargePercentChanged(PowerEventHandle const&);
        static void RemainingChargePercentChanged(event_token const&);
        
        static Windows::Foundation::TimeSpan RemainingDischargeTime();
        static event_token RemainingDischargeTimeChanged(PowerEventHandle const&);
        static void RemainingDischargeTimeChanged(event_token const&);
        static void RemainingDischargeTimeChanged_Callback(ULONGLONG);

        static winrt::Microsoft::ProjectReunion::PowerSourceStatus PowerSourceStatus();
        static event_token PowerSourceStatusChanged(PowerEventHandle const&);
        static void PowerSourceStatusChanged(event_token const&);
        static void PowerSourceStatusChanged_Callback(DWORD);

        static winrt::Microsoft::ProjectReunion::DisplayStatus DisplayStatus();
        static event_token DisplayStatusChanged(PowerEventHandle const&);
        static void DisplayStatusChanged(event_token const&);
        static void DisplayStatusChanged_Callback(DWORD);

        static winrt::Microsoft::ProjectReunion::SystemIdleStatus SystemIdleStatus();
        static event_token SystemIdleStatusChanged(PowerEventHandle const&);
        static void SystemIdleStatusChanged(event_token const&);
        static void SystemIdleStatusChanged_Callback();

        static winrt::Microsoft::ProjectReunion::PowerSchemePersonality PowerSchemePersonality();
        static event_token PowerSchemePersonalityChanged(PowerEventHandle const&);
        static void PowerSchemePersonalityChanged(event_token const&);
        static void PowerSchemePersonalityChanged_Callback(GUID);

        static winrt::Microsoft::ProjectReunion::UserPresenceStatus UserPresenceStatus();
        static event_token UserPresenceStatusChanged(PowerEventHandle const&);
        static void UserPresenceStatusChanged(event_token const&);
        static void UserPresenceStatusChanged_Callback(DWORD);

        static winrt::Microsoft::ProjectReunion::SystemAwayModeStatus SystemAwayModeStatus();
        static event_token SystemAwayModeStatusChanged(PowerEventHandle const&);
        static void SystemAwayModeStatusChanged(event_token const&);
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
        static event_token AddCallback(PowerFunction const&, PowerEventHandle const&);
        static void RegisterListener (PowerFunction const&);
        static void RemoveCallback   (PowerFunction const&, event_token const&);
        static void UnregisterListener(PowerFunction const&);
        static void CheckRegistrationAndOrUpdateValue(PowerFunction const&);
        static void FireEvent(PowerFunction const&);
        static void FireCorrespondingBatteryEvent();
        static void ProcessCompositeBatteryStatus(CompositeBatteryStatus const&);

        static event_token AddCallback(PowerFunctionDetails fn, PowerEventHandle const&);

        PowerFunctionDetails EnergySaverStatusFunc{ &EnergySaverEvent, &EnergySaver_Register, &EnergySaver_Unregister, &EnergySaver_Update };

    };
}

