#ifndef _EVENT_PROPERTIES_H_
#define _EVENT_PROPERTIES_H_

#include <zl-util/pch.h>
#include <zl-util/STLMap.h>

namespace FMODDesigner
{
    struct EventProperties
    {
        typedef STLMap<STLString, int> ParamsType;

        EventProperties(int projectId = -1, int id = -1, STLString name = "", const ParamsType& aParams = ParamsType()) :
        m_projectId(projectId),           
            m_id(id),
            m_is3D(false),
            m_headRelative(false),
            m_loops(false),
            m_duration(0.f),
            m_unloadOnSilence(false),  
            m_retriggerRadius(-1.f),
            m_cutoffRadius(-1.f),
            m_maxRetriggerInstances(1),
            m_minRetriggerTime(0.f),
            m_soundDelayTime(0.f),
            m_aParams(aParams),
            m_name(name)
        {
        }

        int m_projectId;    
        int m_id;
        bool m_is3D;
        bool m_headRelative;
        bool m_loops;
        bool m_unloadOnSilence;   
        float m_duration;
        float m_retriggerRadius;
        float m_cutoffRadius;
        int m_maxRetriggerInstances;
        float m_minRetriggerTime;
        float m_soundDelayTime;
        vector<STLString> m_categoriesToDuck;
        vector<float> m_duckingVolumes;        
        ParamsType m_aParams;
        STLString m_name;
    };
};

#endif