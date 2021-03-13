// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <pch.h>
#include <PowerNotifications.h>
#include <PowerNotificationsPal.h>
#include <PowerManager.g.cpp>

namespace winrt::Microsoft::ProjectReunion::implementation
{

    

    //stats->m_oldBatteryStatus = winrt::Microsoft::ProjectReunion::BatteryStatus::NotPresent;
    //stats->m_oldPowerSupplyStatus = winrt::Microsoft::ProjectReunion::PowerSupplyStatus::NotPresent;



    eventType PowerManager::GetEventObj(PowerFunction const& fn)
    {
        switch (fn) {
            case EnergySaverStatusFn:      return stats->m_energySaverStatusChangedEvent;
            case BatteryStatusFn:          return stats->m_batteryStatusChangedEvent;
            case PowerSupplyStatusFn:      return stats->m_powerSupplyStatusChangedEvent;
            case RemainingChargePercentFn: return stats->m_remainingChargePercentChangedEvent;
            case RemainingDischargeTimeFn: return stats->m_remainingDischargeTimeChangedEvent;
            case PowerSourceStatusFn:      return stats->m_powerSourceStatusChangedEvent;
            case DisplayStatusFn:          return stats->m_displayStatusChangedEvent;
            case SystemIdleStatusFn:       return stats->m_systemIdleStatusChangedEvent;
            case PowerSchemePersonalityFn: return stats->m_powerSchemePersonalityChangedEvent;
            case UserPresenceStatusFn:     return stats->m_userPresenceStatusChangedEvent;
            case SystemAwayModeStatusFn:   return stats->m_systemAwayModeStatusChangedEvent;
            default: throw E_NOTIMPL;
        }
    }

    void PowerManager::RegisterListener(PowerFunction const& fn)
    {
        switch (fn) {        
            case BatteryStatusFn:          
            case PowerSupplyStatusFn:      
            case RemainingChargePercentFn: check_hresult(RegisterCompositeBatteryStatusChangedListener(CompositeBatteryStatusChanged_Callback, &stats->m_batteryStatusHandle)); break;
            case EnergySaverStatusFn:       break;
            case RemainingDischargeTimeFn: check_hresult(RegisterDischargeTimeChangedListener(RemainingDischargeTimeChanged_Callback, &stats->m_dischargeTimeHandle));    break;
            case PowerSourceStatusFn:      check_hresult(RegisterPowerConditionChangedListener(PowerSourceStatusChanged_Callback, &stats->m_powerConditionHandle));       break;
            case DisplayStatusFn:          check_hresult(RegisterDisplayStatusChangedListener(DisplayStatusChanged_Callback, &stats->m_displayStatusHandle));             break;
            case SystemIdleStatusFn:       check_hresult(RegisterSystemIdleStatusChangedListener(SystemIdleStatusChanged_Callback, &stats->m_systemIdleStatusHandle));    break;
            case PowerSchemePersonalityFn: check_hresult(RegisterPowerSchemePersonalityChangedListener(PowerSchemePersonalityChanged_Callback, &stats->m_powerSchemePersonalityHandle)); break;
            case UserPresenceStatusFn:     check_hresult(RegisterUserPresenceStatusChangedListener(UserPresenceStatusChanged_Callback, &stats->m_userPresenceStatusHandle));             break;
            case SystemAwayModeStatusFn:   check_hresult(RegisterSystemAwayModeStatusChangedListener(SystemAwayModeStatusChanged_Callback, &stats->m_systemAwayModeStatusHandle));       break;
            default: throw E_NOTIMPL;
        }
    }

    void PowerManager::UnregisterListener(PowerFunction const& fn)
    {
        switch (fn) {        
            case BatteryStatusFn:          
            case PowerSupplyStatusFn:      
            case RemainingChargePercentFn: check_hresult(UnregisterCompositeBatteryStatusChangedListener(stats->m_batteryStatusHandle)); break;
            case EnergySaverStatusFn:       break;
            case RemainingDischargeTimeFn: check_hresult(UnregisterDischargeTimeChangedListener(stats->m_dischargeTimeHandle));          break;
            case PowerSourceStatusFn:      check_hresult(UnregisterPowerConditionChangedListener(stats->m_powerConditionHandle));        break;
            case DisplayStatusFn:          check_hresult(UnregisterDisplayStatusChangedListener(stats->m_displayStatusHandle));          break;
            case SystemIdleStatusFn:       check_hresult(UnregisterSystemIdleStatusChangedListener(stats->m_systemIdleStatusHandle));    break;
            case PowerSchemePersonalityFn: check_hresult(UnregisterPowerSchemePersonalityChangedListener(stats->m_powerSchemePersonalityHandle)); break;
            case UserPresenceStatusFn:     check_hresult(UnregisterUserPresenceStatusChangedListener(stats->m_userPresenceStatusHandle));         break;
            case SystemAwayModeStatusFn:   check_hresult(UnregisterSystemAwayModeStatusChangedListener(stats->m_systemAwayModeStatusHandle));     break;
            default: throw E_NOTIMPL;
        }
    }


