#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"
#include "ae_ti.h"
#include "awb_ti.h"
#include "TI_aewb.h"

static void TIAE_config_flicker_none(IAE_DynamicParams * aeDynamicParams, int min_exp)
{
    int i = 0;
    aeDynamicParams->exposureTimeRange[i].min = min_exp;
    aeDynamicParams->exposureTimeRange[i].max = 33333;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 16000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 8191;
    i++;

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = min_exp;

    return;
}

static void TIAE_config_flicker_yes(IAE_DynamicParams * aeDynamicParams, int min_exp, int step_size)
{
    int i = 0;
    if (min_exp < step_size)
    {
        aeDynamicParams->exposureTimeRange[i].min = min_exp;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;

        aeDynamicParams->exposureTimeRange[i].min = step_size;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 2000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }
    else
    {
        aeDynamicParams->exposureTimeRange[i].min = step_size;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1000;
        aeDynamicParams->ipipeGainRange[i].min = 256;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
        aeDynamicParams->exposureTimeRange[i].min = step_size;
        aeDynamicParams->exposureTimeRange[i].max = step_size;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 2000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;
    }

    aeDynamicParams->exposureTimeRange[i].min = step_size * 2;
    aeDynamicParams->exposureTimeRange[i].max = step_size * 2;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1500;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;
    i++;

    if (step_size == 10000)
    {
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 125000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 4095;
        i++;
    }
    else //(step_size == 8333)
    {
        aeDynamicParams->exposureTimeRange[i].min = step_size * 3;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 3;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 1333;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 1024;
        i++;

        aeDynamicParams->exposureTimeRange[i].min = step_size * 4;
        aeDynamicParams->exposureTimeRange[i].max = step_size * 4;
        aeDynamicParams->apertureLevelRange[i].min = 1;
        aeDynamicParams->apertureLevelRange[i].max = 1;
        aeDynamicParams->sensorGainRange[i].min = 1000;
        aeDynamicParams->sensorGainRange[i].max = 125000;
        aeDynamicParams->ipipeGainRange[i].min = 1024;
        aeDynamicParams->ipipeGainRange[i].max = 8191;
        i++;
    }

    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;

    return;
}

static void TIAE_config_fixed(IAE_DynamicParams * aeDynamicParams, int exp)
{
    int i;
    aeDynamicParams->exposureTimeRange[i].min = exp;
    aeDynamicParams->exposureTimeRange[i].max = exp;
    aeDynamicParams->apertureLevelRange[i].min = 1;
    aeDynamicParams->apertureLevelRange[i].max = 1;
    aeDynamicParams->sensorGainRange[i].min = 1000;
    aeDynamicParams->sensorGainRange[i].max = 1000;
    aeDynamicParams->ipipeGainRange[i].min = 1024;
    aeDynamicParams->ipipeGainRange[i].max = 1024;
    i++;
    aeDynamicParams->numRanges = i;
    aeDynamicParams->exposureTimeStepSize = 1;
    return;
}

void TI_2A_AE_config(IAE_DynamicParams * aeDynamicParams, int min_exp, int step_size)
{
    aeDynamicParams->size = sizeof(*aeDynamicParams);
    aeDynamicParams->targetBrightnessRange.min = 40;
    aeDynamicParams->targetBrightnessRange.max = 50;
    aeDynamicParams->targetBrightness = 45;
    aeDynamicParams->thrld = 5;

    if (min_exp == 8333 || min_exp == 10000)
    {
        TIAE_config_flicker_yes(aeDynamicParams, min_exp, min_exp);
    }
    else if (step_size == 8333 || step_size == 10000) 
    {
        TIAE_config_flicker_yes(aeDynamicParams, min_exp, step_size);
    }
    else if (step_size == 0) //no flicker at all
    {
        TIAE_config_flicker_none(aeDynamicParams, min_exp);
    }
    else
    {
        TIAE_config_flicker_yes(aeDynamicParams, min_exp, 8333);
    }
}