    //Checks if an event is already registered. If none are, then gets the status and caches it
    void PowerManager::CheckRegistrationAndOrUpdateValue(PowerFunction const& fn)
    {
        auto eventObj = GetEventObj(fn);
        if (NotAlreadyRegisteredForEvents(eventObj))
        {
            switch (fn) {
                case EnergySaverStatusFn:
                    
                    break;

                case BatteryStatusFn:
                    check_hresult(GetCompositeBatteryStatus(&stats->m_cachedCompositeBatteryStatus));
                    ProcessCompositeBatteryStatus(*stats->m_cachedCompositeBatteryStatus);
                    break;

                case PowerSupplyStatusFn:
                    break;

                case RemainingChargePercentFn:
                    break;

                case RemainingDischargeTimeFn:
                    check_hresult(GetDischargeTime(&stats->m_cachedDischargeTime));
                    break;

                case PowerSourceStatusFn:
                    check_hresult(GetPowerCondition(&stats->m_cachedPowerCondition));
                    break;

                case DisplayStatusFn:
                    check_hresult(GetDisplayStatus(&stats->m_cachedDisplayStatus));
                    break;

                case SystemIdleStatusFn:
                    break;

                case PowerSchemePersonalityFn:
                    check_hresult(GetPowerSchemePersonality(&stats->m_cachedPowerSchemePersonality));
                    break;

                case UserPresenceStatusFn:
                    check_hresult(GetUserPresenceStatus(&stats->m_cachedUserPresenceStatus));
                    break;

                case SystemAwayModeStatusFn:
                    check_hresult(GetSystemAwayModeStatus(&stats->m_cachedSystemAwayModeStatus));
                    break;

                default: throw E_NOTIMPL;
            }
        }

    }



    eventType EnergySaverEvent()
    {
        return stats->m_energySaverStatusChangedEvent;
    }

    void EnergySaver_Register()
    {
        //check_hresult(RegisterEnergySaverStatusChangedListener(PowerManager::EnergySaverStatusChanged_Callback, &stats->m_energySaverStatusHandle));
    }

    void EnergySaver_Unregister()
    {
        check_hresult(UnregisterEnergySaverStatusChangedListener(stats->m_energySaverStatusHandle));
    }

    void EnergySaver_Update()
    {
        check_hresult(GetEnergySaverStatus(&stats->m_cachedEnergySaverStatus));
    }



    void PowerManager::ProcessCompositeBatteryStatus(CompositeBatteryStatus const& compositeBatteryStatus)
    {
        //Calculate battery charge
        auto fullChargedCapacity = compositeBatteryStatus.Information.FullChargedCapacity;
        auto remainingCapacity = compositeBatteryStatus.Status.Capacity;
        auto newRemainingChargePercent = static_cast<int>((remainingCapacity * 200) / fullChargedCapacity);
        newRemainingChargePercent += 1;
        newRemainingChargePercent /= 2;
        stats->m_batteryChargePercent = newRemainingChargePercent;

        auto powerState = compositeBatteryStatus.Status.PowerState;

        //Set battery status
        if (compositeBatteryStatus.ActiveBatteryCount == 0)
        {
            stats->m_batteryStatus = BatteryStatus::NotPresent;
        }
        else if ((powerState & BATTERY_DISCHARGING) != FALSE)
        {
            stats->m_batteryStatus = BatteryStatus::Discharging;
        }
        else if ((powerState & BATTERY_CHARGING) != FALSE)
        {
            stats->m_batteryStatus = BatteryStatus::Charging;
        }
        else
        {
            stats->m_batteryStatus = BatteryStatus::Idle;
        }
                
        //Set power supply state
        if ((powerState & BATTERY_POWER_ON_LINE) == FALSE)
        {
            stats->m_powerSupplyStatus = PowerSupplyStatus::NotPresent;
        }
        else if ((powerState & BATTERY_DISCHARGING) != FALSE)
        {
            stats->m_powerSupplyStatus = PowerSupplyStatus::Inadequate;
        }
        else
        {
            stats->m_powerSupplyStatus = PowerSupplyStatus::Adequate;
        }
    }

    void PowerManager::FireCorrespondingBatteryEvent()
    {
        if (stats->m_oldBatteryChargePercent != stats->m_batteryChargePercent)
        {
            stats->m_oldBatteryChargePercent = stats->m_batteryChargePercent;
            //FireEvent(RemainingChargePercentFn);
        }

        if (stats->m_oldBatteryStatus != stats->m_batteryStatus)
        {
            stats->m_oldBatteryStatus = stats->m_batteryStatus;
            //FireEvent(BatteryStatusFn);
        }

        if (stats->m_oldPowerSupplyStatus != stats->m_powerSupplyStatus)
        {
            stats->m_oldPowerSupplyStatus = stats->m_powerSupplyStatus;
            //FireEvent(PowerSupplyStatusFn);
        }
    }

    winrt::Microsoft::ProjectReunion::BatteryStatus PowerManager::BatteryStatus()
    {
        CheckRegistrationAndOrUpdateValue(BatteryStatusFn);
        return stats->m_batteryStatus;        
    }

    event_token PowerManager::BatteryStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(BatteryStatusFn, handler);
    }

    void PowerManager::BatteryStatusChanged(event_token const& token)
    {
        //RemoveCallback(BatteryStatusFn, token);
    }

    void PowerManager::CompositeBatteryStatusChanged_Callback(CompositeBatteryStatus* compositeBatteryStatus)
    {
        ProcessCompositeBatteryStatus(*compositeBatteryStatus);
        FireCorrespondingBatteryEvent();
    } 

    PowerSupplyStatus PowerManager::PowerSupplyStatus()
    {
        CheckRegistrationAndOrUpdateValue(BatteryStatusFn);
        return stats->m_powerSupplyStatus;
    }

    event_token PowerManager::PowerSupplyStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(PowerSupplyStatusFn, handler);
    }

    void PowerManager::PowerSupplyStatusChanged(event_token const& token)
    {
        //RemoveCallback(PowerSupplyStatusFn, token);
    }

    int PowerManager::RemainingChargePercent()
    {
        CheckRegistrationAndOrUpdateValue(BatteryStatusFn);
        return stats->m_batteryChargePercent;
    }

    event_token PowerManager::RemainingChargePercentChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(RemainingChargePercentFn, handler);
    }

    void PowerManager::RemainingChargePercentChanged(event_token const& token)
    {
        //RemoveCallback(RemainingChargePercentFn, token);
    }

    Windows::Foundation::TimeSpan PowerManager::RemainingDischargeTime()
    {
        CheckRegistrationAndOrUpdateValue(RemainingDischargeTimeFn);
        return Windows::Foundation::TimeSpan(std::chrono::seconds(stats->m_cachedDischargeTime));
    }

    event_token PowerManager::RemainingDischargeTimeChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(RemainingDischargeTimeFn, handler);
    }

    void PowerManager::RemainingDischargeTimeChanged(event_token const& token)
    {
        //RemoveCallback(RemainingDischargeTimeFn, token);
    }

    void PowerManager::RemainingDischargeTimeChanged_Callback(ULONGLONG dischargeTime)
    {
        stats->m_cachedDischargeTime = dischargeTime;
        //FireEvent(RemainingDischargeTimeFn);
    }

    PowerSourceStatus PowerManager::PowerSourceStatus()
    {
        CheckRegistrationAndOrUpdateValue(PowerSourceStatusFn);
        return  static_cast<winrt::Microsoft::ProjectReunion::PowerSourceStatus>(stats->m_cachedPowerCondition);
    }

    event_token PowerManager::PowerSourceStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(PowerSourceStatusFn, handler);
    }

    void PowerManager::PowerSourceStatusChanged(event_token const& token)
    {
        //RemoveCallback(PowerSourceStatusFn, token);
    }

    void PowerManager::PowerSourceStatusChanged_Callback(DWORD powerCondition)
    {
        stats->m_cachedPowerCondition = powerCondition;
        //FireEvent(PowerSourceStatusFn);
    }

    DisplayStatus PowerManager::DisplayStatus()
    {
        CheckRegistrationAndOrUpdateValue(DisplayStatusFn);
        return static_cast<winrt::Microsoft::ProjectReunion::DisplayStatus>(stats->m_cachedDisplayStatus);
    }

    event_token PowerManager::DisplayStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(DisplayStatusFn, handler);
    }

    void PowerManager::DisplayStatusChanged(event_token const& token)
    {
        //RemoveCallback(DisplayStatusFn, token);
    }

    void PowerManager::DisplayStatusChanged_Callback(DWORD displayStatus)
    {
        stats->m_cachedDisplayStatus = displayStatus;
        //FireEvent(DisplayStatusFn);
    }

    SystemIdleStatus PowerManager::SystemIdleStatus()
    {
        //PReview: Should this be the default value?
        return SystemIdleStatus::Busy;
    }

    event_token PowerManager::SystemIdleStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(SystemIdleStatusFn, handler);
    }

    void PowerManager::SystemIdleStatusChanged(event_token const& token)
    {
        //RemoveCallback(SystemIdleStatusFn, token);
    }

    void PowerManager::SystemIdleStatusChanged_Callback()
    {
        //FireEvent(SystemIdleStatusFn);
    }

    PowerSchemePersonality PowerManager::PowerSchemePersonality()
    {
        CheckRegistrationAndOrUpdateValue(PowerSchemePersonalityFn);
        if (stats->m_cachedPowerSchemePersonality == GUID_MAX_POWER_SAVINGS)
        {
            return PowerSchemePersonality::PowerSaver;
        }
        else if (stats->m_cachedPowerSchemePersonality == GUID_MIN_POWER_SAVINGS)
        {
            return PowerSchemePersonality::HighPerformance;
        }
        else
        {
            return PowerSchemePersonality::Balanced;
        }        
    }

    event_token PowerManager::PowerSchemePersonalityChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(PowerSchemePersonalityFn, handler);
    }

    void PowerManager::PowerSchemePersonalityChanged(event_token const& token)
    {
        //RemoveCallback(PowerSchemePersonalityFn, token);
    }

    void PowerManager::PowerSchemePersonalityChanged_Callback(GUID powerSchemePersonality)
    {       
        stats->m_cachedPowerSchemePersonality = powerSchemePersonality;
        //FireEvent(PowerSchemePersonalityFn);
    }

    UserPresenceStatus PowerManager::UserPresenceStatus()
    {
        CheckRegistrationAndOrUpdateValue(UserPresenceStatusFn);
        return static_cast<winrt::Microsoft::ProjectReunion::UserPresenceStatus>(stats->m_cachedUserPresenceStatus);
    }

    event_token PowerManager::UserPresenceStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(UserPresenceStatusFn, handler);
    }

    void PowerManager::UserPresenceStatusChanged(event_token const& token)
    {
        //RemoveCallback(UserPresenceStatusFn, token);
    }

    void PowerManager::UserPresenceStatusChanged_Callback(DWORD userPresenceStatus)
    {
        stats->m_cachedUserPresenceStatus = userPresenceStatus;
        //FireEvent(UserPresenceStatusFn);
    }

    SystemAwayModeStatus PowerManager::SystemAwayModeStatus()
    {
        CheckRegistrationAndOrUpdateValue(SystemAwayModeStatusFn);
        return static_cast<winrt::Microsoft::ProjectReunion::SystemAwayModeStatus>(stats->m_cachedSystemAwayModeStatus);
    }

    event_token PowerManager::SystemAwayModeStatusChanged(PowerEventHandle const& handler)
    {
        //return AddCallback(SystemAwayModeStatusFn, handler); 
    }

    void PowerManager::SystemAwayModeStatusChanged(event_token const& token)
    {
        //RemoveCallback(SystemAwayModeStatusFn, token);
    }

    void PowerManager::SystemAwayModeStatusChanged_Callback(DWORD systemAwayModeStatus)
    {        
        stats->m_cachedSystemAwayModeStatus = systemAwayModeStatus;
        //FireEvent(SystemAwayModeStatusFn);
    }

}
